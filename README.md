GSignOn daemon
==============

The GSignOn daemon is a D-Bus service which performs user authentication on
behalf of its clients. There are currently authentication plugins for OAuth 1.0
and 2.0, SASL, Digest-MD5, and plain username/password combination.


Build instructions
------------------

This project depends on GLib and SQLite, and uses the Meson build system. To build it, run
```
meson build --prefix=/usr
cd build
ninja
sudo ninja install
```

Configuration
-------------

You can use `mesonconf -D option=value` to set the configuration values of your choice.

The different options on this project are:

- bus_type : specify the DBus type used. accepted values are `session`, `system` and `p2p`.
- extension : select the extension to build. existing extensions are `default`, `ostro`, `tizen` and `desktop`.
- enable_debug : enable debugging related options.
- enable_doc : build and install the documentation.
- enable_keychain : system context of the keychain.
- enable_storagedir : path for user specific storage directories.

License
-------

See COPYING file.

Resources
---------

[gsignond API reference documentation](http://accounts-sso.gitlab.io/gsignond/index.html)

[Official source code repository](https://gitlab.com/accounts-sso/gsignond)
