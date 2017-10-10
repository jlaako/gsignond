/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of gsignond
 *
 * Copyright (C) 2017 elementary LLC.
 *
 * Contact: Corentin Noël <corentin@elementary.io>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include "desktop-secret-storage.h"

struct _GSignondDesktopSecretStoragePrivate
{
    GError *error;
};

G_DEFINE_TYPE (GSignondDesktopSecretStorage,
               gsignond_desktop_secret_storage,
               GSIGNOND_TYPE_SECRET_STORAGE);

#define GSIGNOND_SECRET_STORAGE_PRIV(obj) G_TYPE_INSTANCE_GET_PRIVATE ((obj), GSIGNOND_TYPE_SECRET_STORAGE, GSignondDesktopSecretStoragePrivate)

enum
{
    SECRET_TYPE_PASSWORD,
    SECRET_TYPE_USERNAME,
    SECRET_TYPE_DATA
};
static gboolean
_open_db (GSignondSecretStorage *self)
{
    return TRUE;
}

static gboolean
_close_db (GSignondSecretStorage *self)
{
    return TRUE;
}

static gboolean
_clear_db (GSignondSecretStorage *self)
{
    return TRUE;
}

static gboolean
_is_open_db (GSignondSecretStorage *self)
{
    return TRUE;
}

static GSignondCredentials *
_load_credentials (GSignondSecretStorage *self, const guint32 id)
{
    GSignondDesktopSecretStorage* desktop_ss = GSIGNOND_DESKTOP_SECRET_STORAGE (self);
    GError *error = NULL;
    GSignondCredentials *result = NULL;
    gchar *password = NULL;
    gchar *username = NULL;

    password = secret_password_lookup_sync (DESKTOP_SECRET_STORAGE_SCHEMA, NULL, &error,
                                            "type", SECRET_TYPE_PASSWORD,
                                            "id", id, NULL);
    if (error != NULL) {
        if (desktop_ss->priv->error != NULL) {
            g_error_free (desktop_ss->priv->error);
        }

        desktop_ss->priv->error = error;
        return NULL;
    }

    username = secret_password_lookup_sync (DESKTOP_SECRET_STORAGE_SCHEMA, NULL, &error,
                                            "type", SECRET_TYPE_USERNAME,
                                            "id", id, NULL);
    if (error != NULL) {
        if (desktop_ss->priv->error != NULL) {
            g_error_free (desktop_ss->priv->error);
        }

        desktop_ss->priv->error = error;
    }

    result = gsignond_credentials_new ();
    gsignond_credentials_set_id (result, id);
    gsignond_credentials_set_password (result, password);
    gsignond_credentials_set_username (result, username);

    secret_password_free (password);
    secret_password_free (username);
    return result;
}

static gboolean
_update_credentials (GSignondSecretStorage *self, GSignondCredentials* creds)
{
    GSignondDesktopSecretStorage* desktop_ss = GSIGNOND_DESKTOP_SECRET_STORAGE (self);
    GError *error = NULL;
    gchar *label = NULL;
    const gchar *password = gsignond_credentials_get_password (creds);
    const gchar *username = gsignond_credentials_get_username (creds);
    guint32 id = gsignond_credentials_get_id (creds);
    gboolean result = TRUE;

    if (password != NULL) {
        label = g_strdup_printf ("Signon: [id: %u, type: %d]", id, SECRET_TYPE_PASSWORD);
        secret_password_store_sync (DESKTOP_SECRET_STORAGE_SCHEMA, SECRET_COLLECTION_DEFAULT,
                                    label, password, NULL, &error,
                                    "type", SECRET_TYPE_PASSWORD,
                                    "id", id, NULL);
        if (error != NULL) {
            if (desktop_ss->priv->error != NULL) {
                g_error_free (desktop_ss->priv->error);
            }

            desktop_ss->priv->error = error;
            result = FALSE;
        }

        g_free (label);
    }

    if (username != NULL) {
        label = g_strdup_printf ("Signon: [id: %u, type: %d]", id, SECRET_TYPE_USERNAME);
        secret_password_store_sync (DESKTOP_SECRET_STORAGE_SCHEMA, SECRET_COLLECTION_DEFAULT,
                                    label, username, NULL, &error,
                                    "type", SECRET_TYPE_USERNAME,
                                    "id", id, NULL);
        if (error != NULL) {
            if (desktop_ss->priv->error != NULL) {
                g_error_free (desktop_ss->priv->error);
            }

            desktop_ss->priv->error = error;
            result = FALSE;
        }

        g_free (label);
    }

    return result;
}

static gboolean
_remove_credentials (GSignondSecretStorage *self, const guint32 id)
{
    GSignondDesktopSecretStorage* desktop_ss = GSIGNOND_DESKTOP_SECRET_STORAGE (self);
    GError *error = NULL;
    secret_password_clear_sync (DESKTOP_SECRET_STORAGE_SCHEMA, NULL, &error,
                                "id", id, NULL);

    if (error != NULL) {
        if (desktop_ss->priv->error != NULL) {
            g_error_free (desktop_ss->priv->error);
        }

        desktop_ss->priv->error = error;
        return FALSE;
    }

    return TRUE;
}

