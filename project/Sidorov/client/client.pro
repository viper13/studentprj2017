QT -= gui
TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11


TEMPLATE = app

INCLUDEPATH += $$PWD/../common/

LIBS += -lpqxx

SOURCES += main.cpp \
    Client.cpp \
    ../common/Worker.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    ChatClient.cpp \
    MessageManager.cpp
HEADERS += \
    ../common/define.h \
    Client.h \
    ../common/Worker.h \
    ../common/Helper.h \
    ChatClient.h \
    MessageManager.h



