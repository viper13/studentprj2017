QT -= qui

TARGET = qtTestApp
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += $$PWD/../Common/

TEMPLATE = app

SOURCES += main.cpp \
    worker.cpp \
    client.cpp \
    ../Common/define.cpp

HEADERS += \
    worker.h \
    ../Common/define.h \
    client.h


