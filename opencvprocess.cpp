#include <QMouseEvent>

#include "opencvprocess.h"

OpencvProcess::OpencvProcess(QWidget *parent)
    :QObject(parent)
{
    currentImageNum=-1;
    toolType=ToolType::Brush;
    somethingSelected=false;

    brushToolFunction = new BrushToolFunction(parent);
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


void OpencvProcess::ApplyToolFunction(QPoint lastPoint, QPoint currentPoint)
{
    switch (toolType) {
    case ToolType::Brush:
        drawLineTo(lastPoint, currentPoint);
        break;
    default:
        break;
    }
}

void OpencvProcess::ApplyToolFunction(QPoint currentPoint)
{
    ;
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

    //ALL CHANGE MADE TO IMAGE MUST CALL THIS TO DISPALY
    emit updateDisplay(currentImageNum);

    return;
}


