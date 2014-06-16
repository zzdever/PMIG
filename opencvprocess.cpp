#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QBitmap>

//#include "opencvprocess.h"
#include "scribblearea.h"

//OpencvProcess::OpencvProcess(QWidget *parent)
//    :QWidget(parent)
//{
//    currentImageNum=-1;
//    toolType=ToolType::Brush;
//    somethingSelected=false;

//    brushToolFunction = new BrushToolFunction(this);
//    eraseToolFunction = new EraseToolFunction(this);

//}

//bool ScribbleArea::openImage(const char*fileName)
//{
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
//    IplImage *img = cvLoadImage(fileName);
//    if(img)
//    {
//        imageStack.append(img);
//        return true;
//    }
//    else
//    {
//        qDebug()<<"Unable to load image "<<fileName;
//        return false;
//    }
//}

//! [1]
bool ScribbleArea::openImage(const QString &fileName)
//! [1] //! [2]
{
    if(totalImageNum == 1)
    {
        QMessageBox *tmpMessage = new QMessageBox(this);
        tmpMessage->setText("Sorry, currently only one layer is supported.");
        tmpMessage->show();
        return false;
    }

    IplImage *img = cvLoadImage(&(fileName.toStdString()[0]));
    if(img)
    {
        imageStackEdit.append(img);

        totalImageNum++;
        currentImageNum = totalImageNum-1;
        updateDisplay(currentImageNum);
        return true;
    }
    else
    {
        qDebug()<<"Unable to load image "<<fileName;
        return false;
    }



//    QSize newSize = loadedImage.size().expandedTo(size());
//    resizeImage(&loadedImage, newSize);
//    image = loadedImage;
//    modified = false;
//    update();
//    return true;
}
//! [2]

//! [3]
//bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
//{
//    QImage visibleImage = image;
//    resizeImage(&visibleImage, size());
//return true;
    //if(saveImage(&(fileName.toStdString()[0]), fileFormat)){
//    if (visibleImage.save(fileName, fileFormat)) {
//        modified = false;
//        return true;
//    } else {
//        return false;
//    }
//}
//! [4]

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    return true;
}



//void ScribbleArea::setToolType(ToolType::toolType toolType)
//{
//    this->toolType = toolType;
//}

void ScribbleArea::ApplyToolFunction(QPoint lastPoint, QPoint currentPoint)
{
    switch (toolType) {
    case ToolType::Brush:
    case ToolType::Erase:
        drawLineTo(lastPoint, currentPoint);
        break;
    case ToolType::Transform:{
        //Pay attetion that vertexLeftTop may be not LeftTop
        //and vertexRightBottom may be not RightBottom
        if(lastPoint.x()>vertexLeftTop.x+5 && lastPoint.x()<vertexRightBottom.x-5
                && lastPoint.y()>vertexLeftTop.y+5 && lastPoint.y()<vertexRightBottom.y-5){
            qDebug()<<"In, move, form ("<<lastPoint.x()<<lastPoint.y()<<") to ("<<currentPoint.x()<<currentPoint.y()<<")";

            for(QVector<CvPoint>::iterator iter=irregularSelectionPoints.begin();iter!=irregularSelectionPoints.end();iter++){
                (*iter).x+=currentPoint.x()-lastPoint.x();
                (*iter).y+=currentPoint.y()-lastPoint.y();
            }
        }
        else if(lastPoint.x()<vertexLeftTop.x-5 || lastPoint.x()>vertexRightBottom.x+5
                || lastPoint.y()<vertexLeftTop.y-5 || lastPoint.y()>vertexRightBottom.y+5)
            qDebug()<<"Out, rotate, form ("<<lastPoint.x()<<lastPoint.y()<<") to ("<<currentPoint.x()<<currentPoint.y()<<")";
        else{
            qDebug()<<"On, transform, form ("<<lastPoint.x()<<lastPoint.y()<<") to ("<<currentPoint.x()<<currentPoint.y()<<")";

            for(QVector<CvPoint>::iterator iter=irregularSelectionPoints.begin();iter!=irregularSelectionPoints.end();iter++){
                (*iter).x+=currentPoint.x()-lastPoint.x();
                (*iter).y+=currentPoint.y()-lastPoint.y();
            }
        }

        setTransformSelectionState();

        break;
    }
    default:
        break;
    }

    //ALL CHANGE MADE TO IMAGE MUST CALL THIS TO DISPALY
    updateDisplay(currentImageNum);
}

