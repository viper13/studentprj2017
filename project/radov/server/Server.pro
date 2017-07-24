#QT += core
QT -= gui

CONFIG += c++11

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH +=$$PWD/../common

LIBS += -lpqxx

#LIBS += -I"/usr/include/pqxx"

SOURCES += \
    Server.cpp \
    Session.cpp \
    Main.cpp \
    ../common/Worker.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    ChatManager.cpp \
    ChatRoom.cpp \
    SessionManager.cpp \
    DataDaseManager.cpp

HEADERS += \
    Server.h \
    Session.h \
    ChatManager.h \
    ChatRoom.h \
    ../common/Worker.h \
    ../common/define.h \
    ../common/Helper.h \
    SessionManager.h \
    DataDaseManager.h

OTHER_FILES += ../dump_db.sql


#DEFINES += QT_DEPRECATED_WARNINGS
