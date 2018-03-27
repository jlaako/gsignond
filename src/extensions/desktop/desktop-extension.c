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

#include "desktop-extension.h"
#include "desktop-secret-storage.h"
#include "desktop-storage-manager.h"

G_DEFINE_TYPE (GSignondDesktopExtension, gsignond_desktop_extension, GSIGNOND_TYPE_EXTENSION);

#define GSIGNOND_DESKTOP_EXTENSION_PRIV(obj) G_TYPE_INSTANCE_GET_PRIVATE ((obj), GSIGNOND_TYPE_DESKTOP_EXTENSION, GSignondDesktopExtensionPrivate)

struct _GSignondDesktopExtensionPrivate
{
    GSignondStorageManager *storage_manager;
    GSignondSecretStorage *secret_storage;
};

static void
_dispose (GObject *obj)
{
    GSignondDesktopExtension *self = GSIGNOND_DESKTOP_EXTENSION (obj);
    if (!self) return;

    GSignondDesktopExtensionPrivate *priv = self->priv;

    if (priv) {
        g_clear_object (&priv->secret_storage);
        g_clear_object (&priv->storage_manager);
    }

    G_OBJECT_CLASS (gsignond_desktop_extension_parent_class)->dispose (obj);
}

static const gchar *
_get_extension_name (GSignondExtension *self)
{
    (void) self;

    return "desktop";
}

static guint32
_get_extension_version (GSignondExtension *self)
{
    (void) self;

    return 0x01000000;
}

static GSignondSecretStorage *
_get_secret_storage (GSignondExtension *self, GSignondConfig *config)
{
    g_return_val_if_fail (self && GSIGNOND_IS_DESKTOP_EXTENSION(self), NULL);

    GSignondDesktopExtensionPrivate *priv = GSIGNOND_DESKTOP_EXTENSION(self)->priv;

    if (!priv->secret_storage) {
        priv->secret_storage =
            g_object_new (GSIGNOND_TYPE_DESKTOP_SECRET_STORAGE,
                          "config", config, NULL);
    }

    return priv->secret_storage;
}

static GSignondStorageManager *
_get_storage_manager (GSignondExtension *self, GSignondConfig *config)
{
    g_return_val_if_fail (self && GSIGNOND_IS_DESKTOP_EXTENSION(self), NULL);

    GSignondDesktopExtensionPrivate *priv = GSIGNOND_DESKTOP_EXTENSION(self)->priv;

    if (!priv->storage_manager) {
        priv->storage_manager =
            g_object_new (GSIGNOND_TYPE_DESKTOP_STORAGE_MANAGER,
                          "config", config, NULL);
    }
    return priv->storage_manager;
}

static void
gsignond_desktop_extension_class_init (GSignondDesktopExtensionClass *klass)
{
    GSignondExtensionClass *parent_class = GSIGNOND_EXTENSION_CLASS (klass);

    g_type_class_add_private (G_OBJECT_CLASS(klass), sizeof (GSignondDesktopExtensionPrivate));

    G_OBJECT_CLASS (klass)->dispose = _dispose;
    parent_class->get_extension_name = _get_extension_name;
    parent_class->get_extension_version = _get_extension_version;
    parent_class->get_storage_manager = _get_storage_manager;
    parent_class->get_secret_storage = _get_secret_storage;
}

static void
gsignond_desktop_extension_init (GSignondDesktopExtension *self)
{
    self->priv = GSIGNOND_DESKTOP_EXTENSION_PRIV (self);

    self->priv->storage_manager = NULL;
    self->priv->secret_storage = NULL;
}

static void
_on_object_dispose (gpointer data, GObject *object)
{
    if (data) *(GSignondDesktopExtension **)data = NULL;
}

GSignondExtension *
desktop_extension_init (void)
{
    static GSignondExtension *desktop_extension  = NULL;

    if (!desktop_extension) {
        desktop_extension = g_object_new (GSIGNOND_TYPE_DESKTOP_EXTENSION, NULL);

        g_object_weak_ref (G_OBJECT (desktop_extension), _on_object_dispose, &desktop_extension);
    }

    return desktop_extension;
}
