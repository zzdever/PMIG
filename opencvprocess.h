#ifndef OPENCVPROCESS_H
#define OPENCVPROCESS_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QPoint>
#include <QDebug>

#include <cv.h>
#include <highgui.h>

#include "toolbox.h"

using namespace cv;

class OpencvProcess:public QObject
{
    Q_OBJECT

private:
    int currentImageNum;

    BrushToolFunction *brushToolFunction;

protected:

public:
    void drawLineTo(int imageNum, QPoint lastPoint, QPoint currentPoint);

    QList<IplImage*> imageStack;
//    QList<Mat> imageStack;

    OpencvProcess(QWidget *parent);
    bool openImage(const char *fileName);
    void saveImage(const char *fileName);
    void setCurrentImageNum(int num);

    void ApplyToolFunction(ToolType::toolType toolType, int imageNum, QPoint lastPoint, QPoint currentPoint);
    void ApplyToolFunction(ToolType::toolType toolType, int imageNum, QPoint currentPoint);

signals:
    void updateDisplay(int changedImageNum);
};

#endif // OPENCVPROCESS_H
