gsso_interface_name = 'com.google.code.AccountsSSO.gSingleSignOn'
gsso_interface_prefix = gsso_interface_name + '.'

dbus_auth_service_xml = files (join_paths('interfaces', 'com.google.code.AccountsSSO.gSingleSignOn.AuthService.xml'))
dbus_auth_service_gen = gnome.gdbus_codegen(
    'gsignond-dbus-auth-service-gen',
    dbus_auth_service_xml,
    interface_prefix: gsso_interface_prefix,
    namespace: 'GSignondDbus'
)

dbus_auth_session_xml = files (join_paths('interfaces', 'com.google.code.AccountsSSO.gSingleSignOn.AuthSession.xml'))
dbus_auth_session_gen = gnome.gdbus_codegen(
    'gsignond-dbus-auth-session-gen',
    dbus_auth_session_xml,
    interface_prefix: gsso_interface_prefix,
    namespace: 'GSignondDbus'
)

dbus_identity_xml = files (join_paths('interfaces', 'com.google.code.AccountsSSO.gSingleSignOn.Identity.xml'))
dbus_identity_gen = gnome.gdbus_codegen(
    'gsignond-dbus-identity-gen',
    dbus_identity_xml,
    interface_prefix: gsso_interface_prefix,
    namespace: 'GSignondDbus'
)

dbus_remote_plugin_xml = files (join_paths('interfaces', 'com.google.code.AccountsSSO.gSingleSignOn.RemotePlugin.xml'))
dbus_remote_plugin_gen = gnome.gdbus_codegen(
    'gsignond-dbus-remote-plugin-gen',
    dbus_remote_plugin_xml,
    interface_prefix: gsso_interface_prefix,
    namespace: 'GSignondDbus'
)

# Create a configuration file

dbus_configuration_data = configuration_data()
if bus_type == 'p2p'
dbus_configuration_data.set_quoted ('GSIGNOND_DBUS_ADDRESS', 'unix:path=%s/gsignond/bus-sock')
endif
dbus_configuration_data.set_quoted ('GSIGNOND_SERVICE', gsso_interface_name)
dbus_configuration_data.set_quoted ('GSIGNOND_DAEMON_OBJECTPATH', '/com/google/code/AccountsSSO/gSingleSignOn')
dbus_configuration_data.set_quoted ('GSIGNOND_DAEMON_INTERFACE', gsso_interface_prefix + 'AuthService')
dbus_configuration_data.set_quoted ('GSIGNOND_IDENTITY_INTERFACE', gsso_interface_prefix + 'Identity')
dbus_configuration_data.set_quoted ('GSIGNOND_AUTH_SESSION_INTERFACE', gsso_interface_prefix + 'AuthSession')
dbus_configuration_data.set_quoted ('GSIGNOND_PLUGIN_OBJECTPATH', '/')

dbus_configuration_data.set_quoted ('SIGNONUI_SERVICE', gsso_interface_prefix + 'UI')
dbus_configuration_data.set_quoted ('SIGNONUI_OBJECTPATH', '/')
dbus_configuration_data.set_quoted ('SIGNONUI_IFACE', gsso_interface_prefix + 'UI')
dbus_configuration_data.set_quoted ('SIGNONUI_DIALOG_IFACE', gsso_interface_prefix + 'UI.Dialog')
dbus_configuration_data.set_quoted ('SIGNONUI_DIALOG_OBJECTPATH', '/Dialog')

dbus_header = configure_file(
    configuration: dbus_configuration_data,
    output: 'gsignond-dbus.h'
)

gsignond_dbus = declare_dependency(
    sources: dbus_header,
    include_directories: [include_directories('.'), root_dir]
)

# Install the .service file

service_configuration_data = configuration_data()
service_configuration_data.set ('NAME', gsso_interface_name)
service_configuration_data.set ('EXEC_PATH', join_paths(get_option('prefix'), get_option('bindir'), 'gsignond'))
configure_file(
    configuration: service_configuration_data,
    input: join_paths('services', 'com.google.code.AccountsSSO.gSingleSignOn.service.in'),
    output: 'com.google.code.AccountsSSO.gSingleSignOn.service',
    install_dir: join_paths(get_option('datadir'), 'dbus-1', 'services')
)

