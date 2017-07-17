QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    session.cpp \
    server.cpp \
    ../common/define.cpp \
    ../common/worker.cpp \
    ../common/helper.cpp

HEADERS += \
    session.h \
    server.h \
    ../common/define.h \
    ../common/worker.h\
    ../common/helper.h


