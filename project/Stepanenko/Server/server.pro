
QT -= qui

TARGET = qtTestApp
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += $$PWD/../Common/

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    sesion.cpp \
    ../Common/define.cpp \
    ../Common/worker.cpp \
    ../Common/protocol.cpp \
    ../Common/helper.cpp \
    chatroom.cpp \
    clientsContainer.cpp \
    chatSession.cpp \
    chatManager.cpp

HEADERS += \
    ../Common/define.h \
    ../Common/worker.h \
    ../Common/helper.h \
    ../Common/protocol.h \
    server.h \
    sesion.h \
    chatroom.h \
    clientsContainer.h \
    chatSession.h \
    chatManager.h


