project('gsignond', 'c')

gnome = import('gnome')
pkg = import('pkgconfig')
glib = dependency('glib-2.0', version : '>=2.34')
gio = dependency('gio-2.0')
gio_unix = dependency('gio-unix-2.0')
gmodule = dependency('gmodule-2.0')
sqlite = dependency('sqlite3')

version_major = '1'
version_minor = '0'
version_revision = '5'

version_compact = '@0@.@1@'.format (version_major, version_minor)
version_full = '@0@.@1@'.format (version_compact, version_revision)

gplugins_dir = join_paths(get_option('prefix'), get_option('libdir'), meson.project_name(), 'gplugins')
sysconf_dir = '/etc'
base_storage_dir = '/var/db'
extension_dir = join_paths(get_option('prefix'), get_option('libdir'), meson.project_name(), 'extensions')
plugin_loaders_dir = join_paths(get_option('prefix'), get_option('libdir'), meson.project_name(), 'pluginloaders')

add_project_arguments('-DGSIGNOND_PLUGINLOADERS_DIR="@0@"'.format(plugin_loaders_dir), language : 'c')
add_project_arguments('-DGSIGNOND_EXTENSIONS_DIR="@0@"'.format(extension_dir), language : 'c')
add_project_arguments('-DGSIGNOND_GPLUGINS_DIR="@0@"'.format(gplugins_dir), language : 'c')
add_project_arguments('-DGSIGNOND_SYSCONF_DIR="@0@"'.format(sysconf_dir), language : 'c')
add_project_arguments('-DBASE_STORAGE_DIR="@0@"'.format(base_storage_dir), language : 'c')
add_project_arguments('-D_GNU_SOURCE=1', language : 'c')

bus_type = get_option('bus_type')
if bus_type == 'p2p'
    add_project_arguments('-DUSE_P2P=1', language : 'c')
elif bus_type == 'system'
    add_project_arguments('-DGSIGNOND_BUS_TYPE=G_BUS_TYPE_SYSTEM', language : 'c')
else
    add_project_arguments('-DGSIGNOND_BUS_TYPE=G_BUS_TYPE_SESSION', language : 'c')
endif

enable_debug = get_option('enable_debug')
if enable_debug == true
    add_project_arguments('-DENABLE_DEBUG=1', language : 'c')
endif

root_dir = include_directories('.')
subdir('src')

enable_doc = get_option('enable_doc')
if enable_doc == true
    subdir('docs')
endif

if enable_debug == true
    check = dependency('check')
    subdir('test')
endif

