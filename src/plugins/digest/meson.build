digest_files = files([
    'gsignond-digest-plugin.c',
    'gsignond-digest-plugin.h'
])

digest_dir = include_directories('.')

digest_plugin = shared_module('digest',
    digest_files,
    dependencies : [glib, gio, gio_unix, gmodule, gsignond_common],
    install: true,
    install_dir : gplugins_dir)
