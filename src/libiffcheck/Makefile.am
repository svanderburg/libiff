AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

lib_LTLIBRARIES = libiffcheck.la
pkginclude_HEADERS = check.h checkinterface.h

libiffcheck_la_SOURCES = check.c checkinterface.c
libiffcheck_la_CFLAGS = -I../libiff
libiffcheck_la_LIBADD = ../libiff/libiff.la
