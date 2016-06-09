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

#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <mntent.h>

#include <glib.h>
#include <glib/gstdio.h>

#include <ecryptfs.h>

#include <trousers/tss.h>
#include <trousers/trousers.h>

#include "config.h"

#include "ostro-storage-manager.h"
#include "gsignond/gsignond-log.h"
#include "gsignond/gsignond-utils.h"

#define EXTENSION_OSTRO_STORAGE_MANAGER_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((obj), \
                                  EXTENSION_TYPE_OSTRO_STORAGE_MANAGER, \
                                  ExtensionOstroStorageManagerPrivate))

/* these are limited by ecryptfs */
#define KEY_BYTES 16
#define KEY_CIPHER "aes"

typedef struct __key_bundle
{
    uint8_t key[KEY_BYTES];
    uint8_t salt[ECRYPTFS_SALT_SIZE];
} _key_bundle_t;

struct _ExtensionOstroStorageManagerPrivate
{
    gchar *kdir;
    gchar *cdir;
    gchar ksig[ECRYPTFS_SIG_SIZE_HEX + 1];
};

enum
{
    PROP_0,
    N_PROPERTIES,
    PROP_CONFIG
};

/*static GParamSpec *properties[N_PROPERTIES] = { NULL, };*/

G_DEFINE_TYPE (ExtensionOstroStorageManager,
               extension_ostro_storage_manager,
               GSIGNOND_TYPE_STORAGE_MANAGER);

static void
_set_config (ExtensionOstroStorageManager *self, GSignondConfig *config)
{
    GSignondStorageManager *parent = GSIGNOND_STORAGE_MANAGER (self);
    g_assert (parent->config == NULL);
    g_assert (self->priv->cdir == NULL);
    parent->config = config;

    gchar *user_dir = g_strdup_printf ("gsignond.%s", g_get_user_name ());
    const gchar *storage_path = gsignond_config_get_string (
                                       parent->config,
                                       GSIGNOND_CONFIG_GENERAL_STORAGE_PATH);
    if (!storage_path)
        storage_path = BASE_STORAGE_DIR;
#   ifdef ENABLE_DEBUG
    const gchar *env_val = g_getenv("SSO_STORAGE_PATH");
    if (env_val)
        storage_path = env_val;
#   endif
    parent->location = g_build_filename (storage_path, user_dir, NULL);
    g_free (user_dir);
    /* store key in user dir */
    self->priv->kdir = g_strdup (parent->location);
    self->priv->cdir = g_strdup_printf ("%s.efs", parent->location);
    DBG ("location %s encryption point %s", parent->location, self->priv->cdir);
}

