cmake_minimum_required(VERSION 3.5)

project(qt-json LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

add_library(qt-json
    configuration.h
    iserializable.h
    iserializable.cpp
    qtjson.h
    qtjson.cpp
    qtjson_exception.h
    qtjson_exception.cpp
    qtjson_global.h
    qtjson_helpers.h
    qtjson_helpers.cpp
    serializableadapter.h
    serializablearray.h
    serializablearray_p.h
    serializablebytearray.h
    serializablebytearray.cpp
    serializabledatetime.h
    serializabledatetime.cpp
    serializabledictionary.h
    serializabledictionary_p.h
    serializablegadget.h
    serializablegadget_p.h
    serializablegadget.cpp
    serializableoptional.h
    serializableversionnumber.h
    serializableversionnumber.cpp
)

target_link_libraries(qt-json PRIVATE Qt5::Core)

target_compile_definitions(qt-json PRIVATE QTJSON_LIBRARY)
