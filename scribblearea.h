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

    bool isModified() const { return modified; }

    void setToolType(ToolType::toolType type);
    void setFgColor(QColor color) {fgColor=color;}
    void setBgColor(QColor color) {bgColor=color;}

    void ApplyToolFunction(QPoint lastPoint, QPoint currentPoint);
    void ApplyToolFunction(QPoint currentPoint);
    void ApplyToolFunction();

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void drawLineTo(QPoint lastPoint, QPoint currentPoint);
    void resizeImage(QImage *image, const QSize &newSize);

public slots:
//    void clearImage();
//    void print();
    void updateDisplay(int changedImageNum);
    void updateCursor();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent * event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    //    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    int totalImageNum, currentImageNum;
    QList<QImage> imageStackDisplay;
    QPoint imageCentralPoint;

    ToolType::toolType toolType;
    const int toolIndicationAlpha;
    QPolygonF marqueeHandlerControl;
    HoverPoints *marqueeHandler;

    BrushToolFunction *brushToolFunction;
    EraseToolFunction *eraseToolFunction;

    QList<IplImage*> imageStackEdit;
//    QList<Mat> imageStack;
    QImage CVMatToQImage(const cv::Mat& imgMat);
    QImage IplImage2QImage(const IplImage *iplImage, double mini, double maxi);


    QPoint lastPoint;
    CvPoint vertexLeftTop, vertexRightBottom;  // ellipse selection also uses this
    QVector<CvPoint> irregularSelectionPoints;
    long int irregularSelectionPointNum;
    QColor fgColor, bgColor;

    bool modified;
    bool somethingSelected;
    bool isMouseMoving;
    bool isMousePressed;


};

#endif
