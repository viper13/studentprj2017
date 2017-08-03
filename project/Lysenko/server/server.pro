QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

LIBS += -lpqxx

SOURCES += main.cpp \
    Server.cpp \
    ../common/Worker.cpp \
    ../common/define.cpp \
    ../common/BufferConverter.cpp \
    MessageSession.cpp \
    CommandSession.cpp \
    DataBaseManager.cpp \
    ChatManager.cpp

HEADERS += \
    ../common/Worker.h \
    ../common/define.h \
    Server.h \
    ../common/BufferConverter.h \
    MessageSession.h \
    CommandSession.h \
    DataBaseManager.h \
    ChatManager.h

OTHER_FILES += $$PWD/../dump_db.sql
