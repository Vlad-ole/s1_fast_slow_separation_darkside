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
    unfolding.cpp \
    main_1pe.cpp \
    main_unfold.cpp \
    fit_avr_am_signal.cpp \
    otonu_help.cpp

HEADERS +=
