noinst_HEADERS = bye.h hello.h conversation.h test.h testregistry.h catdata.h formdata.h listdata.h formdata-pad.h nestedformdata.h extensiondata.h extensiondata-truncated.h extensiondata-truncated2.h extensiondata-extended.h extensiondata2.h

check_PROGRAMS = writeform readform writeform-pad readform-pad writenestedform readnestedform writecat readcat writelist readlist \
    searchforms-form searchforms-cat searchforms-nestedform recalculatechunksizes \
    lookupproperty-simple lookupproperty-array lookupproperty-array-shared lookupproperty-prop lookupproperty-override-local lookupproperty-override-shared lookupproperty-nested \
    writeextension readextension testcheck testpp \
    writeextension-truncated readextension-truncated writeextension-truncated2 readextension-truncated2 writeextension-extended readextension-extended writeextension2 readextension2 \
    catcrudoperations listcrudoperations formcrudoperations extensioncrudoperations \
    testtextchunk listlookups

writeform_SOURCES = formdata.c writeform.c
writeform_LDADD = ../src/libiff/libiff.la
writeform_CFLAGS = -I../src/libiffcore -I../src/libiff

readform_SOURCES = formdata.c readform.c
readform_LDADD = ../src/libiff/libiff.la
readform_CFLAGS = -I../src/libiffcore -I../src/libiff

writeform_pad_SOURCES = formdata-pad.c writeform-pad.c
writeform_pad_LDADD = ../src/libiff/libiff.la
writeform_pad_CFLAGS = -I../src/libiffcore -I../src/libiff

readform_pad_SOURCES = formdata-pad.c readform-pad.c
readform_pad_LDADD = ../src/libiff/libiff.la
readform_pad_CFLAGS = -I../src/libiffcore -I../src/libiff

writenestedform_SOURCES = nestedformdata.c writenestedform.c
writenestedform_LDADD = ../src/libiff/libiff.la
writenestedform_CFLAGS = -I../src/libiffcore -I../src/libiff

readnestedform_SOURCES = nestedformdata.c readnestedform.c
readnestedform_LDADD = ../src/libiff/libiff.la
readnestedform_CFLAGS = -I../src/libiffcore -I../src/libiff

writecat_SOURCES = catdata.c writecat.c
writecat_LDADD = ../src/libiff/libiff.la
writecat_CFLAGS = -I../src/libiffcore -I../src/libiff

readcat_SOURCES = catdata.c readcat.c
readcat_LDADD = ../src/libiff/libiff.la
readcat_CFLAGS = -I../src/libiffcore -I../src/libiff

writelist_SOURCES = listdata.c writelist.c
writelist_LDADD = ../src/libiff/libiff.la
writelist_CFLAGS = -I../src/libiffcore -I../src/libiff

readlist_SOURCES = listdata.c readlist.c
readlist_LDADD = ../src/libiff/libiff.la
readlist_CFLAGS = -I../src/libiffcore -I../src/libiff

lookupproperty_simple_SOURCES = lookupproperty-simple.c
lookupproperty_simple_LDADD = ../src/libiff/libiff.la
lookupproperty_simple_CFLAGS = -I../src/libiffcore -I../src/libiff

lookupproperty_array_SOURCES = lookupproperty-array.c
lookupproperty_array_LDADD = ../src/libiff/libiff.la
lookupproperty_array_CFLAGS = -I../src/libiffcore -I../src/libiff

lookupproperty_array_shared_SOURCES = lookupproperty-array-shared.c
lookupproperty_array_shared_LDADD = ../src/libiff/libiff.la
lookupproperty_array_shared_CFLAGS = -I../src/libiffcore -I../src/libiff

lookupproperty_prop_SOURCES = lookupproperty-prop.c
lookupproperty_prop_LDADD = ../src/libiff/libiff.la
lookupproperty_prop_CFLAGS  = -I../src/libiffcore -I../src/libiff

lookupproperty_override_shared_SOURCES = lookupproperty-override-shared.c
lookupproperty_override_shared_LDADD = ../src/libiff/libiff.la
lookupproperty_override_shared_CFLAGS = -I../src/libiffcore -I../src/libiff

