QT -= gui
TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11


TEMPLATE = app

SOURCES += main.cpp \
    worker.cpp \
    Client.cpp

HEADERS += \
    worker.h \
    define.h \
    Client.h


