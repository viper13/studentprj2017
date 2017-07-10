TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    Worker.cpp \
    client.cpp

HEADERS += \
    Worker.h \
    define.h \
    client.h

