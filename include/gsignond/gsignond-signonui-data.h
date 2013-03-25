/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of gsignond
 *
 * Copyright (C) 2012 Intel Corporation.
 *
 * Contact: Amarnath Valluri<amarnath.valluri@linux.intel.com>
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

#ifndef __GSIGNOND_SIGNONUI_DATA_H__
#define __GSIGNOND_SIGNONUI_DATA_H__

#include <gsignond/gsignond-dictionary.h>
#include <gsignond/gsignond-signonui.h>

G_BEGIN_DECLS

#define GSIGNOND_TYPE_SIGNONUI_DATA (GSIGNOND_TYPE_DICTIONARY)

#define GSIGNOND_SIGNONUI_DATA(obj)  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                           GSIGNOND_TYPE_SIGNONUI_DATA, \
                                           GSignondSignonuiData))
#define GSIGNOND_IS_SIGNONUI_DATA(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj),\
                                           GSIGNOND_TYPE_SIGNONUI_DATA))

typedef GSignondDictionary GSignondSignonuiData;

#define gsignond_signonui_data_new() gsignond_dictionary_new()

#define gsignond_signonui_data_new_from_variant(variantmap) gsignond_dictionary_new_from_variant(variantmap)

#define gsignond_signonui_data_to_variant(data) gsignond_dictionary_to_variant (data)

#define gsignond_signonui_data_ref(data) /*gsignond_dictionary_ref*/g_hash_table_ref (data)

#define gsignond_signonui_data_unref(data) /*gsignond_dictionary_unref*/g_hash_table_unref (data);

const gchar*
gsignond_signonui_data_get_captcha_response (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_captcha_response (GSignondSignonuiData *data,
                            const gchar *response);
const gchar*
gsignond_signonui_data_get_captcha_url (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_captcha_url (GSignondSignonuiData *data,
                       const gchar *url);
const gchar*
gsignond_signonui_data_get_caption (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_caption (GSignondSignonuiData *data,
                   const gchar *caption);
gboolean
gsignond_signonui_data_get_confirm (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_confirm (GSignondSignonuiData *data,
                   gboolean confirm);
const gchar*
gsignond_signonui_data_get_final_url (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_final_url (GSignondSignonuiData *data,
                     const gchar *url);
gboolean
gsignond_signonui_data_get_forgot_password (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_forgot_password (GSignondSignonuiData *data,
                                            gboolean forgot);
const gchar*
gsignond_signonui_data_get_forgot_password_url (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_forgot_password_url (GSignondSignonuiData *data,
                               const gchar *url);
const gchar*
gsignond_signonui_data_get_message (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_message (GSignondSignonuiData *data,
                   const gchar *message);
const gchar*
gsignond_signonui_data_get_open_url (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_open_url (GSignondSignonuiData *data,
                    const gchar *url);
const gchar*
gsignond_signonui_data_get_password (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_password (GSignondSignonuiData *data,
                    const gchar *password);
GSignondSignonuiError
gsignond_signonui_data_get_query_error (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_query_error (GSignondSignonuiData *data,
                       GSignondSignonuiError error);
gboolean
gsignond_signonui_data_get_query_password (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_query_password (GSignondSignonuiData *data,
                          gboolean query);
gboolean
gsignond_signonui_data_get_query_username (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_query_username (GSignondSignonuiData *data,
                                           gboolean query);
gboolean
gsignond_signonui_data_get_remember_password (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_remember_password (GSignondSignonuiData *data,
                             gboolean remember);
const gchar*
gsignond_signonui_data_get_request_id (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_request_id (GSignondSignonuiData *data,
                      const gchar *id);
const gchar*
gsignond_signonui_data_get_test_reply (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_test_reply (GSignondSignonuiData *data,
                      const gchar *reply);
const gchar*
gsignond_signonui_data_get_title (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_title (GSignondSignonuiData *data,
                 const gchar* title);
const gchar*
gsignond_signonui_data_get_url_response (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_url_response (GSignondSignonuiData *data,
                        const gchar *response);
const gchar*
gsignond_signonui_data_get_username (GSignondSignonuiData *data);
void
gsignond_signonui_data_set_username (GSignondSignonuiData *data,
                    const gchar *username);

G_END_DECLS

#endif /* __GSIGNOND_SIGNONUI_DATA_H__ */