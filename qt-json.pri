CONFIG += c++17 exceptions

HEADERS += \
    $$PWD/src/configuration.h \
    $$PWD/src/iserializable.h \
    $$PWD/src/iserializablewrapperfactory.h \
    $$PWD/src/qtjson.h \
    $$PWD/src/qtjson_common_p.h \
    $$PWD/src/qtjson_exception.h \
    $$PWD/src/qtjson_metamacros.h \
    $$PWD/src/qtjson_p.h \
    $$PWD/src/serializablegadget.h \
    $$PWD/src/serializablewrapper.h

SOURCES += \
    $$PWD/src/iserializable.cpp \
    $$PWD/src/iserializablewrapperfactory.cpp \
    $$PWD/src/qtjson.cpp \
    $$PWD/src/qtjson_common.cpp \
    $$PWD/src/qtjson_exception.cpp \
    $$PWD/src/serializablegadget.cpp \
    $$PWD/src/serializablewrapper.cpp

INCLUDEPATH += $$PWD/src

QDEP_PACKAGE_EXPORTS += QTJSON_EXPORT
!qdep_build: DEFINES += "QTJSON_EXPORT="
