QT -= gui
TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11


TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    Client.cpp \
    Worker.cpp \
    ../common/define.cpp
HEADERS += \
    ../common/define.h \
    Client.h \
    Worker.h


