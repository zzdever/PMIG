QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

TARGET = PMIG
TEMPLATE = app



HEADERS += \
    toolbox.h \
    scribblearea.h \
    panels.h \
    mainwindow.h \
    shared/hoverpoints.h

SOURCES += \
    toolbox.cpp \
    scribblearea.cpp \
    panels.cpp \
    opencvprocess.cpp \
    mainwindow.cpp \
    main.cpp \
    shared/hoverpoints.cpp

RESOURCES += \
    resources.qrc



INCLUDEPATH += /usr/local/include/opencv/
INCLUDEPATH += /usr/local/include/


LIBS += /usr/local/lib/libopencv_core.dylib\
        /usr/local/lib/libopencv_highgui.dylib\
        /usr/local/lib/libopencv_imgproc.dylib
