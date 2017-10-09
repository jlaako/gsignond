password_files = files(
    'gsignond-password-plugin.c',
    'gsignond-password-plugin.h'
)

password_dir = include_directories('.')

password_plugin = shared_module('password',
    password_files,
    dependencies : [glib, gio, gio_unix, gmodule, gsignond_common],
    install: true,
    install_dir : gplugins_dir)
