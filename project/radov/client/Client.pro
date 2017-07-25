#QT += core
QT -= gui

CONFIG += c++11

TARGET = client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -lpqxx

INCLUDEPATH +=$$PWD/../common

SOURCES += \
    Client.cpp \
    Main.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    ../common/Worker.cpp \
    ClientManager.cpp


HEADERS += \
    ../common/Helper.h \
    ../common/Worker.h \
    Client.h \
    ClienManager.h \
    ../common/define.h
