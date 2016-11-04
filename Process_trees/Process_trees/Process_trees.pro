QT += core
QT -= gui

CONFIG += c++11

TARGET = Process_trees
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    fft.cpp \
    canvas.cpp \
    main_am.cpp \
    unfolding.cpp

HEADERS +=
