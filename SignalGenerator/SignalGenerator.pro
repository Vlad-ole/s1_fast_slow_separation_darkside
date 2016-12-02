QT += core
QT -= gui

CONFIG += c++11

TARGET = SignalGenerator
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    response.cpp \
    ../Classes/src/typeconvert.cpp \
    ../Classes/src/writedata.cpp \
    ../Classes/src/wunfold.cpp \
    generator.cpp \
    pdfgenerator.cpp

INCLUDEPATH += "/home/darkside/Vlad_Programs/root_roofit_fft_gsl/include"
INCLUDEPATH += "/home/darkside/Vlad_Programs/root_roofit_fft_gsl/math/mathmore/inc"
INCLUDEPATH += "../Classes/include"


LIBS += -L"/home/darkside/Vlad_Programs/root_roofit_fft_gsl/lib" \
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
 -lMathMore \
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
 -lRooFitCore \
 -lRooFit \
 -lRooStats \
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
    response.h \
    ../Classes/include/typeconvert.h \
    ../Classes/include/writedata.h \
    ../Classes/include/wunfold.h \
    generator.h \
    pdfgenerator.h

