QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    Worker.cpp \
    Server.cpp \
    Session.cpp

HEADERS += \
    Worker.h \
    define.h \
    Server.h \
    Session.h

