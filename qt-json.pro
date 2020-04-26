TEMPLATE = subdirs

SUBDIRS += lib

!cross_compile {
	SUBDIRS += \
		demo \
		tests

	demo.depends += lib
	tests.depends += lib
}

OTHER_FILES += \
	README.md \
	qt-json.pri \
	.github/workflows/build.yml \
	.qmake.conf
