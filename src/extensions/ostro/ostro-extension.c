/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of gsignond
 *
 * Copyright (C) 2013-2016 Intel Corporation.
 *
 * Contact: Jussi Laako <jussi.laako@linux.intel.com>
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

#include "config.h"

#include "ostro-extension.h"
#include "ostro-storage-manager.h"
#include "ostro-secret-storage.h"
#include "ostro-access-control-manager.h"

G_DEFINE_TYPE (ExtensionOstro, extension_ostro, GSIGNOND_TYPE_EXTENSION);

#define EXTENSION_OSTRO_PRIV(obj) G_TYPE_INSTANCE_GET_PRIVATE ((obj), EXTENSION_TYPE_OSTRO, ExtensionOstroPrivate)

struct _ExtensionOstroPrivate
{
    GSignondAccessControlManager *access_control_manager;
    GSignondStorageManager *storage_manager;
    GSignondSecretStorage *secret_storage;
};

static void
_dispose (GObject *obj)
{
    ExtensionOstro *self = EXTENSION_OSTRO (obj);
    if (!self) return;

    ExtensionOstroPrivate *priv = self->priv;

    if (priv) {
        g_clear_object (&priv->access_control_manager);
        g_clear_object (&priv->secret_storage);
        g_clear_object (&priv->storage_manager);
    }

    G_OBJECT_CLASS (extension_ostro_parent_class)->dispose (obj);
}

static const gchar *
_get_extension_name (GSignondExtension *self)
{
    (void) self;

    return "ostro";
}

static guint32
_get_extension_version (GSignondExtension *self)
{
    (void) self;

    return 0x01000000;
}

static GSignondStorageManager *
_get_storage_manager (GSignondExtension *self, GSignondConfig *config)
{
    g_return_val_if_fail (self && EXTENSION_IS_OSTRO(self), NULL);

    ExtensionOstroPrivate *priv = EXTENSION_OSTRO(self)->priv;

    if (!priv->storage_manager) {
        priv->storage_manager =
            g_object_new (EXTENSION_TYPE_OSTRO_STORAGE_MANAGER,
                          "config", config, NULL);
    }
    return priv->storage_manager;
}

static GSignondSecretStorage *
_get_secret_storage (GSignondExtension *self, GSignondConfig *config)
{
    g_return_val_if_fail (self && EXTENSION_IS_OSTRO(self), NULL);

    ExtensionOstroPrivate *priv = EXTENSION_OSTRO(self)->priv;

    if (!priv->secret_storage) {
        priv->secret_storage =
            g_object_new (EXTENSION_TYPE_OSTRO_SECRET_STORAGE,
                          "config", config, NULL);
    }

    return priv->secret_storage;
}

#ifdef HAVE_LIBSMACK
static GSignondAccessControlManager *
_get_access_control_manager (GSignondExtension *self, GSignondConfig *config)
{
    g_return_val_if_fail (self && EXTENSION_IS_OSTRO(self), NULL);

    ExtensionOstroPrivate *priv = EXTENSION_OSTRO(self)->priv;

    if (!priv->access_control_manager) {
        priv->access_control_manager =
            g_object_new (EXTENSION_TYPE_OSTRO_ACCESS_CONTROL_MANAGER,
                          "config", config, NULL);
    }

    return priv->access_control_manager;
}
#endif  /* HAVE_LIBSMACK */

static void
extension_ostro_class_init (ExtensionOstroClass *klass)
{
    GSignondExtensionClass *parent_class = GSIGNOND_EXTENSION_CLASS (klass);

    g_type_class_add_private (G_OBJECT_CLASS(klass), sizeof (ExtensionOstroPrivate));

    G_OBJECT_CLASS (klass)->dispose = _dispose;
    parent_class->get_extension_name = _get_extension_name;
    parent_class->get_extension_version = _get_extension_version;
    parent_class->get_storage_manager = _get_storage_manager;
    parent_class->get_secret_storage = _get_secret_storage;
#   ifdef HAVE_LIBSMACK
    parent_class->get_access_control_manager = _get_access_control_manager;
#   endif  /* HAVE_LIBSMACK */
}

static void
extension_ostro_init (ExtensionOstro *self)
{
    self->priv = EXTENSION_OSTRO_PRIV (self);

    self->priv->storage_manager = NULL;
    self->priv->secret_storage = NULL;
    self->priv->access_control_manager = NULL;
}

static void
_on_object_dispose (gpointer data, GObject *object)
{
    if (data) *(ExtensionOstro **)data = NULL;
}

GSignondExtension *
ostro_extension_init ()
{
    static GSignondExtension *ostro_extension  = NULL;

    if (!ostro_extension) {
        ostro_extension = g_object_new (EXTENSION_TYPE_OSTRO, NULL);

        g_object_weak_ref (G_OBJECT (ostro_extension), _on_object_dispose, &ostro_extension);
    }

    return ostro_extension;
}

