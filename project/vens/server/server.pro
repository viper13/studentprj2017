QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

LIBS += -lpqxx

SOURCES += main.cpp \
    ../common/Worker.cpp \
    ../common/Helper.cpp \
    Server.cpp \
    Session.cpp \
    ../common/define.cpp \
    ChatSession.cpp \
    ChatManager.cpp \
    DataBaseManager.cpp

HEADERS += \
    ../common/Worker.h \
    ../common/define.h \
    ../common/Helper.h \
    Server.h \
    Session.h \
    ChatSession.h \
    ChatManager.h \
    DataBaseManager.h

OTHER_FILES += $$PWD/../dump_db.sql
