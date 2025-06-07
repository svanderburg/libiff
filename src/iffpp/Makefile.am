iffpp.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name 'IFF pretty printer' --libtool ./iffpp

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = iffpp
noinst_HEADERS = pp.h
man1_MANS = iffpp.1

iffpp_SOURCES = main.c pp.c
iffpp_LDADD = $(top_builddir)/src/libiff/libiff.la
iffpp_CFLAGS =  \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

EXTRA_DIST = iffpp.1
