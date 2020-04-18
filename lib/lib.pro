TEMPLATE = lib

QT = core
CONFIG += qdep_build

TARGET = qt-json

include(../qt-json.pri)

DEFINES += "QTJSON_EXPORT=Q_DECL_EXPORT"