static void
_set_property (GObject *object, guint prop_id, const GValue *value,
               GParamSpec *pspec)
{
    ExtensionOstroStorageManager *self =
        EXTENSION_OSTRO_STORAGE_MANAGER (object);
    /*ExtensionOstroStorageManagerPrivate *priv = self->priv;*/

    switch (prop_id) {
        case PROP_CONFIG:
            _set_config (self, GSIGNOND_CONFIG (g_value_dup_object (value)));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    ExtensionOstroStorageManager *self =
        EXTENSION_OSTRO_STORAGE_MANAGER (object);
    /*ExtensionOstroStorageManagerPrivate *priv = self->priv;*/

    switch (prop_id) {
        case PROP_CONFIG:
            g_value_set_object (value,
                                GSIGNOND_STORAGE_MANAGER (self)->config);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
_dispose (GObject *object)
{
    G_OBJECT_CLASS (extension_ostro_storage_manager_parent_class)->dispose (object);
}

static void
_finalize (GObject *object)
{
    ExtensionOstroStorageManager *self =
        EXTENSION_OSTRO_STORAGE_MANAGER (object);
    ExtensionOstroStorageManagerPrivate *priv = self->priv;

    if (priv)
        memset(priv->ksig, 0x00, sizeof(priv->ksig));
    g_free (priv->cdir);
    g_free (priv->kdir);

    G_OBJECT_CLASS (extension_ostro_storage_manager_parent_class)->finalize (object);
}

static gboolean
_seal_key_bundle (void **skeyb, size_t *skeyb_size,
                  _key_bundle_t *kbundle)
{
	TSS_HCONTEXT hctx = 0;
	TSS_HTPM htpm = 0;
	TSS_HKEY hkey = 0;
	TSS_HPOLICY hpol = 0;
	TSS_HENCDATA hencdata = 0;
	TSS_UUID key_uuid = TSS_UUID_SRK;
	BYTE wks[] = TSS_WELL_KNOWN_SECRET;
    UINT32 datasize = 0;
	BYTE *databuf = NULL;
	TSS_RESULT res;
    gboolean retval = FALSE;

	res = Tspi_Context_Create (&hctx);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_Create(): %x", res);
		return FALSE;
	}
	res = Tspi_Context_Connect (hctx, NULL);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_Connect(): %x", res);
		goto ctx_exit;
	}

	res = Tspi_Context_GetTpmObject (hctx, &htpm);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_GetTpmObject(): %x", res);
		goto ctx_exit;
	}

	res = Tspi_Context_LoadKeyByUUID (hctx, TSS_PS_TYPE_SYSTEM,
		key_uuid, &hkey);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_LoadKeyByUUID(): %x", res);
		goto tpm_exit;
	}
	res = Tspi_GetPolicyObject (hkey, TSS_POLICY_USAGE, &hpol);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_GetPolicyObject(): %x", res);
		goto key_exit;
	}
	res = Tspi_Policy_SetSecret (hpol, TSS_SECRET_MODE_SHA1,
		sizeof (wks), wks);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Policy_SetSecret(): %x", res);
		goto pol_exit;
	}

	res = Tspi_Context_CreateObject (hctx, TSS_OBJECT_TYPE_ENCDATA,
		TSS_ENCDATA_SEAL, &hencdata);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_CreateObject(): %x", res);
		goto pol_exit;
	}
	res = Tspi_Data_Seal (hencdata, hkey,
                          sizeof(_key_bundle_t), (BYTE *) kbundle, 0);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Data_Seal(): %x", res);
		goto enc_exit;
	}

	res = Tspi_GetAttribData (hencdata, TSS_TSPATTRIB_ENCDATA_BLOB,
		TSS_TSPATTRIB_ENCDATABLOB_BLOB, &datasize, &databuf);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_GetAttribData(): %x", res);
		goto enc_exit;
	}
    *skeyb = g_malloc0 (datasize);
    memcpy (*skeyb, databuf, datasize);
    *skeyb_size = datasize;

    retval = TRUE;

enc_exit:
	Tspi_Context_Close (hencdata);
pol_exit:
	Tspi_Context_Close (hpol);
key_exit:
	Tspi_Context_Close (hkey);
tpm_exit:
	Tspi_Context_Close (htpm);
ctx_exit:
	res = Tspi_Context_FreeMemory (hctx, NULL);
	if (res != TSS_SUCCESS)
		DBG ("Tspi_Context_FreeMemory(): %x", res);
	res = Tspi_Context_Close (hctx);
	if (res != TSS_SUCCESS)
		DBG ("Tspi_Context_Close(): %x", res);

    return retval;
}

