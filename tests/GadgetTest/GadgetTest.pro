QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
	testgadget.h

SOURCES +=  tst_gadget.cpp \
	testgadget.cpp

include(../tests.pri)
