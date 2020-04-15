QT = core

CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lqtjson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lqtjson
else:unix: LIBS += -L$$OUT_PWD/../src/ -lqtjson

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src