lookupproperty_override_local_SOURCES = lookupproperty-override-local.c
lookupproperty_override_local_LDADD = ../src/libiff/libiff.la
lookupproperty_override_local_CFLAGS = -I../src/libiffcore -I../src/libiff

lookupproperty_nested_SOURCES = lookupproperty-nested.c
lookupproperty_nested_LDADD = ../src/libiff/libiff.la
lookupproperty_nested_CFLAGS = -I../src/libiffcore -I../src/libiff

searchforms_form_SOURCES = searchforms-form.c
searchforms_form_LDADD = ../src/libiff/libiff.la
searchforms_form_CFLAGS = -I../src/libiffcore -I../src/libiff

searchforms_cat_SOURCES = searchforms-cat.c
searchforms_cat_LDADD = ../src/libiff/libiff.la
searchforms_cat_CFLAGS = -I../src/libiffcore -I../src/libiff

searchforms_nestedform_SOURCES = searchforms-nestedform.c
searchforms_nestedform_LDADD = ../src/libiff/libiff.la
searchforms_nestedform_CFLAGS = -I../src/libiffcore -I../src/libiff

recalculatechunksizes_SOURCES = recalculatechunksizes.c
recalculatechunksizes_LDADD = ../src/libiff/libiff.la
recalculatechunksizes_CFLAGS = -I../src/libiffcore -I../src/libiff

writeextension_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata.c writeextension.c
writeextension_LDADD = ../src/libiff/libiff.la
writeextension_CFLAGS = -I../src/libiffcore -I../src/libiff

readextension_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata.c readextension.c
readextension_LDADD = ../src/libiff/libiff.la
readextension_CFLAGS = -I../src/libiffcore -I../src/libiff

testcheck_SOURCES = hello.c bye.c conversation.c test.c testregistry.c testcheck.c
testcheck_LDADD = ../src/libiff/libiff.la ../src/libiffcheck/libiffcheck.la
testcheck_CFLAGS = -I../src/libiffcore -I../src/libiff -I../src/libiffcheck

testpp_SOURCES = hello.c bye.c conversation.c test.c testregistry.c testpp.c
testpp_LDADD = ../src/libiff/libiff.la ../src/libiffpp/libiffpp.la
testpp_CFLAGS = -I../src/libiffcore -I../src/libiff -I../src/libiffpp

writeextension_truncated_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata-truncated.c writeextension-truncated.c
writeextension_truncated_LDADD = ../src/libiff/libiff.la
writeextension_truncated_CFLAGS = -I../src/libiffcore -I../src/libiff

readextension_truncated_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata-truncated.c readextension-truncated.c
readextension_truncated_LDADD = ../src/libiff/libiff.la
readextension_truncated_CFLAGS = -I../src/libiffcore -I../src/libiff

writeextension_truncated2_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata-truncated2.c writeextension-truncated2.c
writeextension_truncated2_LDADD = ../src/libiff/libiff.la
writeextension_truncated2_CFLAGS = -I../src/libiffcore -I../src/libiff

readextension_truncated2_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata-truncated2.c readextension-truncated2.c
readextension_truncated2_LDADD = ../src/libiff/libiff.la
readextension_truncated2_CFLAGS = -I../src/libiffcore -I../src/libiff

writeextension_extended_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata-extended.c writeextension-extended.c
writeextension_extended_LDADD = ../src/libiff/libiff.la
writeextension_extended_CFLAGS = -I../src/libiffcore -I../src/libiff

readextension_extended_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata-extended.c readextension-extended.c
readextension_extended_LDADD = ../src/libiff/libiff.la
readextension_extended_CFLAGS = -I../src/libiffcore -I../src/libiff

writeextension2_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata2.c writeextension2.c
writeextension2_LDADD = ../src/libiff/libiff.la
writeextension2_CFLAGS = -I../src/libiffcore -I../src/libiff

readextension2_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensiondata2.c readextension2.c
readextension2_LDADD = ../src/libiff/libiff.la
readextension2_CFLAGS = -I../src/libiffcore -I../src/libiff

catcrudoperations_SOURCES = catdata.c catcrudoperations.c
catcrudoperations_LDADD = ../src/libiff/libiff.la
catcrudoperations_CFLAGS = -I../src/libiffcore -I../src/libiff

