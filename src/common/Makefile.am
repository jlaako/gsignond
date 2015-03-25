include $(top_srcdir)/common.mk

SUBDIRS = db 
NULL=

lib_LTLIBRARIES = libgsignond-common.la

BUILT_SOURCES = \
    gsignond-plugin-enum-types.h \
    gsignond-plugin-enum-types.c \
    $(NULL)

GSIGNOND_INCLUDE_DIR=$(top_srcdir)/include/gsignond

gsignond-plugin-enum-types.h: gsignond-enum.h.template $(GSIGNOND_INCLUDE_DIR)/gsignond-plugin-interface.h
	$(GLIB_MKENUMS) --template $(srcdir)/gsignond-enum.h.template \
                    --fhead "#ifndef GSIGNOND_PLUGIN_ENUM_TYPES_H_\n#define GSIGNOND_PLUGIN_ENUM_TYPES_H_\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS" \
                    --identifier-prefix GSignond \
                    --ftail "\nG_END_DECLS\n\n#endif\n" \
                    $(GSIGNOND_INCLUDE_DIR)/gsignond-plugin-interface.h > $@

gsignond-plugin-enum-types.c: gsignond-enum.c.template $(GSIGNOND_INCLUDE_DIR)/gsignond-plugin-interface.h
	$(GLIB_MKENUMS) --template $(srcdir)/gsignond-enum.c.template \
                    --fhead "#include \"gsignond-plugin-enum-types.h\"\n" \
                    --identifier-prefix GSignond \
                    $(GSIGNOND_INCLUDE_DIR)/gsignond-plugin-interface.h > $@


libgsignond_common_la_CPPFLAGS = \
    -I$(top_builddir) \
    -I$(top_srcdir)/src \
    -I$(top_srcdir)/include \
    -I. \
    $(GSIGNOND_CFLAGS) \
    -DGSIGNOND_GPLUGINS_DIR='"$(gpluginsdir)"' \
    -DGSIGNOND_SYSCONF_DIR='"$(sysconfdir)"' \
    $(NULL)

libgsignond_common_la_DEPENDENCIES = \
    $(top_builddir)/src/common/db/libgsignond-common-db.la \
    $(NULL)

libgsignond_common_la_LIBADD = \
    $(GSIGNOND_LIBS) \
    $(top_builddir)/src/common/db/libgsignond-common-db.la \
    $(NULL)

libgsignond_common_la_SOURCES = \
    gsignond-security-context.c \
    gsignond-access-control-manager.c \
    gsignond-extension-interface.c \
    gsignond-storage-manager.c \
    gsignond-identity-info.h \
    gsignond-identity-info-internal.h \
    gsignond-identity-info.c \
    gsignond-credentials.c \
    gsignond-config.c \
    gsignond-error.c \
    gsignond-plugin-interface.c \
    gsignond-dictionary.c \
    gsignond-session-data.c \
    gsignond-signonui-data.c \
    gsignond-utils.c \
    gsignond-pipe-stream.h \
    gsignond-pipe-stream.c \
    gsignond-disposable.h \
    gsignond-disposable.c \
    $(BUILT_SOURCES) \
    $(NULL)

dist_libgsignond_common_la_SOURCES = \
    gsignond-enum.c.template \
    gsignond-enum.h.template \
    $(NULL)

CLEANFILES = \
    gsignond-plugin-enum-types.h \
    gsignond-plugin-enum-types.c \
    $(NULL)

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --add-include-path=$(GSIGNOND_INCLUDE_DIR)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir) --includedir=$(GSIGNOND_INCLUDE_DIR)

if HAVE_INTROSPECTION
introspection_sources = \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-signonui-data.h \
    gsignond-signonui-data.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-security-context.h \
    gsignond-security-context.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-access-control-manager.h \
    gsignond-access-control-manager.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-extension-interface.h \
    gsignond-extension-interface.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-storage-manager.h \
    gsignond-storage-manager.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-secret-storage.h \
    db/gsignond-secret-storage.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-credentials.h \
    gsignond-credentials.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-config.h \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-config-dbus.h \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-config-general.h \
    gsignond-config.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-error.h \
    gsignond-error.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-plugin-interface.h \
    gsignond-plugin-interface.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-dictionary.h \
    gsignond-dictionary.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-session-data.h \
    gsignond-session-data.c \
    $(GSIGNOND_INCLUDE_DIR)/gsignond-utils.h \
    gsignond-utils.c \
    $(BUILT_SOURCES) \
    $(NULL)

gSignond-1.0.gir: $(lib_LTLIBRARIES)
gSignond_1_0_gir_INCLUDES = GObject-2.0 Gio-2.0
gSignond_1_0_gir_SCANNERFLAGS = \
    --identifier-prefix=GSignond \
    --symbol-prefix=gsignond \
    --c-include="gsignond/gsignond.h" \
    --warn-all
gSignond_1_0_gir_CFLAGS = $(libgsignond_common_la_CPPFLAGS)
gSignond_1_0_gir_LIBS = $(lib_LTLIBRARIES)
gSignond_1_0_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += gSignond-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif

# Vala bindings
if ENABLE_VAPIGEN
-include $(VAPIGEN_MAKEFILE)
gsignond.vapi: gSignond-1.0.gir gsignond.deps gSignond-1.0-custom.vala

VAPIGEN_VAPIS = gsignond.vapi

gsignond_vapi_DEPS = gio-2.0 glib-2.0
gsignond_vapi_METADATADIRS = $(srcdir)
gsignond_vapi_FILES = gSignond-1.0.gir $(srcdir)/gSignond-1.0-custom.vala

vapidir = $(datadir)/vala/vapi
vapi_DATA = $(VAPIGEN_VAPIS) $(VAPIGEN_VAPIS:.vapi=.deps)

CLEANFILES += $(VAPIGEN_VAPIS)
EXTRA_DIST = gsignond.deps gSignond-1.0-custom.vala
endif
