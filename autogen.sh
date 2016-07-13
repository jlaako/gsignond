#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

olddir=`pwd`
cd $srcdir

check_exists() {
	variable=`which $1`

	if test -z $variable; then
		echo "*** No $1 found, please intall it ***" >&2
		exit 1
	fi
}

check_exists aclocal
check_exists autoreconf
check_exists gtkdocize

m4dir=`autoconf --trace 'AC_CONFIG_MACRO_DIR:$1'`
if [ -n "$m4dir" ]; then
	mkdir -p $m4dir
fi

aclocal -I m4 || exit $?
gtkdocize --copy || exit $?
autoreconf --verbose --force --install -Wno-portability || exit $?

cd $olddir
test -n "$NOCONFIGURE" || "$srcdir/configure" --enable-maintainer-mode "$@"
