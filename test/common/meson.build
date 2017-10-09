
test_common = executable('commontest', 'commontest.c',
    dependencies: [glib, check, gsignond_embedded_gplugind],
)

common_environment = environment_test
common_environment.set ('SSO_GPLUGINS_DIR', join_paths(meson.build_root(), 'src', 'plugins', 'password'))
common_environment.set ('SSO_STORAGE_PATH', join_paths(meson.current_build_dir(), 'tmp'))
test('test-common',
    test_common,
    env: common_environment
)
