#ifndef OPENCVPROCESS_H
#define OPENCVPROCESS_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QPoint>
#include <QVector>
#include <QDebug>

#include <cv.h>
#include <highgui.h>

#include "toolbox.h"

using namespace cv;

class OpencvProcess:public QObject
{
    Q_OBJECT

private:
    BrushToolFunction *brushToolFunction;

protected:

public:
    int currentImageNum;
    ToolType::toolType toolType;

    bool somethingSelected;
    //QRect rectangleSelection;
    CvPoint vertexA, vertexB;  // ellipse selection also uses this
    QVector<CvPoint> irregularSelectionPoints;
    long int irregularSelectionPointNum;


    void drawLineTo(QPoint lastPoint, QPoint currentPoint);

    //IplImage* toolIndicationImage;
    QList<IplImage*> imageStack;
//    QList<Mat> imageStack;

    OpencvProcess(QWidget *parent);
    bool openImage(const char *fileName);
    bool saveImage(const char *fileName, const char *fileFormat);
    //void setCurrentImageNum(int num);

    void ApplyToolFunction(QPoint lastPoint, QPoint currentPoint);
    void ApplyToolFunction(QPoint currentPoint);
    void ApplyToolFunction();

signals:
    void updateDisplay(int changedImageNum);
};

#endif // OPENCVPROCESS_H
