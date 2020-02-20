TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        MadgwickAHRS_float.cpp \
        cmdline/cmdline.cpp \
        fastmatread.cpp \
        main.cpp \
        mpu.cpp \
        mpu_geometry.cpp

HEADERS += \
    MadgwickAHRS.h \
    MadgwickAHRS_float.h \
    cmdline/cmdline.h \
    fastmatread.h \
    mpu.h \
    mpu_geometry.h
