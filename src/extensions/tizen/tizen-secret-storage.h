/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of gsignond
 *
 * Copyright (C) 2013 Intel Corporation.
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

#ifndef _TIZEN_SECRET_STORAGE_H_
#define _TIZEN_SECRET_STORAGE_H_

#include <glib-object.h>
#include <gsignond.h>

G_BEGIN_DECLS

#define EXTENSION_TYPE_TIZEN_SECRET_STORAGE \
    (extension_tizen_secret_storage_get_type ())
#define EXTENSION_TIZEN_SECRET_STORAGE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), EXTENSION_TYPE_TIZEN_SECRET_STORAGE, \
                                 ExtensionTizenSecretStorage))
#define EXTENSION_IS_TIZEN_SECRET_STORAGE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EXTENSION_TYPE_TIZEN_SECRET_STORAGE))
#define EXTENSION_TIZEN_SECRET_STORAGE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), EXTENSION_TYPE_TIZEN_SECRET_STORAGE, \
                              ExtensionTizenSecretStorageClass))
#define EXTENSION_IS_TIZEN_SECRET_STORAGE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), EXTENSION_TYPE_TIZEN_SECRET_STORAGE))
#define EXTENSION_TIZEN_SECRET_STORAGE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), EXTENSION_TYPE_TIZEN_SECRET_STORAGE, \
                                ExtensionTizenSecretStorageClass))

typedef struct _ExtensionTizenSecretStorage
    ExtensionTizenSecretStorage;
typedef struct _ExtensionTizenSecretStorageClass
    ExtensionTizenSecretStorageClass;
typedef struct _ExtensionTizenSecretStoragePrivate
    ExtensionTizenSecretStoragePrivate;

struct _ExtensionTizenSecretStorage
{
    GSignondSecretStorage parent_instance;
    ExtensionTizenSecretStoragePrivate *priv;
};

struct _ExtensionTizenSecretStorageClass
{
    GSignondSecretStorageClass parent_class;
};

GType extension_tizen_secret_storage_get_type (void);

G_END_DECLS

#endif  /* _TIZEN_SECRET_STORAGE_H_ */