listcrudoperations_SOURCES = listdata.c listcrudoperations.c
listcrudoperations_LDADD = ../src/libiff/libiff.la
listcrudoperations_CFLAGS = -I../src/libiffcore -I../src/libiff

formcrudoperations_SOURCES = catdata.c formcrudoperations.c
formcrudoperations_LDADD = ../src/libiff/libiff.la
formcrudoperations_CFLAGS = -I../src/libiffcore -I../src/libiff

extensioncrudoperations_SOURCES = hello.c bye.c conversation.c test.c testregistry.c extensioncrudoperations.c
extensioncrudoperations_LDADD = ../src/libiff/libiff.la
extensioncrudoperations_CFLAGS = -I../src/libiffcore -I../src/libiff

testtextchunk_SOURCES = testtextchunk.c
testtextchunk_LDADD = ../src/libiff/libiff.la
testtextchunk_CFLAGS = -I../src/libiffcore -I../src/libiff

listlookups_SOURCES = hello.c bye.c conversation.c test.c testregistry.c listlookups.c
listlookups_LDADD = ../src/libiff/libiff.la
listlookups_CFLAGS = -I../src/libiffcore -I../src/libiff

TESTS = writeform readform writeform-pad readform-pad writenestedform readnestedform writecat readcat writelist readlist \
    validform.sh validcat.sh validcat-wildcard.sh validlist.sh validlist-wildcard.sh invalidiff.sh \
    invalidid1.sh invalidid2.sh invalidformtype1.sh invalidformtype2.sh invalidformtype3.sh invalidformtype4.sh \
    invalidform-prop.sh invalidform-size1.sh invalidform-size2.sh \
    invalidcat-raw.sh invalidcat-prop.sh invalidcat-contentstype.sh invalidcat-size.sh \
    invalidlist-raw.sh invalidlist-contentstype.sh invalidlist-size.sh \
    invalidprop.sh invalidprop-size.sh invalidlist-negsize.sh \
    pp-text.sh searchforms-form searchforms-cat searchforms-nestedform recalculatechunksizes \
    lookupproperty-simple lookupproperty-array lookupproperty-array-shared lookupproperty-prop lookupproperty-override-local lookupproperty-override-shared lookupproperty-nested \
    join-identical.sh join-different.sh \
    writeextension readextension testcheck.sh testpp-c.sh testpp-otherform.sh \
    writeextension-truncated readextension-truncated writeextension-truncated2 readextension-truncated2 writeextension-extended readextension-extended writeextension2 readextension2 \
    catcrudoperations listcrudoperations formcrudoperations extensioncrudoperations \
    testtextchunk listlookups

EXTRA_DIST = invalidcat-contentstype.sh invalidcat-prop.sh invalidcat-raw.sh invalidcat-size.sh invalidform-prop.sh invalidform-size1.sh \
    invalidform-size2.sh invalidformtype1.sh invalidformtype2.sh invalidformtype3.sh invalidformtype4.sh invalidid1.sh invalidid2.sh \
    invalid.IFF invalidiff.sh invalidlist-contentstype.sh invalidlist-raw.sh invalidlist-size.sh invalidprop.sh invalidprop-size.sh join-different.sh \
    join-identical.sh testcheck.sh testpp-c.sh testpp-otherform.sh pp-text.sh validcat.sh validcat-wildcard.sh validform.sh validlist.sh validlist-wildcard.sh \
    extension-otherform.TEST invalidcat-contentstype.TEST invalidcat-prop.TEST invalidcat-raw.TEST invalidcat-size.TEST invalidform-prop.TEST \
    invalidform-size1.TEST invalidform-size2.TEST invalidformtype1.TEST invalidformtype2.TEST invalidformtype3.TEST invalidformtype4.TEST \
    invalidid1.TEST invalidid2.TEST invalidlist-contentstype.TEST invalidlist-raw.TEST invalidlist-size.TEST invalidprop-size.TEST invalidprop.TEST \
    lookupproperty-array-shared.TEST lookupproperty-nested.TEST lookupproperty-override-local.TEST lookupproperty-override-shared.TEST pp-text.TEST validcat-wildcard.TEST validlist-wildcard.TEST \
    join.HELO join.BYE invalidlist-negsize.sh invalidlist-negsize.TEST
