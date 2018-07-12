private_headers = [
    'gsignond-disposable.h',
    'gsignond-identity-info.h',
    'gsignond-identity-info-internal.h',
    'gsignond-pipe-stream.h',
    'gsignond-plugin-enum-types.h',
    'gsignond-db-defines.h',
    'gsignond-db-error.h',
    'gsignond-db-secret-database.h',
    'gsignond-db-sql-database.h',
    'gsignond-db-sql-database-private.h'
]

glib_prefix = glib.get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')
docpath = join_paths(get_option('prefix'), get_option('datadir'), 'gtk-doc', 'html')

gnome.gtkdoc(meson.project_name(),
    main_xml: 'gsignond-docs.xml',
    src_dir: [
        join_paths(meson.source_root(), 'src', 'common'),
        join_paths(meson.build_root(), 'src', 'common'),
        join_paths(meson.source_root(), 'src', 'plugins', 'password'),
        join_paths(meson.build_root(), 'src', 'plugins', 'password'),
        join_paths(meson.source_root(), 'src', 'plugins', 'digest'),
        join_paths(meson.build_root(), 'src', 'plugins', 'digest')
    ],
    fixxref_args: [
        '--html-dir=@0@'.format(docpath),
        '--extra-dir=@0@'.format(join_paths(glib_docpath, 'glib')),
        '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gobject')),
        '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gio'))
    ],
    ignore_headers: private_headers,
    dependencies: gsignond_deps,
    scan_args: ['--rebuild-types'],
    install: true
)
