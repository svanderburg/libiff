iffpp.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name 'IFF pretty printer' --libtool ./iffpp

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = iffpp
noinst_HEADERS = pp.h

iffpp_SOURCES = main.c pp.c
iffpp_LDADD = ../libiff/libiff.la
iffpp_CFLAGS = -I../libiff

man1_MANS = iffpp.1
EXTRA_DIST = iffpp.1
