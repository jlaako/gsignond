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

#ifndef _GSIGNOND_DESKTOP_SECRET_STORAGE_H_
#define _GSIGNOND_DESKTOP_SECRET_STORAGE_H_

#include <glib-object.h>
#include <gsignond.h>
#include <libsecret/secret.h>

G_BEGIN_DECLS

#define GSIGNOND_TYPE_DESKTOP_SECRET_STORAGE \
    (gsignond_desktop_secret_storage_get_type ())
#define GSIGNOND_DESKTOP_SECRET_STORAGE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSIGNOND_TYPE_DESKTOP_SECRET_STORAGE, \
                                 GSignondDesktopSecretStorage))
#define GSIGNOND_IS_DESKTOP_SECRET_STORAGE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSIGNOND_TYPE_DESKTOP_SECRET_STORAGE))
#define GSIGNOND_DESKTOP_SECRET_STORAGE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GSIGNOND_TYPE_DESKTOP_SECRET_STORAGE, \
                              GSignondDesktopSecretStorageClass))
#define GSIGNOND_IS_DESKTOP_SECRET_STORAGE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GSIGNOND_TYPE_DESKTOP_SECRET_STORAGE))
#define GSIGNOND_DESKTOP_SECRET_STORAGE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GSIGNOND_TYPE_DESKTOP_SECRET_STORAGE, \
                                GSignondDesktopSecretStorageClass))

#define DESKTOP_SECRET_STORAGE_SCHEMA _desktop_secret_storage_get_schema ()

typedef struct _GSignondDesktopSecretStorage GSignondDesktopSecretStorage;
typedef struct _GSignondDesktopSecretStorageClass GSignondDesktopSecretStorageClass;
typedef struct _GSignondDesktopSecretStoragePrivate GSignondDesktopSecretStoragePrivate;

struct _GSignondDesktopSecretStorage
{
    GSignondSecretStorage parent_instance;
    GSignondDesktopSecretStoragePrivate *priv;
};

struct _GSignondDesktopSecretStorageClass
{
    GSignondSecretStorageClass parent_class;
};

GType gsignond_desktop_secret_storage_get_type ();

const SecretSchema * _desktop_secret_storage_get_schema (void) G_GNUC_CONST;

G_END_DECLS

#endif  /* _GSIGNOND_DESKTOP_SECRET_STORAGE_H_ */

