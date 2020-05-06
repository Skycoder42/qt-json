TEMPLATE = subdirs

SUBDIRS +=  \
	AdapterTest \
	ArrayTest \
	ByteArrayTest \
	DateTimeTest \
	DictionaryTest \
	GadgetTest \
	OptionalTest \
	testlib

for(sdir, SUBDIRS): \
	!equals(sdir, testlib): \
	$${sdir}.depends += testlib

ArrayTest.depends += AdapterTest
DictionaryTest.depends += AdapterTest
OptionalTest.depends += AdapterTest
