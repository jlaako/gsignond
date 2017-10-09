
libecryptfs = dependency('libecryptfs')
libsmack = dependency('libsmack')
tspi = meson.get_compiler('c').find_library('tspi')

ostro_files = files([
    'ostro-access-control-manager.h',
    'ostro-access-control-manager.c',
    'ostro-extension.h',
    'ostro-extension.c',
    'ostro-secret-storage.h',
    'ostro-secret-storage.c',
    'ostro-storage-manager.h',
    'ostro-storage-manager.c'
])

ostro_plugin = shared_module('extension-ostro',
    ostro_files,
    dependencies : [glib, gio, gio_unix, libecryptfs, libsmack, tspi, gsignond_common],
    install: true,
    install_dir : extension_dir
)
