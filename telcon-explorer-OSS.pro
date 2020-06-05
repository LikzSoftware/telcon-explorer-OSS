! include( common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

TEMPLATE = subdirs

SUBDIRS = \
	app \
	CppUnitLite \
	src \
	tests

app.depends = src
tests.depends = src CppUnitLite

unix {
  INSTALLS += datafiles 
  datafiles.path = $$DATADIR
  datafiles.files = meta/color_schemes.txt meta/land_contours.dat meta/new_land_contours.dat
}
