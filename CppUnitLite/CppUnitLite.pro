TEMPLATE = lib
TARGET = ../CppUnitLite

CONFIG -= qt
CONFIG += staticlib

#QMAKE_CXXFLAGS += -Wall -Werror

SOURCES += \
	Failure.cpp \
	Test.cpp \
	TestResult.cpp \
	SimpleString.cpp \
	TestRegistry.cpp

HEADERS += \
	Failure.h \
       	Test.h \
       	TestRegistry.h \
	SimpleString.h \
	TestHarness.h \
	TestResult.h
