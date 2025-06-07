noinst_HEADERS = bye.h hello.h test.h catdata.h formdata.h listdata.h formdata-pad.h nestedformdata.h extensiondata.h extensiondata-truncated.h extensiondata-truncated2.h extensiondata-extended.h

check_PROGRAMS = writeform readform writeform-pad readform-pad writenestedform readnestedform writecat readcat writelist readlist invalidiff validiff \
    searchforms-form searchforms-cat searchforms-nestedform updatechunksizes lookupproperty-simple lookupproperty-prop lookupproperty-override lookupproperty-nested \
    writeextension readextension checkextension ppextension \
    writeextension-truncated readextension-truncated writeextension-truncated2 readextension-truncated2 writeextension-extended readextension-extended

writeform_SOURCES = formdata.c writeform.c
writeform_LDADD = ${top_builddir}/src/libiff/libiff.la
writeform_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readform_SOURCES = formdata.c readform.c
readform_LDADD = ${top_builddir}/src/libiff/libiff.la
readform_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

writeform_pad_SOURCES = formdata-pad.c writeform-pad.c
writeform_pad_LDADD = ${top_builddir}/src/libiff/libiff.la
writeform_pad_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readform_pad_SOURCES = formdata-pad.c readform-pad.c
readform_pad_LDADD = ${top_builddir}/src/libiff/libiff.la
readform_pad_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

writenestedform_SOURCES = nestedformdata.c writenestedform.c
writenestedform_LDADD = ${top_builddir}/src/libiff/libiff.la
writenestedform_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readnestedform_SOURCES = nestedformdata.c readnestedform.c
readnestedform_LDADD = ${top_builddir}/src/libiff/libiff.la
readnestedform_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

writecat_SOURCES = catdata.c writecat.c
writecat_LDADD = ${top_builddir}/src/libiff/libiff.la
writecat_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readcat_SOURCES = catdata.c readcat.c
readcat_LDADD = ${top_builddir}/src/libiff/libiff.la
readcat_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

writelist_SOURCES = listdata.c writelist.c
writelist_LDADD = ${top_builddir}/src/libiff/libiff.la
writelist_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readlist_SOURCES = listdata.c readlist.c
readlist_LDADD = ${top_builddir}/src/libiff/libiff.la
readlist_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

validiff_SOURCES = validiff.c
validiff_LDADD = ${top_builddir}/src/libiff/libiff.la
validiff_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

invalidiff_SOURCES = invalidiff.c
invalidiff_LDADD = ${top_builddir}/src/libiff/libiff.la
invalidiff_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

lookupproperty_simple_SOURCES = lookupproperty-simple.c
lookupproperty_simple_LDADD = ${top_builddir}/src/libiff/libiff.la
lookupproperty_simple_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

lookupproperty_prop_SOURCES = lookupproperty-prop.c
lookupproperty_prop_LDADD = ${top_builddir}/src/libiff/libiff.la
lookupproperty_prop_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

lookupproperty_override_SOURCES = lookupproperty-override.c
lookupproperty_override_LDADD = ${top_builddir}/src/libiff/libiff.la
lookupproperty_override_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

lookupproperty_nested_SOURCES = lookupproperty-nested.c
lookupproperty_nested_LDADD = ${top_builddir}/src/libiff/libiff.la
lookupproperty_nested_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

searchforms_form_SOURCES = searchforms-form.c
searchforms_form_LDADD = ${top_builddir}/src/libiff/libiff.la
searchforms_form_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

searchforms_cat_SOURCES = searchforms-cat.c
searchforms_cat_LDADD = ${top_builddir}/src/libiff/libiff.la
searchforms_cat_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

searchforms_nestedform_SOURCES = searchforms-nestedform.c
searchforms_nestedform_LDADD = ${top_builddir}/src/libiff/libiff.la
searchforms_nestedform_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

updatechunksizes_SOURCES = updatechunksizes.c
updatechunksizes_LDADD = ${top_builddir}/src/libiff/libiff.la
updatechunksizes_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

writeextension_SOURCES = hello.c bye.c test.c extensiondata.c writeextension.c
writeextension_LDADD = ${top_builddir}/src/libiff/libiff.la
writeextension_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readextension_SOURCES = hello.c bye.c test.c extensiondata.c readextension.c
readextension_LDADD = ${top_builddir}/src/libiff/libiff.la
readextension_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

checkextension_SOURCES = hello.c bye.c test.c checkextension.c
checkextension_LDADD = ${top_builddir}/src/libiff/libiff.la
checkextension_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

