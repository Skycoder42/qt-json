TEMPLATE = subdirs

SUBDIRS += \
    testlib \
    ByteArrayTest

for(sdir, SUBDIRS): \
    !equals(sdir, testlib): \
    $${sdir}.depends += testlib
