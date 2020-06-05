! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

TEMPLATE = app
TARGET = ../telcon-explorer

QT += \
#core \
#    gui \
    opengl

LIBS += -lnetcdf \
    -lgsl \
    -lgslcblas \
    -lm

LIBS += -L.. \
    -ltelcon

HEADERS += \
    startup.h 

SOURCES += \
    startup.cpp \
    main.cpp 

unix {
  INSTALLS += target 
  target.path =$$BINDIR
}
