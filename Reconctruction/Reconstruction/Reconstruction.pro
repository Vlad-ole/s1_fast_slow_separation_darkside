QT += core
QT -= gui

CONFIG += c++11

TARGET = Reconstruction
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    test_read_write.cpp \
    main_.cpp \
    main.cpp
