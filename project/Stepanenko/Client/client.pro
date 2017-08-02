QT -= qui

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += $$PWD/../Common/

LIBS += -lpqxx

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp \
    ../Common/define.cpp \
    ../Common/worker.cpp \
    ../Common/protocol.cpp \
    ../Common/helper.cpp \
    chatClient.cpp \
    messageManager.cpp

HEADERS += \
    ../Common/define.h \
    ../Common/worker.h \
    ../Common/helper.h \
    ../Common/protocol.h \
    client.h \
    chatClient.h \
    messageManager.h


