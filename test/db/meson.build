
test_db = executable(
    'dbtest',
    'dbtest.c',
    dbus_identity_gen,
    dbus_auth_service_gen,
    dbus_auth_session_gen,
    dbus_remote_plugin_gen,
    daemon_files,
    dependencies: [check, gsignon_daemon],
    c_args: '-DGSIGNOND_TEST_DB_DIR="' + join_paths(meson.build_root(), 'test', 'tmp', 'gsignond') + '"'
)

test(
    'test-db',
    test_db,
    env: environment_test
)
