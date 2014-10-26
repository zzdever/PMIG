#-------------------------------------------------
#
# Project created by QtCreator 2014-10-11T18:21:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PMIG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    colorswatch.cpp \
    layerstack.cpp \
    opencvprocess.cpp \
    scribblearea.cpp \
    toolbar.cpp \
    toolbox.cpp \
    shared/hoverpoints.cpp

HEADERS  += mainwindow.h \
    colorswatch.h \
    layerstack.h \
    opencvprocess.h \
    scribblearea.h \
    toolbar.h \
    toolbox.h \
    shared/hoverpoints.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv/
INCLUDEPATH += /usr/local/include/

RESOURCES += \
    resources.qrc


LIBS += /usr/local/lib/libopencv_core.dylib\
        /usr/local/lib/libopencv_highgui.dylib\
        /usr/local/lib/libopencv_imgproc.dylib
