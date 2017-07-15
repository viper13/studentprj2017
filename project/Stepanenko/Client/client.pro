QT -= qui

TARGET = qtTestApp
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += $$PWD/../Common/

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp \
    ../Common/define.cpp \
    ../Common/worker.cpp \
    ../Common/helper.cpp

HEADERS += \
    ../Common/define.h \
    ../Common/worker.h \
    ../Common/helper.h \
    client.h


