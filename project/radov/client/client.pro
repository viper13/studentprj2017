QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    Client.cpp \
    ../common/define.cpp \
    ../common/worker.cpp \
    ../common/helper.cpp

HEADERS += \
    Client.h \
    ../common/define.h \
    ../common/worker.h \
    ../common/helper.h


