QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common

SOURCES += main.cpp \
    Worker.cpp \
    Server.cpp \
    Session.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp

HEADERS += \
    Worker.h \
    Server.h \
    Session.h \
    ../common/define.h \
    ../common/Helper.h

