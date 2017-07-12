QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    Worker.cpp \
    Server.cpp \
    session.cpp \
    ../common/define.cpp

HEADERS += \
    Worker.h \
    ../common/define.h \
    Server.h \
    session.h


