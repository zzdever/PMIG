#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QBitmap>

#include "opencvprocess.h"

OpencvProcess::OpencvProcess(QWidget *parent)
    :QWidget(parent)
{
    currentImageNum=-1;
    toolType=ToolType::Brush;
    somethingSelected=false;

    brushToolFunction = new BrushToolFunction(this);
    eraseToolFunction = new EraseToolFunction(this);

}

bool OpencvProcess::openImage(const char*fileName)
{
//    Mat img = imread(fileName, CV_LOAD_IMAGE_COLOR);
//    if(img.data)
//    {
//        imageStack.append(img);
//        return true;
//    }
//    else
//    {
//        qDebug()<<"Unable to load image "<<fileName;
//        return false;
//    }
    IplImage *img = cvLoadImage(fileName);
    if(img)
    {
        imageStack.append(img);
        return true;
    }
    else
    {
        qDebug()<<"Unable to load image "<<fileName;
        return false;
    }
}

bool OpencvProcess::saveImage(const char *fileName, const char *fileFormat)
{
    return true;
}

void OpencvProcess::updateCursor()
{
    switch(toolType)
    {
    case ToolType::Marquee:
        parentWidget()->setCursor(QCursor(Qt::CrossCursor));
        break;
    case ToolType::Erase:
    {
        QPixmap cursorPixmap(eraseToolFunction->getEraseSize(), eraseToolFunction->getEraseSize());
        cursorPixmap.fill(Qt::white);
        parentWidget()->setCursor(QCursor(cursorPixmap));
    }
        break;
    default:
        parentWidget()->setCursor(QCursor(Qt::ArrowCursor));
        break;
    }
}

void OpencvProcess::setToolType(ToolType::toolType toolType)
{
    this->toolType = toolType;
}

void OpencvProcess::ApplyToolFunction(QPoint lastPoint, QPoint currentPoint)
{
    switch (toolType) {
    case ToolType::Brush:
        drawLineTo(lastPoint, currentPoint);
        break;
    default:
        break;
    }

    //ALL CHANGE MADE TO IMAGE MUST CALL THIS TO DISPALY
    emit updateDisplay(currentImageNum);
}

void OpencvProcess::ApplyToolFunction(QPoint currentPoint)
{
    switch(toolType){
    case ToolType::Erase:
        vertexA.x=currentPoint.rx() - eraseToolFunction->getEraseSize()/2;
        vertexA.y=currentPoint.ry() - eraseToolFunction->getEraseSize()/2;
        vertexB.x=vertexA.x + eraseToolFunction->getEraseSize();
        vertexB.y=vertexA.y + eraseToolFunction->getEraseSize();
        ApplyToolFunction();
        break;
    default:
        break;
    }

    //ALL CHANGE MADE TO IMAGE MUST CALL THIS TO DISPALY
    emit updateDisplay(currentImageNum);
}

void OpencvProcess::ApplyToolFunction()
{
    switch (toolType) {
    case ToolType::Erase:
        cvRectangle(imageStack[currentImageNum], vertexA, vertexB, CV_RGB(255,255,255), -1);
        break;
    default:
        break;
    }

    emit updateDisplay(currentImageNum);
}

void OpencvProcess::drawLineTo(QPoint lastPoint, QPoint currentPoint)
{

    int lineType = CV_AA; // change it to 8 to see non-antialiased graphics
    if(brushToolFunction->getAntiAliasing())
        lineType = CV_AA;
    else
        lineType = 8;
    //Mat image = Mat::zeros(height, width, CV_8UC3);
    //Mat image(imageStack[currentImageNum]);

    Point pt1, pt2;

    pt1.x=lastPoint.x();
    pt1.y=lastPoint.y();
    pt2.x=currentPoint.x();
    pt2.y=currentPoint.y();

    //line( image, pt1, pt2,  Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255), rng.uniform(1,10), lineType );
    cvLine(imageStack[currentImageNum], pt1, pt2, cvScalar(100,50,50,50), brushToolFunction->getBrushSize(), lineType);

    return;
}


