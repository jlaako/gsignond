# Digest plugin test
test_digest_files = files ([
    'digestplugintest.c'
])

test_digest_files += digest_files

test_digestplugin = executable(
    'digestplugintest',
    test_digest_files,
    include_directories: digest_dir,
    dependencies: [glib, check, sqlite, gsignond_embedded_gplugind, gsignond_dbus]
)

test('test-plugin-digest',
    test_digestplugin,
    env: environment_test
)

# Password plugin test
test_password_files = files ([
    'passwordplugintest.c'
])

test_password_files += password_files

test_passwordplugin = executable(
    'passwordplugintest',
    test_password_files,
    include_directories: password_dir,
    dependencies: [glib, check, sqlite, gsignond_embedded_gplugind, gsignond_dbus]
)

test('test-plugin-password',
    test_passwordplugin,
    env: environment_test
)

test_password_environment = environment_test
test_password_environment.set ('SSO_GPLUGINS_DIR', join_paths(meson.build_root(), 'src', 'plugins', 'password'))

# Remote test
test_remote_files = files([
    'pluginremotetest.c'
])

test_remoteplugin = executable(
    'remoteplugintest',
    test_remote_files + daemon_plugins_files + dbus_files,
    include_directories: daemon_dir,
    dependencies: [glib, check, sqlite, gsignond_common, gsignond_embedded_gplugind, gsignond_dbus]
)

test('test-remote-plugin',
    test_remoteplugin,
    env: test_password_environment
)

# Proxy test
test_proxy_files = files([
    'pluginproxytest.c'
])

test_proxyplugin = executable(
    'proxyplugintest',
    test_proxy_files + daemon_plugins_files + dbus_files,
    include_directories: [daemon_dir, root_dir],
    dependencies: [glib, check, sqlite, gsignond_common]
)

# We need gplugind to search in subdirs

#test('test-proxy-plugin',
#    test_proxyplugin,
#    env: test_password_environment
#)
