#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QList>

#include <cv.h>
#include <highgui.h>

#include "toolbox.h"
#include "opencvprocess.h"
#include "shared/hoverpoints.h"


//! [0]
class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
//    void setPenColor(const QColor &newColor);
//    void setPenWidth(int newWidth);

    bool isModified() const { return modified; }

    void setToolType(ToolType::toolType type) {
        toolType=type;
        opencvProcess->toolType=type;
        return; }
//    QColor penColor() const { return myPenColor; }
//    int penWidth() const { return myPenWidth; }

public slots:
//    void clearImage();
    //void print();
    void updateDisplay(int changedImageNum);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseDoubleClickEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    OpencvProcess *opencvProcess;
    int totalImageNum, currentImageNum;
    QList<QImage> imageStack;
    QPoint imageCentralPoint;

    ToolType::toolType toolType;
    const int toolIndicationAlpha;
    QPolygonF marqueeHandlerControl;
    HoverPoints *marqueeHandler;


    QImage CVMatToQImage(const Mat& imgMat);
    QImage IplImage2QImage(const IplImage *iplImage, double mini, double maxi);


    bool modified;
    //bool scribbling;
    //int myPenWidth;
    //QColor myPenColor;
    //QImage image;
    QPoint lastPoint;
    bool isMouseMoving;
    bool isMousePressed;

//    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

};
//! [0]

#endif
