
libecryptfs = dependency('libecryptfs')
libsmack = dependency('libsmack')

tizen_files = files([
    'tizen-access-control-manager.h',
    'tizen-access-control-manager.c',
    'tizen-extension.h',
    'tizen-extension.c',
    'tizen-secret-storage.h',
    'tizen-secret-storage.c',
    'tizen-storage-manager.h',
    'tizen-storage-manager.c'
])

tizen_plugin = shared_module('extension-tizen',
    tizen_files,
    dependencies : [glib, gio, gio_unix, libecryptfs, libsmack, gsignond_common],
    install: true,
    install_dir : extension_dir
)
