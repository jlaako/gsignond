include $(top_srcdir)/common.mk
extensions_LTLIBRARIES = libextension-ostro.la

libextension_ostro_la_CPPFLAGS = \
	-I$(top_builddir) \
	-I$(top_srcdir)/include \
	$(GSIGNOND_CFLAGS) \
	$(LIBSMACK_CFLAGS) \
	$(LIBECRYPTFS_CFLAGS) \
	$(LIBTSPI_CFLAGS)

libextension_ostro_la_LIBADD = \
	$(top_builddir)/src/common/libgsignond-common.la \
	$(GSIGNOND_LIBS) \
	$(LIBSMACK_LIBS) \
	$(LIBECRYPTFS_LIBS) \
	$(LIBTSPI_LIBS)

libextension_ostro_la_SOURCES = \
    ostro-access-control-manager.h \
    ostro-access-control-manager.c \
    ostro-extension.h \
    ostro-extension.c \
    ostro-secret-storage.h \
    ostro-secret-storage.c \
    ostro-storage-manager.h \
    ostro-storage-manager.c

libextension_ostro_la_LDFLAGS = -avoid-version
