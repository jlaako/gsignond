
daemon_files = files(
    'db/gsignond-db-credentials-database.c',
    'db/gsignond-db-credentials-database.h',
    'db/gsignond-db-metadata-database.c',
    'db/gsignond-db-metadata-database.h',
    'dbus/gsignond-dbus.h',
    'dbus/gsignond-dbus-auth-service-adapter.c',
    'dbus/gsignond-dbus-auth-service-adapter.h',
    'dbus/gsignond-dbus-auth-session-adapter.c',
    'dbus/gsignond-dbus-auth-session-adapter.h',
    'dbus/gsignond-dbus-identity-adapter.c',
    'dbus/gsignond-dbus-identity-adapter.h',
    'dbus/gsignond-dbus-server.c',
    'dbus/gsignond-dbus-server.h',
    'dbus/gsignond-dbus-signonui-adapter.c',
    'dbus/gsignond-dbus-signonui-adapter.h',
    'gsignond-auth-session.c',
    'gsignond-auth-session.h',
    'gsignond-daemon.c',
    'gsignond-daemon.h',
    'gsignond-identity.c',
    'gsignond-identity.h',
    'gsignond-signonui-proxy.c',
    'gsignond-signonui-proxy.h',
    'gsignond-types.h'
)

daemon_plugins_files = files(
    'plugins/gsignond-plugin-proxy.c',
    'plugins/gsignond-plugin-proxy.h',
    'plugins/gsignond-plugin-proxy-factory.c',
    'plugins/gsignond-plugin-proxy-factory.h',
    'plugins/gsignond-plugin-remote.c',
    'plugins/gsignond-plugin-remote.h',
    'plugins/gsignond-plugin-remote-private.h',
)

daemon_files += daemon_plugins_files

dbus_files = []
dbus_files += gnome.gdbus_codegen('gsignond-dbus-auth-service-gen',
    'dbus/interfaces/com.google.code.AccountsSSO.gSingleSignOn.AuthService.xml',
    interface_prefix: 'com.google.code.AccountsSSO.gSingleSignOn.',
    namespace: 'GSignondDbus'
)

dbus_files += gnome.gdbus_codegen('gsignond-dbus-auth-session-gen',
    'dbus/interfaces/com.google.code.AccountsSSO.gSingleSignOn.AuthSession.xml',
    interface_prefix: 'com.google.code.AccountsSSO.gSingleSignOn.',
    namespace: 'GSignondDbus'
)

dbus_files += gnome.gdbus_codegen('gsignond-dbus-identity-gen',
    'dbus/interfaces/com.google.code.AccountsSSO.gSingleSignOn.Identity.xml',
    interface_prefix: 'com.google.code.AccountsSSO.gSingleSignOn.',
    namespace: 'GSignondDbus'
)

dbus_files += gnome.gdbus_codegen('gsignond-dbus-remote-plugin-gen',
    'dbus/interfaces/com.google.code.AccountsSSO.gSingleSignOn.RemotePlugin.xml',
    interface_prefix: 'com.google.code.AccountsSSO.gSingleSignOn.',
    namespace: 'GSignondDbus'
)

daemon_files += gnome.mkenums('gsignond-identity-enum-types',
    sources: 'gsignond-identity.h',
    h_template: 'gsignond-identity-enum-types.h.template',
    c_template: 'gsignond-identity-enum-types.c.template',
    fhead: '#ifndef GSIGNOND_IDENTITY_ENUM_TYPES_H_\n#define GSIGNOND_IDENTITY_ENUM_TYPES_H_\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS',
    ftail: '\nG_END_DECLS\n\n#endif\n',
    identifier_prefix: 'GSignond'
)

daemon_dir = include_directories('.')

# We need a fake static library to share the DBus objects with the other plugins
gsignond_dbus_library = static_library('gsignond-dbus',
    dbus_files,
    include_directories: [daemon_dir, root_dir],
    dependencies : [glib, gio, gio_unix]
)

gsignond_dbus = declare_dependency(
    include_directories: [daemon_dir, root_dir],
    link_with: gsignond_dbus_library,
    sources: dbus_files,
    dependencies : [glib, gio, gio_unix]
)

#
if enable_debug == true
    gsignond_db_library = static_library('gsignond-db', [daemon_files], dependencies : [glib, gio, gio_unix, sqlite, gmodule, gsignond_common, gsignond_dbus])
    gsignond_db = declare_dependency(
        include_directories: daemon_dir,
        link_with: gsignond_db_library,
        sources: daemon_files,
        dependencies : [glib, gio, gio_unix, sqlite, gmodule, gsignond_common, gsignond_dbus]
    )
endif

executable('gsignond',
    daemon_files,
    'main.c',
    dependencies : [glib, gio, gio_unix, sqlite, gmodule, gsignond_common, gsignond_dbus],
    include_directories: root_dir,
    install: true
)
