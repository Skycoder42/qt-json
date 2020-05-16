TEMPLATE = lib

QT = core testlib
CONFIG += staticlib c++17 exceptions

HEADERS += \
    serializationtest.h \
    testserializable.h

SOURCES += \
    serializationtest.cpp \
    testserializable.cpp

IS_TESTLIB = true
include(../tests.pri)
