cmake_minimum_required(VERSION 3.5)

project(qt-json LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(qt-json
    qtjson.cpp
    qtjson_helpers.cpp
    serializablebytearray.cpp
    serializabledatetime.cpp
    iserializable.cpp
    qtjson_exception.cpp
    serializablegadget.cpp
    serializableversionnumber.cpp
)

target_link_libraries(qt-json PRIVATE Qt5::Core)

target_compile_definitions(qt-json PRIVATE QTJSON_LIBRARY)
