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
