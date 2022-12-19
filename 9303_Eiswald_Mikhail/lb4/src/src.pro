TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        args/args.cpp \
        main.cpp \
        matrix/matrix.cpp
        matrix/matrix.cpp

HEADERS += \
    args/args.h \
    matrix/matrix.h

LIBS += \
    -lpthread
