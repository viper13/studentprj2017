TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp \
    worker.cpp

HEADERS += \
    define.h \
    client.h \
    worker.h