static gboolean
_unseal_key_bundle (_key_bundle_t *kbundle, void *skeyb, size_t skeyb_size)
{
	TSS_HCONTEXT hctx = 0;
	TSS_HTPM htpm = 0;
	TSS_HKEY hkey = 0;
	TSS_HPOLICY hpol = 0;
	TSS_HENCDATA hencdata = 0;
	TSS_UUID key_uuid = TSS_UUID_SRK;
	BYTE wks[] = TSS_WELL_KNOWN_SECRET;
    UINT32 datasize = 0;
	BYTE *databuf = NULL;
	TSS_RESULT res;
    gboolean retval = FALSE;

	res = Tspi_Context_Create (&hctx);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_Create(): %x", res);
		return FALSE;
	}
	res = Tspi_Context_Connect (hctx, NULL);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_Connect(): %x", res);
		goto ctx_exit;
	}

	res = Tspi_Context_GetTpmObject (hctx, &htpm);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_GetTpmObject(): %x", res);
		goto ctx_exit;
	}

	res = Tspi_Context_LoadKeyByUUID (hctx, TSS_PS_TYPE_SYSTEM,
		key_uuid, &hkey);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_LoadKeyByUUID(): %x", res);
		goto tpm_exit;
	}
	res = Tspi_GetPolicyObject (hkey, TSS_POLICY_USAGE, &hpol);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_GetPolicyObject(): %x", res);
		goto key_exit;
	}
	res = Tspi_Policy_SetSecret (hpol, TSS_SECRET_MODE_SHA1,
		sizeof (wks), wks);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Policy_SetSecret(): %x", res);
		goto pol_exit;
	}

	res = Tspi_Context_CreateObject (hctx, TSS_OBJECT_TYPE_ENCDATA,
		TSS_ENCDATA_SEAL, &hencdata);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Context_CreateObject(): %x", res);
		goto pol_exit;
	}
    res = Tspi_SetAttribData (hencdata, TSS_TSPATTRIB_ENCDATA_BLOB,
		TSS_TSPATTRIB_ENCDATABLOB_BLOB, skeyb_size, skeyb);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_SetAttribData(): %x", res);
		goto enc_exit;
	}
	datasize = 0;
	databuf = NULL;
	res = Tspi_Data_Unseal (hencdata, hkey, &datasize, &databuf);
	if (res != TSS_SUCCESS) {
		DBG ("Tspi_Data_Unseal(): %x", res);
		goto enc_exit;
	}

    if (datasize != sizeof (_key_bundle_t)) {
        DBG ("size of unsealed data doesn't match with key bundle size");
        goto enc_exit;
    }
    memcpy (kbundle, databuf, sizeof (_key_bundle_t));

    retval = TRUE;

enc_exit:
	Tspi_Context_Close (hencdata);
pol_exit:
	Tspi_Context_Close (hpol);
key_exit:
	Tspi_Context_Close (hkey);
tpm_exit:
	Tspi_Context_Close (htpm);
ctx_exit:
	res = Tspi_Context_FreeMemory (hctx, NULL);
	if (res != TSS_SUCCESS)
		DBG ("Tspi_Context_FreeMemory(): %x", res);
	res = Tspi_Context_Close (hctx);
	if (res != TSS_SUCCESS)
		DBG ("Tspi_Context_Close(): %x", res);

    return retval;
}

