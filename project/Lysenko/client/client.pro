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
    MessageClient.cpp \
    CommandClient.cpp \
    ../common/BufferConverter.cpp

HEADERS += \
    ../common/Worker.h \
    ../common/define.h \
    MessageClient.h \
    CommandClient.h \
    ../common/BufferConverter.h

LIBS += -lpqxx
