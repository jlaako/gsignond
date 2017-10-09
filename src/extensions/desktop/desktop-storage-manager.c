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

#include <glib.h>
#include <gio/gio.h>
#include <gsignond.h>
#include <unistd.h>
#include <sys/stat.h>

#include "desktop-storage-manager.h"

G_DEFINE_TYPE (GSignondDesktopStorageManager,
               gsignond_desktop_storage_manager,
               GSIGNOND_TYPE_STORAGE_MANAGER);

static gboolean
_initialize_storage (GSignondStorageManager *self)
{
    GSignondStorageManager *parent = GSIGNOND_STORAGE_MANAGER (self);
    return g_mkdir_with_parents (parent->location, S_IRWXU|S_IRWXG) == 0;
}

static gboolean
_delete_storage (GSignondStorageManager *self)
{
    GSignondStorageManager *parent = GSIGNOND_STORAGE_MANAGER (self);
    GFile *file = g_file_new_for_path (parent->location);
    gboolean result = FALSE;
    result = g_file_delete (file, NULL, NULL);
    g_object_unref (file);
    return result;
}

static gboolean
_storage_is_initialized (GSignondStorageManager *self)
{
    GSignondStorageManager *parent = GSIGNOND_STORAGE_MANAGER (self);
    return access (parent->location, S_IRWXU|S_IRWXG) == 0;
}

static const gchar *
_mount_filesystem (GSignondStorageManager *self)
{
    GSignondStorageManager *parent = GSIGNOND_STORAGE_MANAGER (self);
    return parent->location;
}

static gboolean
_unmount_filesystem (GSignondStorageManager *self)
{
    return TRUE;
}

static gboolean
_filesystem_is_mounted (GSignondStorageManager *self)
{
    return _storage_is_initialized (self);
}

static GObject *
_constructor (GType type, guint n_construct_properties, GObjectConstructParam *construct_properties) {
    GObject * obj;
    GObjectClass * parent_class;
    GSignondStorageManager *parent;
    parent_class = G_OBJECT_CLASS (gsignond_desktop_storage_manager_parent_class);
    obj = parent_class->constructor (type, n_construct_properties, construct_properties);
    parent = GSIGNOND_STORAGE_MANAGER (obj);
    const gchar *storage_path = g_get_user_config_dir ();
#   ifdef ENABLE_DEBUG
    const gchar *env_val = g_getenv("SSO_STORAGE_PATH");
    if (env_val) {
        storage_path = env_val;
    }
#   endif
    parent->location = g_build_filename (storage_path, "gsignond", NULL);
    return obj;
}

static void
gsignond_desktop_storage_manager_class_init (
                                      GSignondDesktopStorageManagerClass *klass)
{
    GSignondStorageManagerClass *parent_class = GSIGNOND_STORAGE_MANAGER_CLASS (klass);

    parent_class->initialize_storage = _initialize_storage;
    parent_class->delete_storage = _delete_storage;
    parent_class->storage_is_initialized = _storage_is_initialized;
    parent_class->mount_filesystem = _mount_filesystem;
    parent_class->unmount_filesystem = _unmount_filesystem;
    parent_class->filesystem_is_mounted = _filesystem_is_mounted;
    G_OBJECT_CLASS (klass)->constructor = _constructor;
}

static void
gsignond_desktop_storage_manager_init (GSignondDesktopStorageManager *self)
{
}


