
daemon_files = files(
    'db/gsignond-db-credentials-database.c',
    'db/gsignond-db-credentials-database.h',
    'db/gsignond-db-metadata-database.c',
    'db/gsignond-db-metadata-database.h',
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

daemon_files += gnome.mkenums('gsignond-identity-enum-types',
    sources: 'gsignond-identity.h',
    h_template: 'gsignond-identity-enum-types.h.template',
    c_template: 'gsignond-identity-enum-types.c.template',
    fhead: '#ifndef GSIGNOND_IDENTITY_ENUM_TYPES_H_\n#define GSIGNOND_IDENTITY_ENUM_TYPES_H_\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS',
    ftail: '\nG_END_DECLS\n\n#endif\n',
    identifier_prefix: 'GSignond'
)

daemon_dependencies = [glib, gio, gio_unix, sqlite, gmodule, gsignond_common, gsignond_dbus]

gsignon_daemon = declare_dependency(
    sources: daemon_plugins_files,
    dependencies: daemon_dependencies,
    include_directories: include_directories('.')
)

executable('gsignond',
    daemon_files,
    dbus_auth_service_gen,
    dbus_auth_session_gen,
    dbus_identity_gen,
    dbus_remote_plugin_gen,
    'main.c',
    dependencies : daemon_dependencies,
    install: true
)
