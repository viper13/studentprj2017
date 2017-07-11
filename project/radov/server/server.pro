QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    session.cpp \
    worker.cpp \
    server.cpp

HEADERS += \
    define.h \
    session.h \
    worker.h \
    server.h

