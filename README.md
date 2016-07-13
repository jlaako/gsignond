GSignOn daemon
=============

The GSignOn daemon is a D-Bus service which performs user authentication on
behalf of its clients. There are currently authentication plugins for OAuth 1.0
and 2.0, SASL, Digest-MD5, and plain username/password combination.


License
-------

See COPYING file.


Build instructions
------------------

This project depends on GLib and SQLite. To build it, run
```
./autogen.sh
make
make install
```

Resources
---------

[gsignond API reference documentation](http://accounts-sso.gitlab.io/gsignond/index.html)

[Official source code repository](https://gitlab.com/accounts-sso/gsignond)
