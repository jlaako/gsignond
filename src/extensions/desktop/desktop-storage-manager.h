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

#ifndef _GSIGNOND_DESKTOP_STORAGE_MANAGER_H_
#define _GSIGNOND_DESKTOP_STORAGE_MANAGER_H_

#include <glib-object.h>
#include <gsignond.h>

G_BEGIN_DECLS

#define GSIGNOND_TYPE_DESKTOP_STORAGE_MANAGER \
    (gsignond_desktop_storage_manager_get_type ())
#define GSIGNOND_DESKTOP_STORAGE_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSIGNOND_TYPE_DESKTOP_STORAGE_MANAGER, \
                                 GSignondDesktopStorageManager))
#define GSIGNOND_IS_DESKTOP_STORAGE_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSIGNOND_TYPE_DESKTOP_STORAGE_MANAGER))
#define GSIGNOND_DESKTOP_STORAGE_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GSIGNOND_TYPE_DESKTOP_STORAGE_MANAGER, \
                              GSignondDesktopStorageManagerClass))
#define GSIGNOND_IS_DESKTOP_STORAGE_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GSIGNOND_TYPE_DESKTOP_STORAGE_MANAGER))
#define GSIGNOND_DESKTOP_STORAGE_MANAGER_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GSIGNOND_TYPE_DESKTOP_STORAGE_MANAGER, \
                                GSignondDesktopStorageManagerClass))

typedef struct _GSignondDesktopStorageManager GSignondDesktopStorageManager;
typedef struct _GSignondDesktopStorageManagerClass GSignondDesktopStorageManagerClass;

struct _GSignondDesktopStorageManager
{
    GSignondStorageManager parent_instance;
};

struct _GSignondDesktopStorageManagerClass
{
    GSignondStorageManagerClass parent_class;
};

GType gsignond_desktop_storage_manager_get_type ();

G_END_DECLS

#endif  /* _GSIGNOND_DESKTOP_STORAGE_MANAGER_H_ */

