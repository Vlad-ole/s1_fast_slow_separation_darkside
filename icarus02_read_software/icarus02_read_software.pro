QT += core
QT -= gui

CONFIG += c++11

TARGET = icarus02_read_software
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Reader.cc \
    RawEvent.cc

HEADERS += \
    Reader.hh \
    ConfigHandler.hh \
    EventHandler.hh \
    RawEvent.hh
