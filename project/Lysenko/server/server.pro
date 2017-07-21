QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

SOURCES += main.cpp \
    Server.cpp \
    ../common/Worker.cpp \
    ../common/define.cpp \
    ../common/BufferConverter.cpp \
    MessageSession.cpp \
    CommandSession.cpp \
    ChatManager.cpp

HEADERS += \
    ../common/Worker.h \
    ../common/define.h \
    Server.h \
    ../common/BufferConverter.h \
    MessageSession.h \
    CommandSession.h \
    ChatManager.h


