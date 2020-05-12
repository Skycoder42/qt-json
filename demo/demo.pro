QT = core

CONFIG += c++17 console exceptions
CONFIG -= app_bundle

SOURCES += \
	demogadget.cpp \
	main.cpp

debug_and_release:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/release/ -l$$qtLibraryTarget(qt-json)
else:debug_and_release:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/debug/ -l$$qtLibraryTarget(qt-json)
else:unix: LIBS += -L$$OUT_PWD/../lib/ -l$$qtLibraryTarget(qt-json)

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

DEFINES += "QTJSON_EXPORT=Q_DECL_IMPORT"

HEADERS += \
	demogadget.h
