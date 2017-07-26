QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common

LIBS += -lpqxx

SOURCES += main.cpp \
    ../common/Worker.cpp \
    Server.cpp \
    Session.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    ChatManager.cpp \
    ChatSession.cpp \
    ChatRoom.cpp \
    User.cpp \
    DataBaseManager.cpp

HEADERS += \
    ../common/Worker.h \
    Server.h \
    Session.h \
    ../common/define.h \
    ../common/Helper.h \
    ChatManager.h \
    ChatSession.h \
    ChatRoom.h \
    User.h \
    DataBaseManager.h

DISTFILES += \
    ../dump_db.sql

