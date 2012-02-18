AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = iffjoin
noinst_HEADERS = join.h

iffjoin_SOURCES = main.c join.c
iffjoin_LDADD = ../libiff/libiff.la
iffjoin_CFLAGS = -I../libiff
