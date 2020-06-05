! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

TEMPLATE = lib
TARGET = ../telcon
CONFIG += staticlib

QT += \
#    core \
#    gui \
    opengl

HEADERS += storage/datastorage.h \
    storage/ncfiledatastorage.h \ 
    storage/tcstorage.h \
    exploration/maps/maptype.h \
    exploration/coordinatetext.h \
    preferences/preferencestorage.h \
    storage/cfgfileparser.h \
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
    storage/filesystem.h \
    storage/pathresolver.h \
    storage/precomputeddata.h \
    colorizer/rgb.h \
    colorizer/transferfunctioneditor.h \
    colorizer/transferfunctionstorage.h \
    colorizer/texture.h \
    preferences/preferences.h \
    preferences/preferencepanelogic.h \
    preferences/preferencepane.h \
    process/precompute.h \
    process/progressbar.h \
    storage/read.h \
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

SOURCES += storage/ncfiledatastorage.cpp \ 
	storage/tcstorage.cpp \
    exploration/coordinatetext.cpp \
    exploration/maps/mapgrid.cpp \
    preferences/preferencestorage.cpp \
    storage/cfgfileparser.cpp \
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
    storage/filesystem.cpp \
    storage/pathresolver.cpp \
    storage/precomputeddata.cpp \
    preferences/preferences.cpp \
    colorizer/transferfunctioneditor.cpp \
    colorizer/transferfunctionstorage.cpp \
    preferences/preferencepanelogic.cpp \
    preferences/preferencepane.cpp \
    process/precompute.cpp \
    storage/read.cpp \
    colorizer/transferfunctionobject.cpp \
    colorizer/transferfunctioneditorwidget.cpp \
    colorizer/transferfunctionwidget.cpp \
    colorizer/icolorizer.cpp \
    projection/distancematrix.cpp \
    projection/sammon.cpp \
    projection/tspoint.cpp \
    exploration/regions/regionsearchexplorer.cpp

FORMS += exploration/explorationwidget.ui \
    preferences/preferencepane.ui \
    colorizer/transferfunctionwidget.ui \
    exploration/regions/regionsearchexplorer.ui
