iffcheck.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name 'IFF conformance checker' --libtool ./iffcheck

bin_PROGRAMS = iffcheck
man1_MANS = iffcheck.1

iffcheck_SOURCES = main.c
iffcheck_LDADD = ../libiff/libiff.la ../libiffcheck/libiffcheck.la
iffcheck_CFLAGS = -I../libiff -I../libiffcheck

EXTRA_DIST = iffcheck.1
