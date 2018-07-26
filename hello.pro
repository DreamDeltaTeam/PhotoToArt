TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -O3

SOURCES += \
        main.cpp \
        lodepng.cpp \
        lodepng.h \
        helpers.cpp \
        helpers.h \
        image.cpp \
        image.h \
        mask.h \
        mask.cpp

HEADERS +=

