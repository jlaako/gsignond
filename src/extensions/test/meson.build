test_files = files(
    'test-storage-manager.h',
    'test-storage-manager.c',
    'test-secret-storage.h',
    'test-secret-storage.c',
    'test-extension.h',
    'test-extension.c',
    'test-access-control-manager.h',
    'test-access-control-manager.c'
)

test_plugin = shared_module('extension-test',
    test_files,
    dependencies : [glib, gio, gio_unix, gsignond_common],
    install: true,
    install_dir : extension_dir
)
