QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    ../common/Worker.cpp \
    Server.cpp \
    Session.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    ChatSession.cpp \
    ChatManager.cpp \
    ChatRoom.cpp

HEADERS += \
    ../common/Worker.h \
    ../common/define.h \
    Server.h \
    Session.h \
    ../common/Helper.h \
    ChatSession.h \
    ChatManager.h \
    ChatRoom.h

