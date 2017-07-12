QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    client.cpp \
    worker.cpp \
    ../common/define.cpp

HEADERS += \
    ../common/define.h \
    client.h \
    worker.h