static GSignondDictionary *
_load_data (GSignondSecretStorage *self, const guint32 id, const guint32 method)
{
    GSignondDesktopSecretStorage* desktop_ss = GSIGNOND_DESKTOP_SECRET_STORAGE (self);
    gchar *data_string = NULL;
    GVariant *data_variant = NULL;
    GSignondDictionary *result = NULL;
    GError *error = NULL;

    data_string = secret_password_lookup_sync (DESKTOP_SECRET_STORAGE_SCHEMA, NULL, &error,
                                               "type", SECRET_TYPE_DATA,
                                               "id", id,
                                               "method", method, NULL);
    if (error != NULL) {
        if (desktop_ss->priv->error != NULL) {
            g_error_free (desktop_ss->priv->error);
        }

        desktop_ss->priv->error = error;
        return NULL;
    } else if (data_string == NULL) {
        return NULL;
    }

    data_variant = g_variant_parse (NULL, data_string, NULL, NULL, &error);
    g_free (data_string);
    if (error != NULL) {
        if (desktop_ss->priv->error != NULL) {
            g_error_free (desktop_ss->priv->error);
        }

        desktop_ss->priv->error = error;
        return NULL;
    }

    result = gsignond_dictionary_new_from_variant (data_variant);
    g_variant_unref (data_variant);
    return result;
}

static gboolean
_update_data (GSignondSecretStorage *self, const guint32 id, const guint32 method, GSignondDictionary *data)
{
    GSignondDesktopSecretStorage* desktop_ss = GSIGNOND_DESKTOP_SECRET_STORAGE (self);
    GVariant *data_variant = gsignond_dictionary_to_variant (data);
    gchar *data_string = g_variant_print (data_variant, TRUE);
    gchar *label = NULL;
    GError *error = NULL;
    gboolean result = TRUE;

    g_variant_unref (data_variant);

    label = g_strdup_printf ("Signon: [id: %u, type: %d]", id, SECRET_TYPE_DATA);
    secret_password_store_sync (DESKTOP_SECRET_STORAGE_SCHEMA, SECRET_COLLECTION_DEFAULT,
                                label, data_string, NULL, &error,
                                "type", SECRET_TYPE_DATA,
                                "id", id,
                                "method", method, NULL);
    if (error != NULL) {
        if (desktop_ss->priv->error != NULL) {
            g_error_free (desktop_ss->priv->error);
        }
        desktop_ss->priv->error = error;
        result = FALSE;
    }

    g_free (label);
    g_free (data_string);
    return result;
}

static gboolean
_remove_data (GSignondSecretStorage *self, const guint32 id, const guint32 method)
{
    GSignondDesktopSecretStorage* desktop_ss = GSIGNOND_DESKTOP_SECRET_STORAGE (self);
    GError *error = NULL;
    secret_password_clear_sync (DESKTOP_SECRET_STORAGE_SCHEMA, NULL, &error,
                                "type", SECRET_TYPE_DATA,
                                "id", id,
                                "method", method,
                                NULL);

    if (error != NULL) {
        if (desktop_ss->priv->error != NULL) {
            g_error_free (desktop_ss->priv->error);
        }
        desktop_ss->priv->error = error;
        return FALSE;
    }

    return TRUE;
}

static const GError *
_get_last_error (GSignondSecretStorage *self)
{
    GSignondDesktopSecretStorage* desktop_ss = GSIGNOND_DESKTOP_SECRET_STORAGE (self);
    return desktop_ss->priv->error;
}

static void
gsignond_desktop_secret_storage_class_init (GSignondDesktopSecretStorageClass *klass)
{
    GSignondSecretStorageClass* parent_class = GSIGNOND_SECRET_STORAGE_CLASS (klass);
    GObjectClass* object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (object_class, sizeof (GSignondDesktopSecretStoragePrivate));

    parent_class->open_db = _open_db;
    parent_class->close_db = _close_db;
    parent_class->clear_db = _clear_db;
    parent_class->is_open_db = _is_open_db;
    parent_class->load_credentials = _load_credentials;
    parent_class->update_credentials = _update_credentials;
    parent_class->remove_credentials = _remove_credentials;
    parent_class->load_data = _load_data;
    parent_class->update_data = _update_data;
    parent_class->remove_data = _remove_data;
    parent_class->get_last_error = _get_last_error;
}

static void
gsignond_desktop_secret_storage_init (GSignondDesktopSecretStorage *self)
{
    self->priv = GSIGNOND_SECRET_STORAGE_PRIV(self);
    self->priv->error = NULL;
}

const SecretSchema *
_desktop_secret_storage_get_schema (void)
{
    static const SecretSchema dss_schema = {
        "com.gitlab.accounts-sso.Secrets", SECRET_SCHEMA_NONE,
        {
            {  "type", SECRET_SCHEMA_ATTRIBUTE_INTEGER },
            {  "id", SECRET_SCHEMA_ATTRIBUTE_INTEGER },
            {  "method", SECRET_SCHEMA_ATTRIBUTE_INTEGER },
            {  "NULL", 0 },
        }
    };

    return &dss_schema;
}

