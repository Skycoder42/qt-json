CONFIG += c++17 exceptions

HEADERS += \
	$$PWD/src/serializablearray.h \
	$$PWD/src/serializablebytearray.h \
	$$PWD/src/serializabledatetime.h \
	$$PWD/src/configuration.h \
	$$PWD/src/iserializable.h \
	$$PWD/src/qtjson.h \
	$$PWD/src/qtjson_common_p.h \
	$$PWD/src/qtjson_exception.h \
	$$PWD/src/serializableadapter.h \
	$$PWD/src/serializabledictionary.h \
	$$PWD/src/serializablegadget.h \
	$$PWD/src/serializableoptional.h \
	$$PWD/src/serializableversionnumber.h \
	$$PWD/src/serializablewrapper.h

SOURCES += \
	$$PWD/src/serializablebytearray.cpp \
	$$PWD/src/serializabledatetime.cpp \
	$$PWD/src/iserializable.cpp \
	$$PWD/src/qtjson.cpp \
	$$PWD/src/qtjson_common.cpp \
	$$PWD/src/qtjson_exception.cpp \
	$$PWD/src/serializablegadget.cpp \
	$$PWD/src/serializableversionnumber.cpp

INCLUDEPATH += $$PWD/src

QDEP_PACKAGE_EXPORTS += QTJSON_EXPORT
!qdep_build: DEFINES += "QTJSON_EXPORT="
