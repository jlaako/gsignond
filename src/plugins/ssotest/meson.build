ssotest_files = files(
    'gsignond-ssotest-plugin.c',
    'gsignond-ssotest-plugin.h'
)

ssotest_plugin = shared_module('ssotest',
    ssotest_files,
    dependencies : [glib, gio, gio_unix, gmodule, gsignond_common],
    install: true,
    install_dir : gplugins_dir
)

ssotest_plugin_dep = declare_dependency(
    sources: ssotest_files,
    dependencies : [glib, gio, gio_unix, gmodule, gsignond_common]
)
