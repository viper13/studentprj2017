QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    Worker.cpp \
    client.cpp \
    ../common/define.cpp

HEADERS += \
    Worker.h \
    ../common/define.h \
    client.h

