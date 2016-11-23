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
    otonu_help.cpp \
    test_vector.cpp \
    test_parse.cpp \
    graph_tree.cpp \
    graph_simulation.cpp \
    graph_real.cpp \
    sigma_estimation.cpp \
    graph_sep_prob.cpp \
    read_slices.cpp \
    graph_sigma_comparison.cpp

HEADERS +=
