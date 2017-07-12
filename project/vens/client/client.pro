QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    Worker.cpp \
    ../common/define.cpp \
    Client.cpp

HEADERS += \
    Worker.h \
    ../common/define.h \
    Client.h

