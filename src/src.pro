TEMPLATE = lib

QT = core
CONFIG += c++17

TARGET = qtjson

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QTJSON_LIBRARY

HEADERS += \
    qtjson.h \
    qtjson_global.h

SOURCES += \
    qtjson.cpp
