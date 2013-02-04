/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of gsignond
 *
 * Copyright (C) 2012 Intel Corporation.
 *
 * Contact: Jussi Laako <jussi.laako@linux.intel.com>
            Amarnath Valluri <amarnath.valluri@linux.intel.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include <string.h>

#include "gsignond/gsignond-log.h"

#include "gsignond-identity-iface.h"
#include "dbus/gsignond-dbus.h"
#include "dbus/gsignond-dbus-identity-adapter.h"
#include "gsignond-identity.h"
#include "gsignond-auth-session.h"

enum 
{
    PROP_0,
    PROP_INFO,
    PROP_APP_CONTEXT,
    N_PROPERTIES
};

enum {
    SIG_STORE,
    SIG_REMOVE,
    SIG_VERIFY_USER,
    SIG_VERIFY_SECRET,
    SIG_ADD_REFERENCE,
    SIG_REMOVE_REFERENCE,
    SIG_SIGNOUT,
    SIG_MAX
};

static GParamSpec *properties[N_PROPERTIES];
static guint signals[SIG_MAX];

struct _GSignondIdentityPrivate
{
    GSignondIdentityInfo *info;
    GSignondAuthServiceIface *owner;
    GSignondDbusIdentityAdapter *identity_adapter;
    GHashTable *auth_sessions; /* "object_path":auth_session_object_path */
};

static void
gsignond_identity_iface_init (gpointer g_iface, gpointer iface_data);

G_DEFINE_TYPE_EXTENDED (GSignondIdentity, gsignond_identity, G_TYPE_OBJECT, 0,
                        G_IMPLEMENT_INTERFACE (GSIGNOND_TYPE_IDENTITY_IFACE, 
                                               gsignond_identity_iface_init));


#define GSIGNOND_IDENTITY_PRIV(obj) G_TYPE_INSTANCE_GET_PRIVATE ((obj), GSIGNOND_TYPE_IDENTITY, GSignondIdentityPrivate)

#define VALIDATE_IDENTITY_READ_ACCESS(identity, ctx, ret) \
{ \
    GSignondAccessControlManager *acm = gsignond_auth_service_iface_get_acm (identity->priv->owner); \
    GSignondSecurityContextList *acl = gsignond_identity_info_get_access_control_list (identity->priv->info); \
    gboolean valid = gsignond_access_control_manager_peer_is_allowed_to_use_identity (acm, ctx, acl); \
    gsignond_security_context_list_free (acl); \
    if (!valid) { \
        /* TODO: throw access error */ \
        return ret; \
    } \
}

#define VALIDATE_IDENTITY_WRITE_ACCESS(identity, ctx, ret) \
{ \
    GSignondAccessControlManager *acm = gsignond_auth_service_iface_get_acm (identity->priv->owner); \
    GSignondSecurityContextList *owners = gsignond_identity_info_get_access_control_list (identity->priv->info); \
    const GSignondSecurityContext *owner = (const GSignondSecurityContext *)g_list_first (owners); \
    gboolean valid = gsignond_access_control_manager_peer_is_owner_of_identity (acm, ctx, owner); \
    gsignond_security_context_list_free (owners); \
    if (!valid) { \
        /* TODO: throw access error */ \
        return ret; \
    } \
}

