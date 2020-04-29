TEMPLATE = lib

QT = core testlib
CONFIG += staticlib c++17

HEADERS += \
    serializationtest.h \
    testlib.h

SOURCES += \
    serializationtest.cpp \
    testlib.cpp

IS_TESTLIB = true
include(../tests.pri)
