TEMPLATE = subdirs

SUBDIRS +=  \
    ByteArrayTest \
    DateTimeTest \
    SerializableAdapterTest \
    testlib

for(sdir, SUBDIRS): \
    !equals(sdir, testlib): \
    $${sdir}.depends += testlib
