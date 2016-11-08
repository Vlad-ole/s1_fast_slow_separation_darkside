QT += core
QT -= gui

CONFIG += c++11

TARGET = Process_trees
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    fft.cpp \
    canvas.cpp \
    main_am.cpp \
    unfolding.cpp \
    main_1pe.cpp \
    main.cpp

HEADERS +=
