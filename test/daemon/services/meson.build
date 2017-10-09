
daemon_test_data = configuration_data()
daemon_test_data.set('abs_top_builddir', meson.build_root())
configure_file(
    input: 'com.google.code.AccountsSSO.gSingleSignOn.service.in',
    output: 'com.google.code.AccountsSSO.gSingleSignOn.service',
    configuration: daemon_test_data
)
