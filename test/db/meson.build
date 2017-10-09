
test_db = executable('dbtest', 'dbtest.c',
    dependencies: [glib, check, sqlite, gsignond_embedded_gplugind, gsignond_db],
    c_args: '-DGSIGNOND_TEST_DB_DIR="' + join_paths(meson.build_root(), 'test', 'tmp', 'gsignond') + '"'
)

test('test-db',
    test_db,
    env: environment_test
)
