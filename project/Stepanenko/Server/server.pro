
QT -= qui

TARGET = server
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += $$PWD/../Common/

LIBS += -lpqxx

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    sesion.cpp \
    ../Common/define.cpp \
    ../Common/worker.cpp \
    ../Common/protocol.cpp \
    ../Common/helper.cpp \
    chatroom.cpp \
    chatSession.cpp \
    chatManager.cpp \
    databaseManager.cpp

HEADERS += \
    ../Common/define.h \
    ../Common/worker.h \
    ../Common/helper.h \
    ../Common/protocol.h \
    server.h \
    sesion.h \
    chatroom.h \
    chatSession.h \
    chatManager.h \
    databaseManager.h

OTHER_FILES += ../dump_db

