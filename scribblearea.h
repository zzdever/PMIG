#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QList>
#include <QCursor>

#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

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
    void selectAll(void);

    void ApplyToolFunction(QPoint lastPoint, QPoint currentPoint);
    void ApplyToolFunction(QPoint currentPoint);
    void ApplyToolFunction();

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void drawLineTo(QPoint lastPoint, QPoint currentPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void deleteSelectedArea(void);
    void setTransformSelectionState(void);

public slots:
//    void clearImage();
//    void print();
    void updateDisplay(int changedImageNum);
    void updateCursor();
    void makeSelection(void);
    void strokeSelectedArea(void);
    void fillSelectedArea(void);
    void blackAndWhite(void);
    void gaussianBlur(void);
    void cannyEdge(void);
    void erodeFilter(void);
    void dilateFilter(void);
    void grabcutFilter(void);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent * event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    //    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    int totalImageNum, currentImageNum;
    QList<QImage> imageStackDisplay;
    QPoint imageCentralPoint;

    QMenu* penMenu;

    ToolType::toolType toolType;
    const int toolIndicationAlpha;
    QPolygonF marqueeHandlerControl;
    HoverPoints *marqueeHandler;
    QPolygonF lassoHandlerControl;
    HoverPoints *lassoHandler;
    //QPolygonF penHandlerControl;
    //HoverPoints *penHandler;
    HoverPoints *anchorPoint;

    BrushToolFunction *brushToolFunction;
    EraseToolFunction *eraseToolFunction;
    PenToolFunction *penToolFunction;

    QList<IplImage*> imageStackEdit;
//    QList<Mat> imageStack;
    QImage CVMatToQImage(const cv::Mat& imgMat);
    QImage IplImage2QImage(const IplImage *iplImage, double mini, double maxi);


    static const uchar TwoPointsSelection=0;
    static const uchar PolygonSelection=1;
    uchar selectionType;
    QPoint lastPoint;
    CvPoint vertexLeftTop, vertexRightBottom;  // ellipse selection also uses this
    QVector<CvPoint> irregularSelectionPoints;
    long int irregularSelectionPointNum;
    QColor fgColor, bgColor;

    bool modified;
    bool somethingSelected;
    bool isMouseMoving;
    bool isMousePressed;

private slots:
    void updateMarqueeHandlerControlPoints(QPolygonF);

//    void updatePenHandlerControlPoints(QPolygonF points) {
//        penHandlerControl=points;
//        penHandler->setPoints(points);
//    }

private:

    cv::Mat tmpImage;
    cv::Mat mask;

    void Ipl2Mat();
    void drawMask(int value=100);
    void Mat2Ipl();
    void readjustRect();
    void dragMoveSelectedArea();
    void rotateSelectedArea();



};

#endif
