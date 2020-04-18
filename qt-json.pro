TEMPLATE = subdirs

SUBDIRS += lib

!cross_compile {
    SUBDIRS += demo
    demo.depends += lib
}

OTHER_FILES += \
    README.md \
    qt-json.pri \
    .github/workflows/build.yml \
    .qmake.conf
