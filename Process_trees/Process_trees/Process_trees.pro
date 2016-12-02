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
    graph_sigma_comparison.cpp \
    unfolding_example.cpp \
    unfolding_wiener.cpp \
    Graph_read/graph_real.cpp \
    Graph_read/graph_sep_prob.cpp \
    Graph_read/graph_sigma_comparison.cpp \
    Graph_read/graph_simulation.cpp \
    Graph_read/graph_tree.cpp \
    test/test_parse.cpp \
    test/test_vector.cpp \
    Unfolding/unfolding.cpp \
    Unfolding/unfolding_example.cpp \
    Unfolding/unfolding_wiener.cpp \
    Unfolding/wunfold.cpp \
    fit_avr_am_signal_unfold.cpp \
    combinecanvas.cpp \
    getgraph.cpp

HEADERS +=
