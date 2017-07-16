QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    Server.cpp \
    session.cpp \
    ../common/Worker.cpp \
    ../common/define.cpp \
    ../common/bufferconverter.cpp

HEADERS += \
    ../common/Worker.h \
    ../common/define.h \
    Server.h \
    session.h \
    ../common/bufferconverter.h


