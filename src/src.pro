TEMPLATE = lib

QT = core
CONFIG += c++17

TARGET = qtjson

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QTJSON_LIBRARY

HEADERS += \
    iserializable.h \
    qtjson.h \
    qtjson_common_p.h \
    qtjson_global.h \
    qtjson_metamacros.h \
    serializablegadget.h

SOURCES += \
    iserializable.cpp \
    qtjson.cpp \
    qtjson_common.cpp \
    serializablegadget.cpp
