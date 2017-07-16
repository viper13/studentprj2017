QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    client.cpp \
    ../common/Worker.cpp \
    ../common/define.cpp \
    ../common/bufferconverter.cpp

HEADERS += \
    client.h \
    ../common/Worker.h \
    ../common/define.h \
    ../common/bufferconverter.h

