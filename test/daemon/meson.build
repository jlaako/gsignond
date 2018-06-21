
subdir('services')

test_daemon = executable(
    'daemontest',
    'daemon-test.c',
    dbus_identity_gen,
    dbus_auth_service_gen,
    dbus_auth_session_gen,
    dependencies: [glib, check, gsignond_common, gsignond_dbus],
    c_args: '-DGSIGNOND_TEST_DBUS_SERVICE_DIR="' + join_paths(meson.current_build_dir(), 'services') + '"'
)

conf_test_data = configuration_data()
conf_test_data.set('EXTENSION', 'Extension = test')
conf_test_data.set('BASE_STORAGE_DIR', 'StoragePath = @0@'.format(base_storage_dir))
conf_test_data.set('KEYCHAIN_SYSCTX', 'KeychainSystemContext = @0@'.format(keychain))
configure_file(
    input: join_paths(meson.source_root(), 'gsignond.conf.in'),
    configuration: conf_test_data,
    output: 'gsignond.conf'
)

daemon_environment = environment_test
daemon_environment.set ('SSO_GPLUGINS_DIR', join_paths(meson.build_root(), 'src', 'plugins', 'ssotest'))
daemon_environment.set ('SSO_STORAGE_PATH', join_paths(meson.current_build_dir(), 'tmp'))

test('test-daemon',
    test_daemon,
    env: daemon_environment
)
