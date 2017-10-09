
libsecret = dependency('libsecret-1')

desktop_files = files(
    'desktop-extension.h',
    'desktop-extension.c',
    'desktop-secret-storage.h',
    'desktop-secret-storage.c',
    'desktop-storage-manager.h',
    'desktop-storage-manager.c'
)

desktop_plugin = shared_module('extension-desktop',
    desktop_files,
    dependencies : [glib, gio, gio_unix, libsecret, gsignond_common],
    install: true,
    install_dir : extension_dir
)
