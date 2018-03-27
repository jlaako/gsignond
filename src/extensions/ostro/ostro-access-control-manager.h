/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of gsignond
 *
 * Copyright (C) 2013-2016 Intel Corporation.
 *
 * Contact: Jussi Laako <jussi.laako@linux.intel.com>
 * Contact: Elena Reshetova <elena.reshetova@linux.intel.com>
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

#ifndef _OSTRO_ACCESS_CONTROL_MANAGER_H_
#define _OSTRO_ACCESS_CONTROL_MANAGER_H_

#include <glib-object.h>
#include <gsignond.h>

G_BEGIN_DECLS

#define EXTENSION_TYPE_OSTRO_ACCESS_CONTROL_MANAGER \
    (extension_ostro_access_control_manager_get_type ())
#define EXTENSION_OSTRO_ACCESS_CONTROL_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                 EXTENSION_TYPE_OSTRO_ACCESS_CONTROL_MANAGER, \
                                 ExtensionOstroAccessControlManager))
#define EXTENSION_IS_OSTRO_ACCESS_CONTROL_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 EXTENSION_TYPE_OSTRO_ACCESS_CONTROL_MANAGER))
#define EXTENSION_OSTRO_ACCESS_CONTROL_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), \
                              EXTENSION_TYPE_OSTRO_ACCESS_CONTROL_MANAGER, \
                              ExtensionOstroAccessControlManagerClass))
#define EXTENSION_IS_OSTRO_ACCESS_CONTROL_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                              EXTENSION_TYPE_OSTRO_ACCESS_CONTROL_MANAGER))
#define EXTENSION_OSTRO_ACCESS_CONTROL_MANAGER_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                EXTENSION_TYPE_OSTRO_ACCESS_CONTROL_MANAGER, \
                                ExtensionOstroAccessControlManagerClass))

typedef struct _ExtensionOstroAccessControlManager
    ExtensionOstroAccessControlManager;
typedef struct _ExtensionOstroAccessControlManagerClass
    ExtensionOstroAccessControlManagerClass;
typedef struct _ExtensionOstroAccessControlManagerPrivate
    ExtensionOstroAccessControlManagerPrivate;

struct _ExtensionOstroAccessControlManager
{
    GSignondAccessControlManager parent_instance;
    ExtensionOstroAccessControlManagerPrivate *priv;
};

struct _ExtensionOstroAccessControlManagerClass
{
    GSignondAccessControlManagerClass parent_class;
};

GType extension_ostro_access_control_manager_get_type (void);

void
extension_ostro_access_control_manager_security_context_of_peer (
                            GSignondAccessControlManager *self,
                            GSignondSecurityContext *peer_ctx,
                            int peer_fd, const gchar *peer_service,
                            const gchar *peer_app_ctx);

gboolean
extension_ostro_access_control_manager_peer_is_allowed_to_use_identity (
                            GSignondAccessControlManager *self,
                            const GSignondSecurityContext *peer_ctx,
                            const GSignondSecurityContext *identity_owner,
                            const GList *identity_acl);

gboolean
extension_ostro_access_control_manager_peer_is_owner_of_identity (
                            GSignondAccessControlManager *self,
                            const GSignondSecurityContext *peer_ctx,
                            const GSignondSecurityContext *identity_owner);

GSignondSecurityContext *
extension_ostro_access_control_manager_security_context_of_keychain (
                            GSignondAccessControlManager *self);

G_END_DECLS

#endif  /* _OSTRO_ACCESS_CONTROL_MANAGER_H_ */

