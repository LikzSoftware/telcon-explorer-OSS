INCLUDEPATH += $$PWD/src
SRC_DIR = $$PWD
QMAKE_CXXFLAGS += -Wall -std=c++11

MOC_DIR = moc
OBJECTS_DIR = build


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

unix {
  #VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /usr
  }
  BINDIR = $$PREFIX/bin
  DATADIR =$$PREFIX/share/telcon

  DEFINES += DATADIR=\\\"$$DATADIR\\\" 
}
