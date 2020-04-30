TEMPLATE = subdirs

SUBDIRS +=  \
    ByteArrayTest \
    DateTimeTest \
    testlib

for(sdir, SUBDIRS): \
    !equals(sdir, testlib): \
    $${sdir}.depends += testlib
