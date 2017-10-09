
gplugind_embedded_files = files(
    'gsignond-plugin-daemon.c',
    'gsignond-plugin-daemon.h',
    'gsignond-plugin-loader.c',
    'gsignond-plugin-loader.h'
)

gplugind_files = gplugind_embedded_files + files('main.c')

gplugind_dependencies = [glib, gio, gmodule, gsignond_common, gsignond_dbus]

executable('gsignond-plugind',
    gplugind_files,
    dependencies : gplugind_dependencies,
    install: true,
    install_dir: plugin_loaders_dir
)


# We need a fake static library to embed the gplugind function into the test binaries.
if enable_debug == true
    gplugind_dir = include_directories('.')

    gsignond_embedded_gplugind_library = static_library(
        'gsignond-embedded-gplugind',
        gplugind_embedded_files,
        dependencies : gplugind_dependencies
    )

    gsignond_embedded_gplugind = declare_dependency(
        include_directories: gplugind_dir,
        link_with: [gsignond_dbus_library, gsignond_embedded_gplugind_library],
        sources: gplugind_embedded_files,
        dependencies : gplugind_dependencies
    )
endif
