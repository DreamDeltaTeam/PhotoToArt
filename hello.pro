TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -Ofast

SOURCES += \
        main.cpp \
        lodepng.cpp \
        lodepng.h \
        helpers.cpp \
        helpers.h \
        image.cpp \
        image.h \
        mask.h \
        mask.cpp \
        cxxopts.hpp

HEADERS +=

