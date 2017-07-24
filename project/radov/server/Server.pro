#QT += core
QT -= gui

CONFIG += c++11

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH +=$$PWD/../common

SOURCES += \
    Server.cpp \
    Session.cpp \
    Main.cpp \
    ../common/Worker.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    ChatManager.cpp \
    ChatRoom.cpp \
    SessionManager.cpp

HEADERS += \
    Server.h \
    Session.h \
    ChatManager.h \
    ChatRoom.h \
    ../common/Worker.h \
    ../common/Define.h \
    ../common/Helper.h \
    ../common/Helper.h \
    SessionManager.h


#DEFINES += QT_DEPRECATED_WARNINGS