ppextension_SOURCES = hello.c bye.c test.c ppextension.c
ppextension_LDADD = ${top_builddir}/src/libiff/libiff.la
ppextension_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

writeextension_truncated_SOURCES = hello.c bye.c test.c extensiondata-truncated.c writeextension-truncated.c
writeextension_truncated_LDADD = ${top_builddir}/src/libiff/libiff.la
writeextension_truncated_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readextension_truncated_SOURCES = hello.c bye.c test.c extensiondata-truncated.c readextension-truncated.c
readextension_truncated_LDADD = ${top_builddir}/src/libiff/libiff.la
readextension_truncated_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

writeextension_truncated2_SOURCES = hello.c bye.c test.c extensiondata-truncated2.c writeextension-truncated2.c
writeextension_truncated2_LDADD = ${top_builddir}/src/libiff/libiff.la
writeextension_truncated2_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readextension_truncated2_SOURCES = hello.c bye.c test.c extensiondata-truncated2.c readextension-truncated2.c
readextension_truncated2_LDADD = ${top_builddir}/src/libiff/libiff.la
readextension_truncated2_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

writeextension_extended_SOURCES = hello.c bye.c test.c extensiondata-extended.c writeextension-extended.c
writeextension_extended_LDADD = ${top_builddir}/src/libiff/libiff.la
writeextension_extended_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

readextension_extended_SOURCES = hello.c bye.c test.c extensiondata-extended.c readextension-extended.c
readextension_extended_LDADD = ${top_builddir}/src/libiff/libiff.la
readextension_extended_CFLAGS = \
    -I${top_srcdir}/src/libiff \
    -I${top_builddir}/src/libiff

TESTS = writeform readform writeform-pad readform-pad writenestedform readnestedform writecat readcat writelist readlist \
    validform.sh validcat.sh validcat-wildcard.sh validlist.sh validlist-wildcard.sh invalidiff.sh \
    invalidid1.sh invalidid2.sh invalidformtype1.sh invalidformtype2.sh invalidformtype3.sh invalidformtype4.sh \
    invalidform-prop.sh invalidform-size1.sh invalidform-size2.sh \
    invalidcat-raw.sh invalidcat-prop.sh invalidcat-contentstype.sh invalidcat-size.sh \
    invalidlist-raw.sh invalidlist-contentstype.sh invalidlist-size.sh \
    invalidprop.sh invalidprop-size.sh invalidlist-negsize.sh \
    pp-text.sh searchforms-form searchforms-cat searchforms-nestedform updatechunksizes \
    lookupproperty-simple lookupproperty-prop lookupproperty-override lookupproperty-nested \
    join-identical.sh join-different.sh \
    writeextension readextension checkextension ppextension-c.sh ppextension-otherform.sh \
    writeextension-truncated readextension-truncated writeextension-truncated2 readextension-truncated2 writeextension-extended readextension-extended

EXTRA_DIST = invalidcat-contentstype.sh invalidcat-prop.sh invalidcat-raw.sh invalidcat-size.sh invalidform-prop.sh invalidform-size1.sh \
    invalidform-size2.sh invalidformtype1.sh invalidformtype2.sh invalidformtype3.sh invalidformtype4.sh invalidid1.sh invalidid2.sh \
    invalidiff.sh invalidlist-contentstype.sh invalidlist-raw.sh invalidlist-size.sh invalidprop.sh invalidprop-size.sh join-different.sh \
    join-identical.sh ppextension-c.sh ppextension-otherform.sh pp-text.sh validcat.sh validcat-wildcard.sh validform.sh validlist.sh validlist-wildcard.sh \
    extension-otherform.TEST invalidcat-contentstype.TEST invalidcat-prop.TEST invalidcat-raw.TEST invalidcat-size.TEST invalidform-prop.TEST \
    invalidform-size1.TEST invalidform-size2.TEST invalidformtype1.TEST invalidformtype2.TEST invalidformtype3.TEST invalidformtype4.TEST \
    invalidid1.TEST invalidid2.TEST invalidlist-contentstype.TEST invalidlist-raw.TEST invalidlist-size.TEST invalidprop-size.TEST invalidprop.TEST \
    lookupproperty-nested.TEST lookupproperty-override.TEST pp-text.TEST validcat-wildcard.TEST validlist-wildcard.TEST \
    join.HELO join.BYE invalidlist-negsize.sh invalidlist-negsize.TEST
