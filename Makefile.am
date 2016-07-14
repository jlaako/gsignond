ACLOCAL_AMFLAGS = -I m4

# --enable-distcheck is here to make it possible to use
# also --enable-dbus-type=session here
AM_DISTCHECK_CONFIGURE_FLAGS = \
    --enable-debug \
    --enable-gtk-doc \
    --enable-distcheck \
    --enable-introspection=yes

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gsignond.pc

sysconf_DATA = gsignond.conf

SUBDIRS = src test docs

EXTRA_DIST = dists tools m4/introspection.m4

valgrind:
	cd test; make valgrind

lcov: check
	@rm -rf lcov-report
	@lcov -c --directory src/ --output-file lcov.output
	@genhtml lcov.output --output-directory lcov-report
	@rm lcov.output
	@echo "Coverage report is in file://$(abs_srcdir)/lcov-report/index.html"
