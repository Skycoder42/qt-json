TEMPLATE = subdirs

SUBDIRS += lib

!cross_compile {
    SUBDIRS += \
        tests \
#        demo

	demo.depends += lib
	tests.depends += lib
}

OTHER_FILES += \
	README.md \
	qt-json.pri \
	.github/workflows/build.yml \
	.qmake.conf
