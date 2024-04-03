iffpp.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name 'IFF pretty printer' --libtool ./iffpp

bin_PROGRAMS = iffpp
man1_MANS = iffpp.1

iffpp_SOURCES = main.c
iffpp_LDADD = ../libiff/libiff.la ../libiffpp/libiffpp.la
iffpp_CFLAGS = -I../libiff -I../libiffpp

EXTRA_DIST = iffpp.1
