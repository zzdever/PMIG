#include <QMouseEvent>

#include "opencvprocess.h"

OpencvProcess::OpencvProcess(QWidget *parent)
    :QObject(parent)
{
    currentImageNum=-1;
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

//void OpencvProcess::setTotalImageNum(int num)
//{
//    totalImageNum = num;
//}

void OpencvProcess::setCurrentImageNum(int num)
{
    currentImageNum = num;
}




//void OpencvProcess::mousePressEvent(QMouseEvent *event)
//{
//    if (event->button() == Qt::LeftButton) {
//        lastPoint = event->pos();
//        isScribbling = true;
//    }
//}

//void OpencvProcess::mouseMoveEvent(QMouseEvent *event)
//{
//    if ((event->buttons() & Qt::LeftButton) && isScribbling)
//        drawLineTo(event->pos());
//}

//void OpencvProcess::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (event->button() == Qt::LeftButton && isScribbling)
//    {
//        drawLineTo(event->pos());
//        isScribbling = false;
//    }
//    Mat image(imageStack[currentImageNum]);
//    imshow("df",image);
//}

void OpencvProcess::ApplyToolFunction(ToolType::toolType toolType, int imageNum, QPoint lastPoint, QPoint currentPoint)
{
    switch (toolType) {
    case ToolType::Brush:
        drawLineTo(imageNum, lastPoint, currentPoint);
        break;
    default:
        break;
    }
}

void OpencvProcess::ApplyToolFunction(ToolType::toolType toolType, int imageNum, QPoint currentPoint)
{
    ;
}

void OpencvProcess::drawLineTo(int imageNum, QPoint lastPoint, QPoint currentPoint)
{

    int lineType = CV_AA; // change it to 8 to see non-antialiased graphics
    RNG rng(0xFFFFFFFF);
    //Mat image = Mat::zeros(height, width, CV_8UC3);
    //Mat image(imageStack[currentImageNum]);

    Point pt1, pt2;
//    pt1.x = rng.uniform(x1, x2);
//    pt1.y = rng.uniform(y1, y2);
//    pt2.x = rng.uniform(x1, x2);
//    pt2.y = rng.uniform(y1, y2);
    pt1.x=lastPoint.x();
    pt1.y=lastPoint.y();
    pt2.x=currentPoint.x();
    pt2.y=currentPoint.y();

    //int icolor =(unsigned)rng;

    //line( image, pt1, pt2,  Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255), rng.uniform(1,10), lineType );
    cvLine(imageStack[imageNum], pt1, pt2, cvScalar(100,50,50,50), brushToolFunction->getBrushSize(), lineType);

    //ALL CHANGE MADE TO IMAGE MUST CALL THIS TO DISPALY
    emit updateDisplay(imageNum);

    return;
}


