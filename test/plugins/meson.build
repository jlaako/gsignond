# Digest plugin test
test_digestplugin = executable(
    'digestplugintest',
    'digestplugintest.c',
    digest_files,
    include_directories: digest_dir,
    dependencies: [glib, check, sqlite, gsignond_common]
)

test('test-plugin-digest',
    test_digestplugin,
    env: environment_test
)

# Password plugin test
test_passwordplugin = executable(
    'passwordplugintest',
    'passwordplugintest.c',
    password_files,
    include_directories: password_dir,
    dependencies: [glib, check, sqlite, gsignond_common]
)

test('test-plugin-password',
    test_passwordplugin,
    env: environment_test
)

test_password_environment = environment_test
test_password_environment.set ('SSO_GPLUGINS_DIR', join_paths(meson.build_root(), 'src', 'plugins', 'password'))
test_password_environment.set ('SSO_STORAGE_PATH', join_paths(meson.current_build_dir(), 'tmp'))

# Remote test
test_remoteplugin = executable(
    'remoteplugintest',
    'pluginremotetest.c',
    dependencies: [check, gsignon_daemon, gplugind]
)

test('test-remote-plugin',
    test_remoteplugin,
    env: test_password_environment
)

# Proxy test
test_proxyplugin = executable(
    'proxyplugintest',
    'pluginproxytest.c',
    dbus_remote_plugin_gen,
    dependencies: [check, gsignon_daemon]
)

# We need gplugind to search in subdirs

#test('test-proxy-plugin',
#    test_proxyplugin,
#    env: test_password_environment
#)
