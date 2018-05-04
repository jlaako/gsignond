project('gsignond', 'c', version: '1.0.6')

gnome = import('gnome')
pkg = import('pkgconfig')
glib = dependency('glib-2.0', version : '>=2.34')
gio = dependency('gio-2.0')
gio_unix = dependency('gio-unix-2.0')
gmodule = dependency('gmodule-2.0')
sqlite = dependency('sqlite3')

version_part = meson.project_version ().split('.')
version_major = version_part.get(0)
version_minor = version_part.get(1)
version_revision = version_part.get(2)

version_compact = '@0@.@1@'.format (version_major, version_minor)

keychain = '"@0@"'.format (get_option('keychain'))
extension = get_option('extension')

gplugins_dir = join_paths(get_option('prefix'), get_option('libdir'), meson.project_name(), 'gplugins')
sysconf_dir = '/etc'
base_storage_dir = get_option('storagedir')
extension_dir = join_paths(get_option('prefix'), get_option('libdir'), meson.project_name(), 'extensions')
plugin_loaders_dir = join_paths(get_option('prefix'), get_option('libdir'), meson.project_name(), 'pluginloaders')

add_project_arguments('-DGSIGNOND_PLUGINLOADERS_DIR="@0@"'.format(plugin_loaders_dir), language : 'c')
add_project_arguments('-DGSIGNOND_EXTENSIONS_DIR="@0@"'.format(extension_dir), language : 'c')
add_project_arguments('-DGSIGNOND_GPLUGINS_DIR="@0@"'.format(gplugins_dir), language : 'c')
add_project_arguments('-DGSIGNOND_SYSCONF_DIR="@0@"'.format(sysconf_dir), language : 'c')
add_project_arguments('-DBASE_STORAGE_DIR="@0@"'.format(base_storage_dir), language : 'c')
add_project_arguments('-DKEYCHAIN_SYSCTX="@0@"'.format(keychain), language : 'c')
add_project_arguments('-D_GNU_SOURCE=1', language : 'c')

bus_type = get_option('bus_type')
if bus_type == 'p2p'
    add_project_arguments('-DUSE_P2P=1', language : 'c')
elif bus_type == 'system'
    add_project_arguments('-DGSIGNOND_BUS_TYPE=G_BUS_TYPE_SYSTEM', language : 'c')
else
    add_project_arguments('-DGSIGNOND_BUS_TYPE=G_BUS_TYPE_SESSION', language : 'c')
endif

enable_debug = get_option('debug')
enable_sql_log = get_option('sql_log')
if enable_debug == true
    add_project_arguments('-DENABLE_DEBUG=1', language : 'c')

    if enable_sql_log == true
        add_project_arguments('-DENABLE_SQL_LOG=1', language : 'c')
    endif
elif enable_sql_log == true
    error('To enable SQL logs, you have to set -Ddebug=true')
endif

add_project_arguments(
    '-Wredundant-decls',
    '-Wuninitialized',
    '-Wformat-nonliteral',
    '-Werror=strict-prototypes',
    '-Werror=incompatible-pointer-types',
    '-Werror=format-security',
    '-Werror=format=2',
    '-Werror=shadow',
    language : 'c'
)

subdir('dbus')
subdir('src')

enable_doc = get_option('documentation')
if enable_doc == true
    subdir('docs')
endif

if enable_debug == true
    check = dependency('check')
    subdir('test')
endif

conf_data = configuration_data()
conf_data.set('EXTENSION', 'Extension = @0@'.format(extension))
conf_data.set('BASE_STORAGE_DIR', 'StoragePath = @0@'.format(base_storage_dir))
conf_data.set('KEYCHAIN_SYSCTX', 'KeychainSystemContext = @0@'.format(keychain))
# Disable default settings
if extension == 'default'
    conf_data.set('EXTENSION', '#@0@'.format(conf_data.get('EXTENSION')))
endif
if base_storage_dir == '/var/db'
    conf_data.set('BASE_STORAGE_DIR', '#@0@'.format(conf_data.get('BASE_STORAGE_DIR')))
endif
if keychain == '""'
    conf_data.set('KEYCHAIN_SYSCTX', '#@0@'.format(conf_data.get('KEYCHAIN_SYSCTX')))
endif
configure_file(
    input: 'gsignond.conf.in',
    configuration: conf_data,
    install_dir: sysconf_dir,
    output: 'gsignond.conf'
)
