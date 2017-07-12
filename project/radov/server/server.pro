QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    session.cpp \
    worker.cpp \
    server.cpp \
    ../common/define.cpp

HEADERS += \
    ../common/define.h \
    session.h \
    worker.h \
    server.h

