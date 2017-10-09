/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of gsignond
 *
 * Copyright (C) 2012-2013 Intel Corporation.
 *
 * Contact: Alexander Kanavin <alex.kanavin@gmail.com>
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

#include "gsignond-session-data.h"
#include "gsignond-utils.h"


/**
 * SECTION:gsignond-session-data
 * @short_description: definitions for authentication session parameters
 * @title: GSignondSessionData
 * @include: gsignond/gsignond-session-data.h
 *
 * This file provides commonly used parameters for authentication sessions.
 * For each of those a getter and setter is defined, on #GSignondSessionData
 * container. The plugins may not use all of these parameters, and they commonly
 * require additional, custom parameters which are set using #GSignondDictionary
 * setters with explicit key string.
 */

/**
 * GSignondSessionData:
 *
 * Opaque #GSignondSessionData data structure.
 */
/**
 * GSignondSessionDataClass:
 *
 * Opaque #GSignondSessionDataClass data structure.
 */

G_DEFINE_TYPE (GSignondSessionData, gsignond_session_data, GSIGNOND_TYPE_DICTIONARY);

/**
 * GSignondUiPolicy:
 * @GSIGNOND_UI_POLICY_DEFAULT: use a default user interaction scenario
 * @GSIGNOND_UI_POLICY_REQUEST_PASSWORD: force an authorization request from the user;
 * any cached access tokens should be discarded by the plugin.
 * @GSIGNOND_UI_POLICY_NO_USER_INTERACTION: force no interaction with the user
 * @GSIGNOND_UI_POLICY_VALIDATION: interaction with the user is only allowed
 * for validation captchas and similar security measures
 * 
 * Policy setting to define how plugins should handle interaction with the user.
 */

static void
gsignond_session_data_class_init (GSignondSessionDataClass *klass)
{
  
}

static void
gsignond_session_data_init (GSignondSessionData *self)
{
  /* initialize the object */
}

/**
 * gsignond_session_data_new_from_variant:
 * @variant: instance of #GVariant
 *
 * Converts the #GVariant to #GSignondSessionData. This is useful for example if 
 * the dictionary needs to be deserialized, or if it's contained in another 
 * #GSignondSessionData and has been retrieved using gsignond_dictionary_get().
 *
 * Returns: (transfer full): #GSignondSessionData if successful, NULL otherwise.
 */
GSignondSessionData *
gsignond_session_data_new_from_variant (GVariant *variant)
{
    GSignondSessionData *session_data = NULL;
    GVariantIter iter;
    gchar *key = NULL;
    GVariant *value = NULL;
    GHashTable * table = NULL;

    g_return_val_if_fail (variant != NULL, NULL);

    session_data = gsignond_session_data_new ();
    table = gsignond_dictionary_get_table (GSIGNOND_DICTIONARY (session_data));
    g_variant_iter_init (&iter, variant);
    while (g_variant_iter_next (&iter, "{sv}", &key, &value))
    {
        g_hash_table_insert (table, key, value);
    }

    return session_data;
}

/**
 * gsignond_session_data_new:
 *
 * Creates a new instance of #GSignondSessionData.
 *
 * Returns: (transfer full): #GSignondSessionData object if successful,
 * NULL otherwise.
 */
GSignondSessionData *
gsignond_session_data_new (void)
{
    return GSIGNOND_SESSION_DATA (
            g_object_new (GSIGNOND_TYPE_SESSION_DATA, NULL));
}

/**
 * gsignond_session_data_get_username:
 * @data: a #GSignondDictionary structure
 * 
 * A getter for a username associated with the authentication session.
 * 
 * Returns: (transfer none)
 */
const gchar *
gsignond_session_data_get_username (GSignondSessionData *data)
{
    return gsignond_dictionary_get_string (GSIGNOND_DICTIONARY(data), "UserName");
}

/**
 * gsignond_session_data_set_username:
 * @data: a #GSignondDictionary structure
 * @username: username to set
 * 
 * A setter for a username associated with the authentication session.
 */
void
gsignond_session_data_set_username (GSignondSessionData *data, 
                                    const gchar *username)
{
    gsignond_dictionary_set_string (GSIGNOND_DICTIONARY(data), "UserName", username);
}

/**
 * gsignond_session_data_get_secret:
 * @data: a #GSignondDictionary structure
 * 
 * A getter for a secret (e.g. a password) associated with the authentication session.
 * 
 * Returns: (transfer none)
 */
const gchar *
gsignond_session_data_get_secret (GSignondSessionData *data)
{
    return gsignond_dictionary_get_string (GSIGNOND_DICTIONARY(data), "Secret");
}

/**
 * gsignond_session_data_set_secret:
 * @data: a #GSignondDictionary structure
 * @secret: a secret to set
 * 
 * A setter for a secret (e.g. a password) associated with the authentication session.
 */
