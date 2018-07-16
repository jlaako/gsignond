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
test_password_environment.set ('SSO_GPLUGINS_DIR', join_paths(meson.current_build_dir()))
test_password_environment.set ('SSO_STORAGE_PATH', join_paths(meson.current_build_dir(), 'tmp'))

# gplugind can't search in subdirs so we recreate some plugins here

passwordtest_plugin = shared_module('password',
    dependencies: password_plugin_dep)
ssotest_plugin = shared_module('ssotest',
    dependencies: ssotest_plugin_dep)

# Remote test
test_remoteplugin = executable(
    'remoteplugintest',
    'pluginremotetest.c',
    dependencies: [check, gsignon_daemon, gplugind]
)

test('test-remote-plugin',
    test_remoteplugin,
    env: test_password_environment,
    depends: [ passwordtest_plugin, ssotest_plugin ]
)

# Proxy test
test_proxyplugin = executable(
    'proxyplugintest',
    'pluginproxytest.c',
    dbus_remote_plugin_gen,
    dependencies: [check, gsignon_daemon]
)

test('test-proxy-plugin',
    test_proxyplugin,
    env: test_password_environment,
    depends: [ passwordtest_plugin, ssotest_plugin ]
)
