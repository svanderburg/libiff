AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = iffpp
noinst_HEADERS = pp.h

iffpp_SOURCES = main.c pp.c
iffpp_LDADD = ../libiff/libiff.la
iffpp_CFLAGS = -I../libiff
