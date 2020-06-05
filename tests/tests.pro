! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

TEMPLATE = app
TARGET = ../telcon-tests

QT += \
	core \
       	gui \
	opengl

#CONFIG += console
#CONFIG -= qt

INCLUDEPATH += ..

LIBS += -lnetcdf \
    -lgsl \
    -lgslcblas \
    -lm

LIBS += -L.. \
	-ltelcon \
	-lCppUnitLite

HEADERS += \
	cppunitextras.h \
	preferences/fakepreferencepaneview.h

SOURCES += \
	colorizer/transferfunctioneditortest.cpp \
	colorizer/transferfunctionobjecttest.cpp \
	cppunitextras.cpp \
	exploration/maps/layouttest.cpp \
	exploration/maps/mapsubviewtest.cpp \ 
	storage/nhtests.cpp \
	storage/pathresolvertest.cpp \
	storage/precomputeddatatest.cpp \
	preferences/preferencepanelogictest.cpp \
	process/regionconnectivitytest.cpp \
	process/regionsearchtest.cpp \
	tests-main.cpp
