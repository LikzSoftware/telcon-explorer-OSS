TEMPLATE = app
TARGET = NCCorrelation
QT += core \
    gui \
    opengl
QMAKE_CXXFLAGS += -std=c++11
LIBS += -lnetcdf \
    -lgsl \
    -lgslcblas \
    -lm
MOC_DIR = moc
OBJECTS_DIR = build
HEADERS += init/datastorage.h \
    init/ncfiledatastorage.h \ 
    init/tcstorage.h \
    exploration/maps/maptype.h \
    exploration/coordinatetext.h \
    preferences/preferencestorage.h \
    init/cfgfileparser.h \
    exploration/projection/selectionhull.h \
    exploration/projection/projectionview.h \
    exploration/projection/projectionwidget.h \
    exploration/projection/transformmatrix2d.h \
    exploration/explorationmodelimpl.h \
    exploration/fakeexplorationmodel.h \
    exploration/maps/annotationlink.h \
    exploration/maps/textpainter.h \
    exploration/maps/equirectangularmapsubview.h \
    exploration/maps/polarmapsubview.h \
    exploration/maps/mapgrid.h \
    exploration/explorationmodel.h \
    exploration/explorationwidget.h \
    exploration/maps/legendsubview.h \
    exploration/maps/layout.h \
    exploration/maps/maplayoutview.h \
    exploration/maps/mrect.h \
    exploration/maps/mapsubview.h \
    process/link.h \
    process/regionconnectivity.h \
    process/regionsearch.h \
    init/filesystem.h \
    init/pathresolver.h \
    colorizer/rgb.h \
    colorizer/transferfunctioneditor.h \
    colorizer/transferfunctionstorage.h \
    colorizer/texture.h \
    preferences/preferences.h \
    preferences/preferencepanelogic.h \
    preferences/preferencepane.h \
    process/precompute.h \
    process/progressbar.h \
    init/read.h \
    init/startup.h \
    colorizer/transferfunctionobject.h \
    colorizer/transferfunctioneditorwidget.h \
    colorizer/transferfunctionwidget.h \
    colorizer/icolorizer.h \
    projection/distancematrix.h \
    projection/distancetype.h \
    projection/ilspdata.h \
    projection/projectedpointinfo.h \
    projection/sammon.h \
    projection/tspoint.h \
    typedefs.h \
    multiplatform/declareqcloseevent.h \
    multiplatform/declareqmouseevent.h \
    multiplatform/devicepixelratio.h \
    multiplatform/gl.h \
    multiplatform/glu.h \
    multiplatform/QWidget \
    multiplatform/QDialog \
    multiplatform/QApplication \
    exploration/regions/regionsearchexplorer.h \
    exploration/mouseselectionmode.h
SOURCES += init/ncfiledatastorage.cpp \ 
	init/tcstorage.cpp \
    exploration/coordinatetext.cpp \
    exploration/maps/mapgrid.cpp \
    preferences/preferencestorage.cpp \
    init/cfgfileparser.cpp \
    exploration/projection/selectionhull.cpp \
    exploration/projection/projectionview.cpp \
    exploration/projection/projectionwidget.cpp \
    exploration/projection/transformmatrix2d.cpp \
    exploration/explorationmodelimpl.cpp \
    exploration/fakeexplorationmodel.cpp \
    exploration/maps/equirectangularmapsubview.cpp \
    exploration/maps/polarmapsubview.cpp \
    exploration/explorationmodel.cpp \
    exploration/explorationwidget.cpp \
    exploration/maps/legendsubview.cpp \
    exploration/maps/layout.cpp \
    exploration/maps/maplayoutview.cpp \
    exploration/maps/mapsubview.cpp \
    process/regionconnectivity.cpp \
    process/regionsearch.cpp \
    init/filesystem.cpp \
    init/pathresolver.cpp \
    preferences/preferences.cpp \
    colorizer/transferfunctioneditor.cpp \
    colorizer/transferfunctionstorage.cpp \
    preferences/preferencepanelogic.cpp \
    preferences/preferencepane.cpp \
    process/precompute.cpp \
    init/read.cpp \
    init/startup.cpp \
    colorizer/transferfunctionobject.cpp \
    colorizer/transferfunctioneditorwidget.cpp \
    colorizer/transferfunctionwidget.cpp \
    colorizer/icolorizer.cpp \
    projection/distancematrix.cpp \
    projection/sammon.cpp \
    projection/tspoint.cpp \
    main.cpp \
    exploration/regions/regionsearchexplorer.cpp
FORMS += exploration/explorationwidget.ui \
    preferences/preferencepane.ui \
    colorizer/transferfunctionwidget.ui \
    exploration/regions/regionsearchexplorer.ui
RESOURCES += 
Config.Tests { 
    TARGET = NCCTests
    INCLUDEPATH += ..
    SOURCES -= main.cpp \
        init/startup.cpp
    HEADERS += cppunitextras.h \
        preferences/fakepreferencepaneview.h
    SOURCES += exploration/maps/mapsubviewtest.cpp \ 
    	init/nhtests.cpp \
        exploration/maps/layouttest.cpp \
        cppunitextras.cpp \
        process/regionconnectivitytest.cpp \
        process/regionsearchtest.cpp \
        init/pathresolvertest.cpp \
        colorizer/transferfunctioneditortest.cpp \
        preferences/preferencepanelogictest.cpp \
        colorizer/transferfunctionobjecttest.cpp \
        tests-main.cpp
    LIBS += -L../CppUnitLite \
        -lCppUnitLite
}

mac {
    EXTRAS_PATH = $$(HOME)/Developer/local
    INCLUDEPATH += $$EXTRAS_PATH/include \
            /usr/local/include
    LIBS += -L$$EXTRAS_PATH/lib \
            -L/usr/local/lib
    QMAKE_CXXFLAGS += -stdlib=libc++
    QMAKE_LFLAGS += -stdlib=libc++
    CONFIG -= app_bundle
}
