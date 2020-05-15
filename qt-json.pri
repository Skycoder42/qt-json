CONFIG += c++17 exceptions

HEADERS += \
	$$PWD/src/qtjson.h \
	$$PWD/src/qtjson_helpers.h \
	$$PWD/src/serializablearray.h \
	$$PWD/src/serializablearray_p.h \
	$$PWD/src/serializablebytearray.h \
	$$PWD/src/serializabledatetime.h \
	$$PWD/src/configuration.h \
	$$PWD/src/iserializable.h \
	$$PWD/src/qtjson_exception.h \
	$$PWD/src/serializableadapter.h \
	$$PWD/src/serializabledictionary.h \
	$$PWD/src/serializabledictionary_p.h \
	$$PWD/src/serializablegadget.h \
	$$PWD/src/serializablegadget_p.h \
	$$PWD/src/serializableoptional.h \
	$$PWD/src/serializableversionnumber.h

SOURCES += \
	$$PWD/src/qtjson_helpers.cpp \
	$$PWD/src/serializablebytearray.cpp \
	$$PWD/src/serializabledatetime.cpp \
	$$PWD/src/iserializable.cpp \
	$$PWD/src/qtjson_exception.cpp \
	$$PWD/src/serializablegadget.cpp \
	$$PWD/src/serializableversionnumber.cpp

INCLUDEPATH += $$PWD/src

QDEP_PACKAGE_EXPORTS += QTJSON_EXPORT
!qdep_build: DEFINES += "QTJSON_EXPORT="
