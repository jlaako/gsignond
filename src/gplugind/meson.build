
gplugind_files = files(
    'gsignond-plugin-daemon.c',
    'gsignond-plugin-daemon.h',
    'gsignond-plugin-loader.c',
    'gsignond-plugin-loader.h'
)

gplugind_files += dbus_remote_plugin_gen

gplugind_dependencies = [glib, gio, gmodule, gsignond_common, gsignond_dbus]

gplugind = declare_dependency(
    sources: gplugind_files,
    dependencies: gplugind_dependencies,
    include_directories: include_directories('.')
)

executable('gsignond-plugind',
    'main.c',
    gplugind_files,
    dependencies : gplugind_dependencies,
    install: true,
    install_dir: plugin_loaders_dir
)
