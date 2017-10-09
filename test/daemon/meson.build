
subdir('services')

test_daemon = executable('daemontest', 'daemon-test.c',
    dependencies: [glib, check, gsignond_embedded_gplugind, gsignond_dbus],
    c_args: '-DGSIGNOND_TEST_DBUS_SERVICE_DIR="' + join_paths(meson.current_build_dir(), 'services') + '"'
)

daemon_environment = environment_test
daemon_environment.set ('SSO_GPLUGINS_DIR', join_paths(meson.build_root(), 'src', 'plugins', 'ssotest'))
daemon_environment.set ('SSO_STORAGE_PATH', join_paths(meson.current_build_dir(), 'tmp'))

test('test-daemon',
    test_daemon,
    env: daemon_environment
)
