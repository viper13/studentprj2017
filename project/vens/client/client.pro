QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    ../common/Worker.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    Client.cpp

HEADERS += \
    ../common/Worker.h \
    ../common/define.h \
    ../common/Helper.h \
    Client.h