static void
_get_property (GObject *object, guint property_id, GValue *value,
               GParamSpec *pspec)
{
    GSignondIdentity *self = GSIGNOND_IDENTITY (object);

    switch (property_id)
    {
        case PROP_INFO:
            g_value_set_boxed (value, self->priv->info);
            break;
        case PROP_APP_CONTEXT:
            g_object_get_property (G_OBJECT (self->priv->identity_adapter), "app-context", value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
_set_property (GObject *object, guint property_id, const GValue *value,
               GParamSpec *pspec)
{
    GSignondIdentity *self = GSIGNOND_IDENTITY (object);

    switch (property_id)
    {
        case PROP_INFO:
            self->priv->info =
                GSIGNOND_IDENTITY_INFO (g_value_get_boxed (value));
            break;
        case PROP_APP_CONTEXT:
            g_object_set_property (G_OBJECT (self->priv->identity_adapter), "app-context", value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
_dispose (GObject *object)
{
    GSignondIdentity *self = GSIGNOND_IDENTITY(object);

    if (self->priv->identity_adapter) {
        g_object_unref (self->priv->identity_adapter);
        self->priv->identity_adapter = NULL;
    }

    if (self->priv->owner) {
        g_object_unref (self->priv->owner);
        self->priv->owner = NULL;
    }

    if (self->priv->info) {
        gsignond_identity_info_free (self->priv->info);
        self->priv->info = NULL;
    }
   
    G_OBJECT_CLASS (gsignond_identity_parent_class)->dispose (object);
}

static void
_finalize (GObject *object)
{
}

static void
gsignond_identity_init (GSignondIdentity *self)
{
    GError *err = NULL;
    self->priv = GSIGNOND_IDENTITY_PRIV(self);

    self->priv->identity_adapter =
        gsignond_dbus_identity_adapter_new (GSIGNOND_IDENTITY_IFACE (self));
    self->priv->auth_sessions = 
        g_hash_table_new_full (g_str_hash, g_str_equal, NULL, g_object_unref);

}

static void
gsignond_identity_class_init (GSignondIdentityClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (object_class, sizeof (GSignondIdentityPrivate));

    object_class->get_property = _get_property;
    object_class->set_property = _set_property;
    object_class->dispose = _dispose;
    object_class->finalize = _finalize;

    properties[PROP_INFO] =
        g_param_spec_boxed ("info", 
                            "identity info", 
                            "IdentityInfo structure",
                            GSIGNOND_TYPE_IDENTITY_INFO,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    
    properties[PROP_APP_CONTEXT] = g_param_spec_string (
                "app-context",
                "application security context",
                "Application security context of the identity object creater",
                NULL,
                G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);

    signals[SIG_REMOVE] = g_signal_new ("remove",
                  GSIGNOND_TYPE_IDENTITY,
                  G_SIGNAL_RUN_FIRST| G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_BOOLEAN,
                  1,
                  GSIGNOND_TYPE_IDENTITY_INFO);
    signals[SIG_STORE] = g_signal_new ("store",
                  GSIGNOND_TYPE_IDENTITY,
                  G_SIGNAL_RUN_FIRST | G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_UINT,
                  1,
                  GSIGNOND_TYPE_IDENTITY_INFO);
    signals[SIG_ADD_REFERENCE] = g_signal_new ("add-reference",
                  GSIGNOND_TYPE_IDENTITY,
                  G_SIGNAL_RUN_FIRST | G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_INT,
                  1,
                  G_TYPE_STRING);
    signals[SIG_REMOVE_REFERENCE] = g_signal_new ("remove-reference",
                  GSIGNOND_TYPE_IDENTITY,
                  G_SIGNAL_RUN_FIRST | G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_INT,
                  1,
                  G_TYPE_STRING);
    signals[SIG_SIGNOUT] = g_signal_new ("signout",
                  GSIGNOND_TYPE_IDENTITY,
                  G_SIGNAL_RUN_FIRST | G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_BOOLEAN,
                  0,
                  G_TYPE_NONE);
}

static gboolean
_request_credentials_update (GSignondIdentityIface *iface, const gchar *message, const GSignondSecurityContext *ctx) 
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY (iface);
    guint32 id = 0;

    if (!identity || !identity->priv->info) {
        /*
         * TODO: throw error
         */
        return FALSE;
    }
    VALIDATE_IDENTITY_READ_ACCESS (identity, ctx, FALSE);

    if (!gsignond_identity_info_get_store_secret (identity->priv->info)) {
        /*
         * TODO: throw error
         */
        return FALSE;
    }

    /*
     * TODO: Call UI to request credentials
     * and when ready, emit signal "store" to save the new credentials info
     * to database(which is handled by Daemon object).
     * On success, call 
     *      gsignond_identity_iface_notify_credentials_updated(self, id, NULL);
     * otherwise, calls
     *      gsignond_identity_iface_notify_credentials_updated(self, 0, error);
     */

     /*
      * emit "identity-data-updated"
      */

    return TRUE;
}

static GVariant * 
_get_info (GSignondIdentityIface *iface, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY (iface);
    GVariant *info = NULL;

    if (identity->priv->info) {
        gchar *secret = 0;
        gchar *username = 0;

        VALIDATE_IDENTITY_READ_ACCESS (identity, ctx, NULL);

        secret = g_strdup (gsignond_identity_info_get_secret (identity->priv->info));

        /* unset password */
        gsignond_identity_info_set_secret (identity->priv->info, "");

        /* unset username if its secret */
        if (gsignond_identity_info_get_is_username_secret (identity->priv->info)) {
            username = g_strdup (gsignond_identity_info_get_username (identity->priv->info));
            gsignond_identity_info_set_username (identity->priv->info, "");
        }

        /* prepare identity info, excluding password and username if secret */
        info = gsignond_dictionary_to_variant (identity->priv->info);

        /* reset password back */
        if (secret) {
            gsignond_identity_info_set_secret (identity->priv->info, secret);
            g_free (secret);
        }

        /* reset username back */
        if (username) {
            gsignond_identity_info_set_username (identity->priv->info, username);
            g_free (username);
        }
    }

    return info;
}

static void
_on_session_close (gpointer data, GObject *session)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY (data);

    g_object_weak_unref (session, _on_session_close, data);

    g_hash_table_remove (identity->priv->auth_sessions, 
                         (gpointer)gsignond_auth_session_get_object_path (
                            GSIGNOND_AUTH_SESSION (session)));
}

static const gchar *
_get_auth_session (GSignondIdentityIface *iface, const gchar *method, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY (iface);
    GSignondAuthSession *session = NULL;
    const gchar *object_path = NULL;

    g_value_return_if_fail (iface, NULL);
    g_value_return_if_fail (method, NULL);
    VALIDATE_IDENTITY_READ_ACCESS (identity, ctx, NULL);
    /*
     * FIXME: check if given method is supported
     */

    session = gsignond_auth_session_new (iface, method);

    if (!session)
        return NULL;

    object_path = gsignond_auth_session_get_object_path (session);

    g_hash_table_insert (identity->priv->auth_sessions, 
                         (gpointer)object_path, 
                         (gpointer)session);

    return object_path;
}

static gboolean 
_verify_user (GSignondIdentityIface *iface, const GVariant *params, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY (iface);
    guint32 id = 0;
    const gchar *passwd = 0;

    if (!identity || !identity->priv->info) {
        /*
         * TODO: throw error
         */
        return FALSE;
    }

    VALIDATE_IDENTITY_READ_ACCESS (identity, ctx, FALSE);

    if (!gsignond_identity_info_get_store_secret (identity->priv->info) ||
        !(passwd = gsignond_identity_info_get_secret (identity->priv->info)) ||
        !strlen (passwd)) {
        /*
         * TODO: password not stored to verify credentials, throw error
         */
        return FALSE;
    }

    /*
     * TODO: Call UI to request credentials
     * and when ready, emit signal "store" to save the new credentials info
     * to database(which is handled by Daemon object).
     * On success, call 
     *      gsignond_identity_iface_notify_credentials_updated(self, id, NULL);
     */    

    return TRUE;
}

static gboolean
_verify_secret (GSignondIdentityIface *iface, const gchar *secret, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY(iface);

    VALIDATE_IDENTITY_READ_ACCESS (identity, ctx, FALSE);

    return FALSE;
}

static gboolean 
_sign_out (GSignondIdentityIface *iface, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY(iface);
    gboolean success = FALSE;

    VALIDATE_IDENTITY_READ_ACCESS (identity, ctx, FALSE);

    /*
     * TODO: close all auth_sessions and emit "identity-signed-out"
     */

    g_signal_emit (iface,
                   signals[SIG_SIGNOUT],
                   0,
                   &success);

    return success;
}

static guint32
_store (GSignondIdentityIface *iface, const GVariant *info, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY(iface);
    GSignondIdentityInfo *identity_info = NULL;
    gboolean was_new_identity = FALSE;
    GSignondSecurityContextList *contexts = NULL;
    guint32 id;

    VALIDATE_IDENTITY_WRITE_ACCESS (identity, ctx, 0);

    identity_info = gsignond_dictionary_new_from_variant ((GVariant *)info);
    /* dont trust 'identity id' passed via 'info' */
    id = gsignond_identity_info_get_id (identity->priv->info);
    gsignond_identity_info_set_id (identity_info, id);

    was_new_identity = gsignond_identity_info_get_is_identity_new (identity_info);

    contexts = gsignond_identity_info_get_access_control_list (identity->priv->info);
    if (contexts) {
        GSignondAccessControlManager *acm = gsignond_auth_service_iface_get_acm (identity->priv->owner);
        gboolean valid = gsignond_access_control_manager_acl_is_valid (acm, ctx, contexts);
        gsignond_security_context_list_free (contexts);

        if (!valid) {
            /*
             * FIXME: throw error
             */

            return 0;
        }
    }

    /* Add creater to owner list */
    contexts = gsignond_identity_info_get_owner_list (identity_info);
    if (!contexts) {
        ctx = ctx ? gsignond_security_context_copy (ctx)
                  : gsignond_security_context_new_from_values ("*", NULL);

        contexts = g_list_append (contexts, (gpointer)ctx);

        gsignond_identity_info_set_owner_list (identity_info, contexts);
    }

    gsignond_security_context_list_free (contexts);

    /* update object cache */
    if (identity->priv->info) gsignond_identity_info_free (identity->priv->info);
    identity->priv->info = identity_info;

    /* Ask daemon to store identity info to db */
    g_signal_emit (identity,
                   signals[SIG_STORE],
                   0,
                   identity_info, 
                   &id);

    if (was_new_identity) 
        gsignond_identity_set_id (identity, id);

    gsignond_identity_iface_notify_info_updated (iface, GSIGNOND_IDENTITY_DATA_UPDATED);
 
    return id;
}

static void
_remove (GSignondIdentityIface *iface, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY(iface);
    
    VALIDATE_IDENTITY_WRITE_ACCESS (identity, ctx, );

    g_signal_emit (identity,
                   signals[SIG_REMOVE],
                   0,
                   identity->priv->info,
                   NULL);

    /*
     * TODO: emit "identity-removed"
     */
    gsignond_identity_iface_notify_info_updated (iface, GSIGNOND_IDENTITY_REMOVED);
}

static gint32
_add_reference (GSignondIdentityIface *iface, const gchar *reference, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY(iface);
    gint32 res;
    
    VALIDATE_IDENTITY_READ_ACCESS (identity, ctx, 0);

    g_signal_emit (iface,
                   signals[SIG_ADD_REFERENCE],
                   0,
                   reference,
                   &res);

    return res;
}

static gint32
_remove_reference (GSignondIdentityIface *iface, const gchar *reference, const GSignondSecurityContext *ctx)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY(iface);
    gint32 res;

    VALIDATE_IDENTITY_READ_ACCESS (identity, ctx, 0);

    g_signal_emit (iface,
                   signals[SIG_REMOVE_REFERENCE],
                   0,
                   reference,
                   &res);

    return res;
}

static GSignondAccessControlManager *
_get_acm (GSignondIdentityIface *iface)
{
    GSignondIdentity *identity = GSIGNOND_IDENTITY (iface);

    g_return_val_if_fail (identity, NULL);

    return gsignond_auth_service_iface_get_acm (identity->priv->owner);
}

static void
gsignond_identity_iface_init (gpointer g_iface, gpointer iface_data)
{
    GSignondIdentityIfaceInterface *identity_iface =
        (GSignondIdentityIfaceInterface *) g_iface;

    (void)iface_data;

    identity_iface->request_credentials_update = _request_credentials_update;
    identity_iface->get_info = _get_info;
    identity_iface->verify_user = _verify_user;
    identity_iface->verify_secret = _verify_secret;
    identity_iface->remove = _remove;
    identity_iface->sign_out = _sign_out;
    identity_iface->store = _store;
    identity_iface->add_reference = _add_reference;
    identity_iface->remove_reference = _remove_reference;
    identity_iface->get_acm = _get_acm;
}

/**
 * gsignond_identity_get_id:
 * @identity: instance of #GSignondIdentity
 * 
 * Retrieves identity id.
 *
 * Returns: identity id
 */
guint32 
gsignond_identity_get_id (GSignondIdentity *identity)
{
    return gsignond_identity_info_get_id (identity->priv->info);
}

/**
 * gsignond_identity_set_id:
 * @identity: instance of #GSignondIdentity
 * @id: unique identifier
 * 
 * Sets the #identity id to #id.
 *
 * Returns[transfer null]: Dbus object path used by this identity.
 */
gboolean 
gsignond_identity_set_id (GSignondIdentity *identity, guint32 id)
{
    gsignond_identity_info_set_id (identity->priv->info, id);
    g_object_notify_by_pspec (G_OBJECT(identity), properties[PROP_INFO]);

    return TRUE;
}

/**
 * gsignond_identity_get_object_path:
 * @identity: instance of #GSignondIdentity
 * 
 * Retrieves the dbus object path of the identity.
 *
 * Returns[transfer null]: Dbus object path used by this identity.
 */
const gchar *
gsignond_identity_get_object_path (GSignondIdentity *identity)
{
    return gsignond_dbus_identity_adapter_get_object_path (identity->priv->identity_adapter);
}

/**
 * gsignond_identity_new:
 * @owner: Owner of this object, instance of #GSignondAuthServiceIface
 * @info (transfer full): Identity info, instance of #GSignondIdentityInfo
 * @app_context: application security context
 * 
 * Creates new instance of #GSignondIdentity
 *
 * Returns[transfer full]: new instance of #GSignondIdentity
 */
GSignondIdentity * gsignond_identity_new (GSignondAuthServiceIface *owner,
                                          GSignondIdentityInfo *info,
                                          const gchar *app_context)
{
    GSignondIdentity *identity =
        GSIGNOND_IDENTITY(g_object_new (GSIGNOND_TYPE_IDENTITY,
                                        "info", info,
                                        "app-context", app_context,
                                        NULL));

    identity->priv->owner = g_object_ref (owner);

    return identity;
}
