!equals(IS_TESTLIB, true) {
    debug_and_release:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../testlib/release/ -ltestlib
    else:debug_and_release:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../testlib/debug/ -ltestlib
    else: LIBS += -L$$OUT_PWD/../testlib/ -ltestlib

    INCLUDEPATH += $$PWD/testlib
    DEPENDPATH += $$PWD/testlib

    win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../testlib/release/testlib.lib
    else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../testlib/debug/testlib.lib
    else:debug_and_release:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../testlib/release/libtestlib.a
    else:debug_and_release:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../testlib/debug/libtestlib.a
    else: PRE_TARGETDEPS += $$OUT_PWD/../testlib/libtestlib.a
}

debug_and_release:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/release/ -l$$qtLibraryTarget(qt-json)
else:debug_and_release:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/debug/ -l$$qtLibraryTarget(qt-json)
else: LIBS += -L$$OUT_PWD/../../lib/ -l$$qtLibraryTarget(qt-json)

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

DEFINES += "QTJSON_EXPORT=Q_DECL_IMPORT"

win32 {
    libenvd.name = PATH
    libenvd.CONFIG = always_prepend
    libenvd.value = $$shell_path($$absolute_path($$OUT_PWD/../../lib/debug/))
    libenvr.name = PATH
    libenvr.CONFIG = always_prepend
    libenvr.value = $$shell_path($$absolute_path($$OUT_PWD/../../lib/release/))
    QT_TOOL_ENV += libenvr libenvd
} else {
    darwin: libenv.name = DYLD_LIBRARY_PATH
    else: libenv.name = LD_LIBRARY_PATH
    libenv.CONFIG = always_prepend
    libenv.value = $$shell_path($$absolute_path($$OUT_PWD/../../lib))
    QT_TOOL_ENV += libenv
}
