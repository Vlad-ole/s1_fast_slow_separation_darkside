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
    main.cpp \
    test_link_libs.cpp

INCLUDEPATH += "/home/darkside/Vlad_Programs/root-6.06.08/include"

LIBS += -L"/home/darkside/Vlad_Programs/root-6.06.08/lib" \
-lCore \
-lCling \
-lASImage \
-lASImageGui \
-lASImageGui \
-lcomplexDict \
-ldequeDict \
-lEG \
-lEve \
-lFitPanel \
-lFoam \
-lforward_listDict \
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
-lMinuit \
-lMLP \
-lmultimap2Dict \
-lmultimapDict \
-lMultiProc \
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
-lTMVAGui \
-lTMVA \
-lTreePlayer \
-lTree \
-lTreeViewer \
-lunordered_mapDict \
-lunordered_multimapDict \
-lunordered_multisetDict \
-lunordered_setDict \
-lvalarrayDict \
-lvectorDict \
-lVMC \
-lX3d \
-lXMLIO
