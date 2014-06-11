
QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport


TARGET = PMIG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    layerstack.cpp \
    mainwindow0.cpp \
    colorswatch.cpp \
    mainwindow2.cpp \
    opencvprocess.cpp \
    scribblearea.cpp \
    toolbar.cpp \
    toolbox.cpp

HEADERS  += mainwindow.h \
    layerstack.h \
    mainwindow0.h \
    colorswatch.h \
    mainwindow2.h \
    opencvprocess.h \
    scribblearea.h \
    toolbar.h \
    toolbox.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc


QMAKE_INCDIR += /opt/include\
                /opt/include/opencv/\
                /opt/include/opencv2/

LIBS += /opt/lib/libopencv_core.so\
        /opt/lib/libopencv_highgui.so\
        /opt/lib/libopencv_imgproc.so\