static gboolean
_initialize_storage (GSignondStorageManager *parent)
{
    ExtensionOstroStorageManager *self =
        EXTENSION_OSTRO_STORAGE_MANAGER (parent);
    ExtensionOstroStorageManagerPrivate *priv = self->priv;

    g_return_val_if_fail (parent->location, FALSE);
    g_return_val_if_fail (priv->cdir, FALSE);

    if (g_access (parent->location, R_OK) == 0 &&
        g_access (priv->cdir, R_OK) == 0)
        return TRUE;

    /* generate random key bundle */
    DBG ("generating random key bundle");
    int rndfd = open ("/dev/random", O_RDONLY);
    if (rndfd < 0) {
        WARN ("open() of /dev/random failed");
        return FALSE;
    }
    _key_bundle_t keyb;
    if (read (rndfd, &keyb, sizeof (keyb)) < (ssize_t) sizeof (keyb)) {
        WARN ("read() from /dev/random failed");
        close (rndfd);
        return FALSE;
    }
    close (rndfd);

    /* seal the key bundle with TPM */
    DBG ("sealing key bundle with TPM");
    void *skeyb = NULL;
    size_t skeyb_size = 0;
    if (!_seal_key_bundle (&skeyb, &skeyb_size, &keyb)) {
        WARN ("failed to create a sealed key bundle");
        return FALSE;
    }
    memset (&keyb, 0x00, sizeof (keyb));

    /* store sealed key bundle */
    DBG ("storing sealed key bundle");
    gchar *skbfile = g_build_filename (priv->kdir, "gsignond-kb.bin", NULL);
    int skbfd = open (skbfile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    g_free (skbfile);
    if (skbfd < 0) {
        WARN ("open(\"%s\") failed", skbfile);
        g_free (skeyb);
        return FALSE;
    }
    ssize_t skbwsize = write (skbfd, skeyb, skeyb_size);
    close (skbfd);
    g_free (skeyb);
    if (skbwsize < (ssize_t) skeyb_size) {
        WARN ("write() of the sealed key bundle failed");
        return FALSE;
    }

    gboolean res = FALSE;

    uid_t uid = getuid ();
    if (seteuid (0))
        WARN ("seteuid() failed");

    DBG ("create mount point %s", parent->location);
    if (g_mkdir_with_parents (parent->location, S_IRWXU | S_IRWXG))
        goto init_exit;

    DBG ("create storage point %s", priv->cdir);
    if (g_mkdir_with_parents (priv->cdir, S_IRWXU | S_IRWXG))
        goto init_exit;

    if (chown (parent->location, 0, getegid ()))
        WARN ("chown() failed");
    if (chmod (parent->location, S_IRWXU | S_IRWXG))
        WARN ("chmod() failed");
    if (chown (priv->cdir, 0, getegid ()))
        WARN ("chown() failed");
    if (chmod (priv->cdir, S_IRWXU | S_IRWXG))
        WARN ("chmod() failed");
    res = TRUE;

init_exit:
    if (seteuid (uid))
        WARN ("seteuid() failed");

    return res;
}

static gboolean
_storage_is_initialized (GSignondStorageManager *parent)
{
    ExtensionOstroStorageManager *self =
        EXTENSION_OSTRO_STORAGE_MANAGER (parent);
    ExtensionOstroStorageManagerPrivate *priv = self->priv;

    g_return_val_if_fail (priv->cdir, FALSE);

    if (g_access (priv->cdir, 0) || g_access (parent->location, 0))
        return FALSE;

    return TRUE;
}

static void
_clear_string (gchar *string)
{
    while (*string) {
        *string = '\0';
        string++;
    }
}

static const gchar *
_mount_filesystem (GSignondStorageManager *parent)
{
    gchar *retval = NULL;
    ExtensionOstroStorageManager *self =
        EXTENSION_OSTRO_STORAGE_MANAGER (parent);
    ExtensionOstroStorageManagerPrivate *priv = self->priv;
    /*gchar fekey[ECRYPTFS_MAX_PASSPHRASE_BYTES + 1];
    gchar fesalt[ECRYPTFS_SALT_SIZE];*/

    DBG ("load sealed key bundle");
    gchar *skbfile = g_build_filename (priv->kdir, "gsignond-kb.bin", NULL);
    int skbfd = open (skbfile, O_RDONLY);
    g_free (skbfile);
    if (skbfd < 0) {
        DBG ("failed to open sealed key bundle file");
        return NULL;
    }
    struct stat skbstat;
    if (fstat (skbfd, &skbstat)) {
        DBG ("fstat() of sealed key bundle file failed");
        close (skbfd);
        return NULL;
    }
    size_t skeyb_size = skbstat.st_size;
    void *skeyb = g_malloc0 (skeyb_size);
    ssize_t skbrsize = read (skbfd, skeyb, skeyb_size);
    close (skbfd);
    if (skbrsize < (ssize_t) skeyb_size) {
        DBG ("failed to read sealed key bundle");
        g_free (skeyb);
        return NULL;
    }
    DBG ("unseal key bundle");
    _key_bundle_t keyb;
    if (!_unseal_key_bundle (&keyb, skeyb, skeyb_size)) {
        DBG ("failed to unseal key bundle");
        g_free (skeyb);
        return NULL;
    }
    g_free (skeyb);

    /* ecryptfs expects string key and binary salt, thus we base64 encode the
     * binary key data for "passphrase" */
    gchar *passphrase = g_base64_encode (keyb.key, sizeof (keyb.key));
    memset (&keyb, 0x00, sizeof (keyb));
    DBG ("add passphrase to kernel keyring");
    int ecryptfs_err =
        ecryptfs_add_passphrase_key_to_keyring (priv->ksig,
                                                passphrase,
                                                (char *) keyb.salt);
    _clear_string (passphrase);
    g_free (passphrase);
    if (ecryptfs_err < 0)
        return NULL;

    gchar *mntopts = g_strdup_printf (
                                      "ecryptfs_check_dev_ruid" \
                                      ",ecryptfs_cipher=%s" \
                                      ",ecryptfs_key_bytes=%d" \
                                      ",ecryptfs_unlink_sigs" \
                                      ",ecryptfs_sig=%s",
                                      KEY_CIPHER, KEY_BYTES,
                                      priv->ksig);
    DBG ("mount options: %s", mntopts);
    uid_t uid = getuid ();
    if (seteuid (0))
        WARN ("seteuid() failed");
    DBG ("perform mount %s -> %s", priv->cdir, parent->location);
    if (mount (priv->cdir, parent->location,
               "ecryptfs", MS_NOSUID | MS_NODEV, mntopts)) {
        INFO ("mount failed %d: %s", errno, strerror(errno));
        goto _mount_exit;
    }

    DBG ("mount succeeded at %s", parent->location);
    retval = parent->location;

_mount_exit:
    g_free (mntopts);
    if (seteuid (uid))
        WARN ("seteuid() failed");

    return retval;
}

static gboolean
_unmount_filesystem (GSignondStorageManager *parent)
{
    g_return_val_if_fail (parent != NULL, FALSE);

    uid_t uid = getuid ();
    if (seteuid (0))
        WARN ("seteuid() failed");
    umount (parent->location);
    if (seteuid (uid))
        WARN ("seteuid() failed");

    return TRUE;
}

static gboolean
_filesystem_is_mounted (GSignondStorageManager *parent)
{
    gboolean retval = FALSE;
    FILE *mntf = setmntent("/proc/mounts", "r");
    g_return_val_if_fail (mntf != NULL, FALSE);
    
    struct mntent *me;
    while ((me = getmntent(mntf))) {
        if (g_strcmp0 (parent->location, me->mnt_dir) == 0) {
            retval = TRUE;
            break;
        }
    }

    endmntent(mntf);

    return retval;
}

static gboolean
_delete_storage (GSignondStorageManager *parent)
{
    ExtensionOstroStorageManager *self =
        EXTENSION_OSTRO_STORAGE_MANAGER (parent);
    ExtensionOstroStorageManagerPrivate *priv = self->priv;

    g_return_val_if_fail (priv->cdir, FALSE);
    g_return_val_if_fail (!_filesystem_is_mounted(parent), FALSE);

    return (gsignond_wipe_directory (priv->cdir) &&
            gsignond_wipe_directory (parent->location));
}

static void
extension_ostro_storage_manager_class_init (
                                      ExtensionOstroStorageManagerClass *klass)
{
    GObjectClass *base = G_OBJECT_CLASS (klass);

    base->set_property = _set_property;
    base->get_property = _get_property;
    base->dispose = _dispose;
    base->finalize = _finalize;

    /*g_object_class_install_properties (base, N_PROPERTIES, properties);*/
    g_object_class_override_property (base, PROP_CONFIG, "config");

    g_type_class_add_private (klass,
                              sizeof(ExtensionOstroStorageManagerPrivate));

    GSignondStorageManagerClass *parent_class =
        GSIGNOND_STORAGE_MANAGER_CLASS (klass);
    parent_class->initialize_storage = _initialize_storage;
    parent_class->delete_storage = _delete_storage;
    parent_class->storage_is_initialized = _storage_is_initialized;
    parent_class->mount_filesystem = _mount_filesystem;
    parent_class->unmount_filesystem = _unmount_filesystem;
    parent_class->filesystem_is_mounted = _filesystem_is_mounted;
}

static void
extension_ostro_storage_manager_init (ExtensionOstroStorageManager *self)
{
    ExtensionOstroStorageManagerPrivate *priv =
        EXTENSION_OSTRO_STORAGE_MANAGER_GET_PRIVATE (self);
    self->priv = priv;
}

