
internal_files = files([
    'gsignond-identity-info.h',
    'gsignond-identity-info-internal.h',
    'gsignond-identity-info.c',
    'gsignond-pipe-stream.h',
    'gsignond-pipe-stream.c',
    'gsignond-disposable.h',
    'gsignond-disposable.c',
    'db/gsignond-db-defines.h',
    'db/gsignond-db-error.c',
    'db/gsignond-db-error.h',
    'db/gsignond-db-secret-database.c',
    'db/gsignond-db-secret-database.h',
    'db/gsignond-db-sql-database.c',
    'db/gsignond-db-sql-database.h',
    'db/gsignond-db-sql-database-private.h'
])

common_files = files([
    'gsignond-access-control-manager.c',
    'gsignond-credentials.c',
    'gsignond-config.c',
    'gsignond-dictionary.c',
    'gsignond-error.c',
    'gsignond-extension-interface.c',
    'gsignond-plugin-interface.c',
    'gsignond-security-context.c',
    'gsignond-session-data.c',
    'gsignond-signonui-data.c',
    'gsignond-storage-manager.c',
    'gsignond-utils.c',
    'db/gsignond-secret-storage.c'
])

common_headers = files([
    'gsignond.h',
    'gsignond-access-control-manager.h',
    'gsignond-config.h',
    'gsignond-config-dbus.h',
    'gsignond-config-general.h',
    'gsignond-credentials.h',
    'gsignond-dictionary.h',
    'gsignond-error.h',
    'gsignond-extension-interface.h',
    'gsignond-log.h',
    'gsignond-plugin-interface.h',
    'db/gsignond-secret-storage.h',
    'gsignond-security-context.h',
    'gsignond-session-data.h',
    'gsignond-signonui-data.h',
    'gsignond-storage-manager.h',
    'gsignond-utils.h'
])

common_files += gnome.mkenums('gsignond-plugin-enum-types',
    sources: 'gsignond-plugin-interface.h',
    h_template: 'gsignond-plugin-enum-types.h.template',
    c_template: 'gsignond-plugin-enum-types.c.template',
    fhead: '#ifndef GSIGNOND_PLUGIN_ENUM_TYPES_H_\n#define GSIGNOND_PLUGIN_ENUM_TYPES_H_\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS',
    ftail: '\nG_END_DECLS\n\n#endif\n',
    identifier_prefix: 'GSignond')

gsignond_library = shared_library('gsignond-common',
    common_files, common_headers, internal_files,
    dependencies : [glib, gio, gio_unix, sqlite],
    soversion: version_major,
    version: version_full,
    install: true)

install_headers(common_headers, subdir : meson.project_name())

pkg.generate(
    name: meson.project_name(),
    description: 'Single-sign-on daemon and libraries',
    filebase: meson.project_name(),
    url: 'https://gitlab.com/accounts-sso/gsignond',
    libraries: gsignond_library,
    subdirs: meson.project_name(),
    requires: ['glib-2.0', 'gio-2.0', 'gio-unix-2.0', 'gmodule-2.0', 'sqlite3'],
    version: version_full,
    variables: [
        'gpluginsdir=' + gplugins_dir,
        'pluginloadersdir=' + plugin_loaders_dir,
        'extensionsdir='+ extension_dir
    ]
)

gsignond_common = declare_dependency(
    include_directories: include_directories('.'),
    link_with: gsignond_library,
    sources: common_headers,
    dependencies : [glib, gio, gio_unix, sqlite]
)

gir_files = gnome.generate_gir(gsignond_library,
    namespace: 'GSignond',
    nsversion: version_compact,
    sources: [common_files, common_headers],
    includes: ['GObject-2.0', 'Gio-2.0'],
    link_with: gsignond_library,
    identifier_prefix: 'GSignond',
    symbol_prefix: 'gsignond',
    extra_args: ['--c-include="gsignond.h"', '--warn-all'],
    install: true
)

gnome.generate_vapi(meson.project_name(),
    sources: gir_files[0],
    packages: ['gio-2.0', 'gobject-2.0', 'gmodule-2.0'],
    install: true
)