void
gsignond_session_data_set_secret (GSignondSessionData *data, 
                                  const gchar *secret)
{
    gsignond_dictionary_set_string (GSIGNOND_DICTIONARY(data), "Secret", secret);
}

/**
 * gsignond_session_data_get_realm:
 * @data: a #GSignondDictionary structure
 * 
 * A getter for a realm associated with the authentication session.
 * 
 * Returns: (transfer none)
 */
const gchar *
gsignond_session_data_get_realm (GSignondSessionData *data)
{
    return gsignond_dictionary_get_string (GSIGNOND_DICTIONARY(data), "Realm");
}

/**
 * gsignond_session_data_set_allowed_realms:
 * @data: a #GSignondDictionary structure
 * @realms: a #GSequence if allowed realms
 *
 * A setter for a list of realms allowed for the identity use.
 */
void
gsignond_session_data_set_allowed_realms (GSignondSessionData *data,
                                          GSequence *realms)
{
    gsignond_dictionary_set (GSIGNOND_DICTIONARY(data), "AllowedRealms",
                             gsignond_sequence_to_variant (realms));
}

/**
 * gsignond_session_data_get_allowed_realms: (skip)
 * @data: a #GSignondDictionary structure
 *
 * A getter for a list of realms allowed for the identity use.
 *
 * Returns: (transfer full): #GSequence of allowed realms
 */
GSequence *
gsignond_session_data_get_allowed_realms (GSignondSessionData *data)
{
    return gsignond_variant_to_sequence (gsignond_dictionary_get (GSIGNOND_DICTIONARY(data),
                                                                  "AllowedRealms"));
}

/**
 * gsignond_session_data_set_realm:
 * @data: a #GSignondDictionary structure
 * @realm: a realm to set
 * 
 * A setter for a realm associated with the authentication session.
 */
void
gsignond_session_data_set_realm (GSignondSessionData *data,
                                 const gchar *realm)
{
    gsignond_dictionary_set_string (GSIGNOND_DICTIONARY(data), "Realm", realm);
}

/**
 * gsignond_session_data_get_caption:
 * @data: a #GSignondDictionary structure
 * 
 * A getter for a caption associated with the authentication session.
 * Caption tells the user which application/credentials/provider is requestion
 * authentication.
 * 
 * Returns: (transfer none)
 */
const gchar *
gsignond_session_data_get_caption (GSignondSessionData *data)
{
    return gsignond_dictionary_get_string (GSIGNOND_DICTIONARY(data), "Caption");
}

/**
 * gsignond_session_data_set_caption:
 * @data: a #GSignondDictionary structure
 * @caption: a caption to set
 * 
 * A setter for a caption associated with the authentication session.
 * Caption tells the user which application/credentials/provider is requestion
 * authentication.
 */
void
gsignond_session_data_set_caption (GSignondSessionData *data,
                                   const gchar *caption)
{
    gsignond_dictionary_set_string (GSIGNOND_DICTIONARY(data), "Caption", caption);
}

/**
 * gsignond_session_data_get_renew_token:
 * @data: a #GSignondDictionary structure
 * @renew_token: (out): the value for the parameter is written here
 * 
 * A getter for a renew token property associated with the authentication session.
 * This property tells the plugin to discard any cached tokens and start 
 * the authentication process anew.
 * 
 * Returns: whether the key-value pair exists in the @data dictionary or not.
 */
gboolean
gsignond_session_data_get_renew_token (GSignondSessionData *data,
                                       gboolean *renew_token)
{
    return gsignond_dictionary_get_boolean (GSIGNOND_DICTIONARY(data), "RenewToken", renew_token);
}

/**
 * gsignond_session_data_set_renew_token:
 * @data: a #GSignondDictionary structure
 * @renew_token: whether to renew the token set
 * 
 * A setter for a renew token property associated with the authentication session.
 * This property tells the plugin to discard any cached tokens and start 
 * the authentication process anew.
 */
void
gsignond_session_data_set_renew_token (GSignondSessionData *data,
                                       gboolean renew_token)
{
    gsignond_dictionary_set_boolean (GSIGNOND_DICTIONARY(data), "RenewToken", renew_token);
}

/**
 * gsignond_session_data_get_ui_policy:
 * @data: a #GSignondDictionary structure
 * @ui_policy: the value for the parameter is written here
 * 
 * A getter for UI policy setting associated with the authentication session.
 * The UI policy indicates how the authentication plugin should interact with the user.
 * 
 * Returns: whether the key-value pair exists in the @data dictionary or not.
 */
gboolean
gsignond_session_data_get_ui_policy (GSignondSessionData *data,
                                     GSignondUiPolicy *ui_policy)
{
    return gsignond_dictionary_get_uint32 (GSIGNOND_DICTIONARY(data), "UiPolicy", ui_policy);
}

