SUBDIRS = libiff iffjoin iffpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libiff.pc

apidox:
	doxygen Doxyfile

EXTRA_DIST = libiff.pc.in

# When calling help2man, we need to make sure that the output is
# redirected to stdout, not stderr.  This is because the output of
# iffjoin --help is redirected to stderr
HELP2MAN = @HELP2MAN@ --no-discard-stderr --version-string=$(VERSION)
