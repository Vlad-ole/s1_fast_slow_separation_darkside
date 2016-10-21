QT += core
QT -= gui

CONFIG += c++11

TARGET = Parser
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    RawEvent.cpp \
    Reader.cc

LIBS += -lz

HEADERS += \
    RawEvent.hh \
    Reader.hh \
    Message.hh \
    MessageHandler.hh \
    VParameterNode.hh \
    ConfigHandler.hh \
    ParameterList.hh \
    Parameter.hh \
    ConfigFunctor.hh \
    EventHandler.hh \
    Event.hh \
    phrase.hh \
    EventData.hh