void ScribbleArea::ApplyToolFunction(QPoint currentPoint)
{
    switch(toolType){
    case ToolType::Erase:
        vertexLeftTop.x=currentPoint.rx() - eraseToolFunction->getEraseSize()/2;
        vertexLeftTop.y=currentPoint.ry() - eraseToolFunction->getEraseSize()/2;
        vertexRightBottom.x=vertexLeftTop.x + eraseToolFunction->getEraseSize();
        vertexRightBottom.y=vertexLeftTop.y + eraseToolFunction->getEraseSize();
        ApplyToolFunction();
        break;
    default:
        break;
    }

    //ALL CHANGE MADE TO IMAGE MUST CALL THIS TO DISPALY
    //emit
    updateDisplay(currentImageNum);
}

void ScribbleArea::ApplyToolFunction()
{
    switch (toolType) {

    default:
        break;
    }

    updateDisplay(currentImageNum);
}

void ScribbleArea::drawLineTo(QPoint lastPoint, QPoint currentPoint)
{
    //Mat image = Mat::zeros(height, width, CV_8UC3);
    //Mat image(imageStack[currentImageNum]);

    cv::Point pt1, pt2;

    pt1.x=lastPoint.x();
    pt1.y=lastPoint.y();
    pt2.x=currentPoint.x();
    pt2.y=currentPoint.y();

    QColor color;
    int size;
    int lineType; // change it to 8 to see non-antialiased graphics
    switch(toolType)
    {
    case ToolType::Brush:{
        color=fgColor;
        size=brushToolFunction->getBrushSize();
        if(brushToolFunction->getAntiAliasing())
            lineType = CV_AA;
        else
            lineType = 8;
        break;
    }
    case ToolType::Erase:{
        color=bgColor;
        size=eraseToolFunction->getEraseSize();
        lineType = CV_AA;
        break;
    }
    default:{
        color=fgColor;
        size=5;
        break;
    }
    }


    //line( image, pt1, pt2,  Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255), rng.uniform(1,10), lineType );
    cvLine(imageStackEdit[currentImageNum], pt1, pt2,
           cvScalar(color.blue(),color.green(), color.red(), color.alpha()), size, lineType);
    /// cvScalar(b, g, r, a);
    return;
}

void ScribbleArea::deleteSelectedArea()
{
    if(somethingSelected == false) {return;}

    switch (selectionType){
    case TwoPointsSelection:
        cvRectangle(imageStackEdit[currentImageNum], vertexLeftTop, vertexRightBottom, CV_RGB(255,255,255), -1);
        break;
    case PolygonSelection:
        foreach(CvPoint tmp, irregularSelectionPoints){
            qDebug()<<"("<<tmp.x<<","<<tmp.y<<")";
        }

        break;
    }

    updateDisplay(currentImageNum);
}

void ScribbleArea::strokeSelectedArea(void)
{
    if(somethingSelected == false) return;

    //test
    foreach(CvPoint tmp, irregularSelectionPoints){
        qDebug()<<"("<<tmp.x<<","<<tmp.y<<")";
    }
    //end test

    updateDisplay(currentImageNum);
}

void ScribbleArea::fillSelectedArea(void)
{
    if(somethingSelected == false) return;

    //test
    foreach(CvPoint tmp, irregularSelectionPoints){
        qDebug()<<"("<<tmp.x<<","<<tmp.y<<")";
    }
    //end test

    updateDisplay(currentImageNum);
}

void ScribbleArea::blackAndWhite(void)
{
    //test
    qDebug()<<"balck and white";
    //test

    if(somethingSelected == false) return;

    updateDisplay(currentImageNum);
}
