QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    ../common/define.cpp \
    ../common/worker.cpp \
    ../common/helper.cpp \
    ChatSession.cpp \
    ChatManager.cpp \
    Server.cpp \
    Session.cpp

HEADERS += \
    ../common/define.h \
    ../common/worker.h\
    ../common/helper.h \
    ChatSession.h \
    ChatManager.h \
    Server.h \
    Session.h


