AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

lib_LTLIBRARIES = libiffpp.la
pkginclude_HEADERS = pp.h ppinterface.h

libiffpp_la_SOURCES = pp.c ppinterface.c
libiffpp_la_CFLAGS = -I../libiff
libiffpp_la_LIBADD = ../libiff/libiff.la
