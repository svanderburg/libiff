iffcheck.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name 'IFF conformance checker' --libtool ./iffcheck

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = iffcheck
noinst_HEADERS = check.h
man1_MANS = iffcheck.1

iffcheck_SOURCES = main.c check.c
iffcheck_LDADD = ../libiff/libiff.la
iffcheck_CFLAGS = -I../libiff

EXTRA_DIST = iffcheck.1
