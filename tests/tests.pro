TEMPLATE = subdirs

SUBDIRS +=  \
    AdapterTest \
    ArrayTest \
    ByteArrayTest \
    DateTimeTest \
    testlib

for(sdir, SUBDIRS): \
    !equals(sdir, testlib): \
    $${sdir}.depends += testlib

SerializableArrayTest.depends += SerializableAdapterTest