/**
 * gsignond_session_data_set_ui_policy:
 * @data: a #GSignondDictionary structure
 * @ui_policy: ui policy to set
 * 
 * A getter for UI policy setting associated with the authentication session.
 * The UI policy indicates how the authentication plugin should interact with the user.
 */
void
gsignond_session_data_set_ui_policy (GSignondSessionData *data, 
                                     GSignondUiPolicy ui_policy)
{
    gsignond_dictionary_set_uint32 (GSIGNOND_DICTIONARY(data), "UiPolicy", ui_policy);
}    

/**
 * gsignond_session_data_get_network_proxy:
 * @data: a #GSignondDictionary structure
 * 
 * A getter for a network proxy setting associated with the authentication session.
 * If this property is not set, the default system proxy settings should be used.
 * 
 * Returns: (transfer none)
 */
const gchar *
gsignond_session_data_get_network_proxy (GSignondSessionData *data)
{
    return gsignond_dictionary_get_string (GSIGNOND_DICTIONARY(data), "NetworkProxy");
}

/**
 * gsignond_session_data_set_network_proxy:
 * @data: a #GSignondDictionary structure
 * @network_proxy: network proxy to use
 * 
 * A setter for a network proxy setting associated with the authentication session.
 * If this property is not set, the default system proxy settings should be used.
 */
void
gsignond_session_data_set_network_proxy (GSignondSessionData *data,
                                         const gchar *network_proxy)
{
    gsignond_dictionary_set_string (GSIGNOND_DICTIONARY(data), "NetworkProxy", network_proxy);
}

/**
 * gsignond_session_data_get_network_timeout:
 * @data: a #GSignondDictionary structure
 * @network_timeout: (out): the value for the parameter is written here
 * 
 * A getter for a network timeout setting associated with the authentication session.
 * This can be used to change the default timeout in case of unresponsive servers.
 * 
 * Returns: whether the key-value pair exists in the @data dictionary or not.
 */
gboolean
gsignond_session_data_get_network_timeout (GSignondSessionData *data,
                                           guint32 *network_timeout)
{
    return gsignond_dictionary_get_uint32 (GSIGNOND_DICTIONARY(data), "NetworkTimeout",
                                           network_timeout);
}

/**
 * gsignond_session_data_set_network_timeout:
 * @data: a #GSignondDictionary structure
 * @network_timeout: network timeout to use
 * 
 * A setter for a network timeout setting associated with the authentication session.
 * This can be used to change the default timeout in case of unresponsive servers.
 */
void
gsignond_session_data_set_network_timeout (GSignondSessionData *data,
                                           guint32 network_timeout)
{
    gsignond_dictionary_set_uint32 (GSIGNOND_DICTIONARY(data), "NetworkTimeout",
                                    network_timeout);
}

/**
 * gsignond_session_data_get_window_id:
 * @data: a #GSignondDictionary structure
 * @window_id: (out): the value for the parameter is written here
 * 
 * A getter for a window id setting associated with the authentication session.
 * This can be used to embed the user interaction window produced by the authentication
 * session into an application window.
 * 
 * Returns: whether the key-value pair exists in the @data dictionary or not.
 */
gboolean
gsignond_session_data_get_window_id (GSignondSessionData *data,
                                     guint32 *window_id)
{
    return gsignond_dictionary_get_uint32 (GSIGNOND_DICTIONARY(data), "WindowId", window_id);
}

/**
 * gsignond_session_data_set_window_id:
 * @data: a #GSignondDictionary structure
 * @window_id: window id to use
 * 
 * A setter for a window id setting associated with the authentication session.
 * This can be used to embed the user interaction window produced by the authentication
 * session into an application window.
 */
void
gsignond_session_data_set_window_id (GSignondSessionData *data,
                                     guint32 window_id)
{
    gsignond_dictionary_set_uint32 (GSIGNOND_DICTIONARY(data), "WindowId", window_id);
}

/**
 * gsignond_session_data_copy:
 * @other: instance of #GSignondSessionData
 *
 * Creates a copy of the dictionary session data.
 *
 * Returns: (transfer full): #GSignondSessionData object if the copy was successful,
 * NULL otherwise.
 */
GSignondSessionData *
gsignond_session_data_copy (GSignondSessionData *other)
{
    GSignondSessionData *session = NULL;
    GHashTable *other_table = NULL;
    GSignondDictionary *session_dict = NULL;
    GHashTableIter iter;
    gchar *key = NULL;
    GVariant *value = NULL;

    g_return_val_if_fail (other != NULL, NULL);

    session = gsignond_session_data_new ();
    other_table = gsignond_dictionary_get_table (GSIGNOND_DICTIONARY (other));
    session_dict = GSIGNOND_DICTIONARY (session);

    g_hash_table_iter_init (&iter, other_table);
    while (g_hash_table_iter_next (&iter,
                                   (gpointer)&key,
                                   (gpointer)&value))
    {
        gsignond_dictionary_set (session_dict, key, value);
    }
    

    return session;
}

