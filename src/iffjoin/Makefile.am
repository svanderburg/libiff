iffjoin.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name 'Joins an arbitrary number of IFF files into a single concatenation IFF file' --libtool ./iffjoin

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = iffjoin
noinst_HEADERS = join.h
man1_MANS = iffjoin.1

iffjoin_SOURCES = main.c join.c
iffjoin_LDADD = ../libiff/libiff.la
iffjoin_CFLAGS = -I../libiff

EXTRA_DIST = iffjoin.1
