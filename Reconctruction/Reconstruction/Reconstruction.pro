QT += core
QT -= gui

CONFIG += c++11

TARGET = Reconstruction
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    read_file.cpp \
    derivative.cpp \
    baseline.cpp \
    integral.cpp \
    fill_canvas.cpp \
    fft.cpp \
    main_1pe.cpp \
    main_am.cpp

INCLUDEPATH += "/home/darkside/Vlad_Programs/root/include"

LIBS += -L"/home/darkside/Vlad_Programs/root/lib" \
 -lASImageGui \
 -lASImage \
 -lCintex \
 -lCint \
 -lcomplexDict \
 -lCore \
 -ldequeDict \
 -lEG \
 -lEve \
 -lFitPanel \
 -lFoam \
 -lFTGL \
 -lFumili \
 -lGed \
 -lGenetic \
 -lGenVector \
 -lGeomBuilder \
 -lGeomPainter \
 -lGeom \
 -lGLEW \
 -lGpad \
 -lGraf3d \
 -lGraf \
 -lGuiBld \
 -lGuiHtml \
 -lGui \
 -lGviz3d \
 -lGX11 \
 -lGX11TTF \
 -lHbook \
 -lHistPainter \
 -lHist \
 -lHtml \
 -llistDict \
 -lmap2Dict \
 -lmapDict \
 -lMathCore \
 -lMatrix \
 -lMemStat \
 -lminicern \
 -lMinuit \
 -lMLP \
 -lmultimap2Dict \
 -lmultimapDict \
 -lmultisetDict \
 -lNet \
 -lNew \
 -lPhysics \
 -lPostscript \
 -lProofBench \
 -lProofDraw \
 -lProofPlayer \
 -lProof \
 -lQuadp \
 -lRecorder \
 -lReflexDict \
 -lReflex \
 -lRGL \
 -lRint \
 -lRIO \
 -lRootAuth \
 -lSessionViewer \
 -lsetDict \
 -lSmatrix \
 -lSpectrumPainter \
 -lSpectrum \
 -lSPlot \
 -lSQLIO \
 -lSrvAuth \
 -lThread \
 -lTMVA \
 -lTreePlayer \
 -lTree \
 -lTreeViewer \
 -lvalarrayDict \
 -lvectorDict \
 -lVMC \
 -lX3d \
 -lXMLIO

HEADERS += \
    read_file.h \
    derivative.h \
    baseline.h \
    integral.h \
    fill_canvas.h \
    fft.h
