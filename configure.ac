AC_PREREQ(2.61)
AC_INIT([libiff], m4_esyscmd_s([echo -n $(cat ./version)$VERSION_SUFFIX]))
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([1.7.2 -Wall -Werror -Wno-extra-portability])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_PATH_PROG(HELP2MAN, help2man, false)

# Checks for headers
AC_CHECK_HEADER([getopt.h], [HAVE_GETOPT_H=1], [HAVE_GETOPT_H=0])
AC_SUBST(HAVE_GETOPT_H)

# Endianness check
AC_C_BIGENDIAN([IFF_BIG_ENDIAN=1],[IFF_BIG_ENDIAN=0])
AC_SUBST(IFF_BIG_ENDIAN)

# Output

AC_CONFIG_FILES([
Makefile
doc/Makefile
src/Makefile
src/libiff.pc
src/libiff/ifftypes.h
src/libiff/Makefile
src/iffjoin/Makefile
src/iffpp/Makefile
tests/Makefile
])
AC_OUTPUT
