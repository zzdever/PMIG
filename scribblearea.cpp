﻿#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#endif

#include "scribblearea.h"

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(totalImageNum <= 0) return;
    if (event->button() == Qt::LeftButton) {
        isMousePressed = true;

        int eventX=event->pos().x()-(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2);
        int eventY=event->pos().y()-(imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2);

        switch(toolType)
        {
        case ToolType::Marquee:
        case ToolType::Lasso:{
            somethingSelected=false;
            marqueeHandlerControl.clear();
            marqueeHandler->setPoints(marqueeHandlerControl);
            lassoHandlerControl.clear();
            lassoHandler->setPoints(lassoHandlerControl);

            switch(toolType){
            case ToolType::Marquee:
                vertexLeftTop.x=eventX;
                vertexLeftTop.y=eventY;
                marqueeHandlerControl << QPointF(event->pos().x(), event->pos().y())
                           << QPointF(event->pos().x(), event->pos().y())
                           << QPointF(event->pos().x(), event->pos().y())
                           << QPointF(event->pos().x(), event->pos().y());
                break;
            case ToolType::Lasso:
                irregularSelectionPoints.append({eventX, eventY});
                irregularSelectionPointNum++;
                lassoHandlerControl<<QPointF(event->pos().x(), event->pos().y());
                lassoHandler->setPoints(lassoHandlerControl);
                break;
            default:break;
            }



            break;
        }
        case ToolType::Transform:

            break;
        default: break;
        }

        lastPoint = event->pos();
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if(totalImageNum <= 0) return;
    if ((event->buttons() & Qt::LeftButton) && isMousePressed){
        isMouseMoving = true;

        int eventX=event->pos().x()-(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2);
        int eventY=event->pos().y()-(imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2);
        int lastX=lastPoint.x()-(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2);
        int lastY=lastPoint.y()-(imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2);

        switch(toolType)
        {
        case ToolType::Brush:
        case ToolType::Erase:
            break;
        case ToolType::Marquee:{
            vertexRightBottom.x=eventX;
            vertexRightBottom.y=eventY;

            QPointF tmpOriginPoint(marqueeHandlerControl.at(0));
            marqueeHandlerControl.clear();
            marqueeHandlerControl << tmpOriginPoint
                       << QPointF(event->pos().x(), tmpOriginPoint.ry())
                       << QPointF(event->pos().x(), event->pos().y())
                       << QPointF(tmpOriginPoint.rx(), event->pos().y());
            marqueeHandler->setPoints(marqueeHandlerControl);

            update();
            break;
        }
        case ToolType::Lasso:{
            irregularSelectionPoints.append({eventX, eventY});
            irregularSelectionPointNum++;
            lassoHandlerControl<<QPointF(event->pos().x(), event->pos().y());
            lassoHandler->setPoints(lassoHandlerControl);
        }
//        case ToolType::Erase:
//            ApplyToolFunction(QPoint(eventX,eventY));
//            break;

        default:
            break;
        }

        ApplyToolFunction(QPoint(lastX,lastY), QPoint(eventX,eventY));
        lastPoint = event->pos();

    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(totalImageNum <= 0) return;  
    if(isMousePressed==false) return;
    isMousePressed = false;

    if (event->button() == Qt::LeftButton && isMouseMoving) {
        isMouseMoving = false;

        int eventX=event->pos().x()-(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2);
        int eventY=event->pos().y()-(imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2);
        int lastX=lastPoint.x()-(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2);
        int lastY=lastPoint.y()-(imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2);

        switch(toolType)
        {
        case ToolType::Brush:
        case ToolType::Erase:
            break;
        case ToolType::Marquee:{
            somethingSelected=true;

            QPointF tmpOriginPoint(marqueeHandlerControl.at(0));
            marqueeHandlerControl.clear();
            marqueeHandlerControl << tmpOriginPoint
                       << QPointF(event->pos().x(), tmpOriginPoint.ry())
                       << QPointF(event->pos().x(), event->pos().y())
                       << QPointF(tmpOriginPoint.rx(), event->pos().y());
            marqueeHandler->setPoints(marqueeHandlerControl);
            update();
            break;
        }
        case ToolType::Lasso:{
            irregularSelectionPoints.append({eventX, eventY});
            irregularSelectionPointNum++;
            lassoHandlerControl<<QPointF(event->pos().x(), event->pos().y());
            lassoHandler->setPoints(lassoHandlerControl);
            lassoHandler->setCloseType(HoverPoints::Close);
            somethingSelected=true;
        }
        default:
            break;
        }

        ApplyToolFunction(QPoint(lastX,lastY), QPoint(eventX,eventY));

    }
    else {
        int eventX=event->pos().x()-(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2);
        int eventY=event->pos().y()-(imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2);

        switch(toolType)
        {
        case ToolType::Brush:
        case ToolType::Erase:
            break;
        case ToolType::Marquee:{
            somethingSelected=false;
            marqueeHandlerControl.clear();
            marqueeHandler->setPoints(marqueeHandlerControl);
            update();
            break;
        }
        case ToolType::Lasso:{
            somethingSelected=false;
            lassoHandlerControl.clear();
            lassoHandler->setPoints(lassoHandlerControl);
            update();
            break;
        }
//        case ToolType::Erase:
//            break;

        default:
            break;
        }

        ApplyToolFunction(QPoint(eventX,eventY));
    }
}





void ScribbleArea::updateDisplay(int changedImageNum)
{
    if(changedImageNum > imageStackDisplay.size())
    {
        qDebug()<<"Out of bound, no such image opened";
        return;
    }

    if(changedImageNum == imageStackDisplay.size())
    {
        QImage newImage;
        newImage = IplImage2QImage(
                    imageStackEdit[changedImageNum], 0, 1000);
//        newImage = CVMatToQImage(opencvProcess->imageStack[changedImageNum]);
        imageStackDisplay.append(newImage);

        //resizeImage(&imageStack[0], QSize(imageStack[0].width()/2, imageStack[0].height()/2));
    }
    else
    {
        imageStackDisplay[changedImageNum] = IplImage2QImage(
                    imageStackEdit[changedImageNum], 0, 1000);
//        imageStack[changedImageNum] = CVMatToQImage(opencvProcess->imageStack[changedImageNum]);
        modified=true;
    }

    update();
}

//! [12] //! [13]
void ScribbleArea::paintEvent(QPaintEvent *event)
//! [13] //! [14]
{
    QPainter painter(this);
//    QRect dirtyRect = event->rect();
//    painter.drawImage(dirtyRect, image, dirtyRect);

    /// @note can be optimized, update one small area one time
    if(imageStackDisplay.size()>0)
    {
        for(int i=0; i<imageStackDisplay.size(); i++)
        {
            painter.drawImage(imageCentralPoint.x()-imageStackDisplay[i].width()/2,
                imageCentralPoint.y()-imageStackDisplay[i].height()/2,
                imageStackDisplay[i]);
        }
    }
}
//! [14]

//! [15]
void ScribbleArea::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
//    if (width() > image.width() || height() > image.height()) {
//        int newWidth = qMax(width() + 128, image.width());
//        int newHeight = qMax(height() + 128, image.height());
//        resizeImage(&image, QSize(newWidth, newHeight));
//        update();
//    }
    imageCentralPoint.setX(this->width()/2);
    imageCentralPoint.setY(this->height()/2);
    QWidget::resizeEvent(event);
}

void ScribbleArea::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Delete))
    {
        if(somethingSelected == false) return;
        deleteSelectedArea();
    }

    if(event->matches(QKeySequence::SelectAll))
    {
        selectAll();
    }
}

void ScribbleArea::enterEvent(QEvent * event)
{
    if(totalImageNum>0 && event->type() == QEvent::Enter){
        updateCursor();
    }

}

void ScribbleArea::updateCursor()
{
    switch(toolType)
    {
    case ToolType::Marquee:
        setCursor(QCursor(Qt::CrossCursor));
        break;
    case ToolType::Erase:
    {
        QPixmap cursorPixmap(eraseToolFunction->getEraseSize(), eraseToolFunction->getEraseSize());
        cursorPixmap.fill(Qt::white);
        setCursor(QCursor(cursorPixmap));
    }
        break;
    default:
        setCursor(QCursor(Qt::ArrowCursor));
        break;
    }
}


void ScribbleArea::setToolType(ToolType::toolType type)
{
    toolType=type;
    if(totalImageNum>0) updateCursor();
    return;
}


void ScribbleArea::selectAll(void)
{
    if(totalImageNum <= 0) return;
    marqueeHandlerControl.clear();
    marqueeHandlerControl << QPointF(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2,
                                     imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2)
               << QPointF(imageCentralPoint.x()+imageStackDisplay[currentImageNum].width()/2,
                          imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2)
               << QPointF(imageCentralPoint.x()+imageStackDisplay[currentImageNum].width()/2,
                          imageCentralPoint.y()+imageStackDisplay[currentImageNum].height()/2)
               << QPointF(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2,
                          imageCentralPoint.y()+imageStackDisplay[currentImageNum].height()/2);
    marqueeHandler->setPoints(marqueeHandlerControl);
    somethingSelected = true;
    update();
}


//!
//! [0]
ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent),
      toolIndicationAlpha(150)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    isMouseMoving = false;
    isMousePressed = false;
    toolType = ToolType::Brush;
    somethingSelected=false;
    irregularSelectionPoints.clear();
    irregularSelectionPointNum=0;

    brushToolFunction = new BrushToolFunction(this);
    eraseToolFunction = new EraseToolFunction(this);
    fgColor=Qt::black;
    bgColor=Qt::white;

    totalImageNum = 0;
    currentImageNum = -1;
    //connect(opencvProcess, &OpencvProcess::updateDisplay, this, &ScribbleArea::updateDisplay);

    imageCentralPoint.setX(this->width()/2);
    imageCentralPoint.setY(this->height()/2);

    marqueeHandler = new HoverPoints(this, HoverPoints::RectangleShape);
    marqueeHandler->setConnectionType(HoverPoints::LineConnection);
    marqueeHandler->setCloseType(HoverPoints::Close);
    marqueeHandler->setEditable(false);
    marqueeHandler->setPointSize(QSize(10, 10));
    marqueeHandler->setShapeBrush(QBrush(QColor(0, 0, 0, toolIndicationAlpha)));
    marqueeHandler->setShapePen(QPen(QColor(0, 0, 0, toolIndicationAlpha)));
    marqueeHandler->setConnectionPen(QPen(QColor(0, 0, 0, toolIndicationAlpha)));
    //marqueeHandler->setBoundingRect(QRectF(0, 0, 500, 500));
    connect(marqueeHandler, SIGNAL(pointsChanged(QPolygonF)),this, SLOT(updateMarqueeHandlerControlPoints(QPolygonF)));

    lassoHandler = new HoverPoints(this, HoverPoints::CircleShape);
    lassoHandler->setConnectionType(HoverPoints::LineConnection);
    lassoHandler->setCloseType(HoverPoints::NoClose);
    lassoHandler->setEditable(false);
    lassoHandler->setPointSize(QSize(1, 1));
    lassoHandler->setShapeBrush(QBrush(QColor(0, 0, 0, 255)));
    lassoHandler->setShapePen(QPen(QColor(0, 0, 0, 255)));
    lassoHandler->setConnectionPen(QPen(QColor(0, 0, 0, 255)));
    //setMouseTracking(true);

}
//! [0]

void ScribbleArea::updateMarqueeHandlerControlPoints(QPolygonF newControlPoints)
{return;
    if(newControlPoints.size()<4) return;
    QPointF trans;

    if(!(trans=(newControlPoints.at(0)-marqueeHandlerControl.at(0))).isNull()){
        newControlPoints[1].ry() += trans.ry();
        newControlPoints[3].rx() += trans.rx();
    }
    else if(!(trans=(newControlPoints.at(1)-marqueeHandlerControl.at(1))).isNull()){
        newControlPoints[0].ry() += trans.ry();
        newControlPoints[2].rx() += trans.rx();
    }
    else if(!(trans=(newControlPoints.at(2)-marqueeHandlerControl.at(2))).isNull()){
        newControlPoints[3].ry() += trans.ry();
        newControlPoints[1].rx() += trans.rx();
    }
    else if(!(trans=(newControlPoints.at(3)-marqueeHandlerControl.at(3))).isNull()){
        newControlPoints[2].ry() += trans.ry();
        newControlPoints[0].rx() += trans.rx();
    }

    marqueeHandler->setPoints(newControlPoints);
    marqueeHandlerControl = newControlPoints;
    update();
    vertexLeftTop.x=marqueeHandlerControl[0].rx()-(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2);
    vertexLeftTop.y=marqueeHandlerControl[0].ry()-(imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2);
    vertexRightBottom.x=marqueeHandlerControl[2].rx()-(imageCentralPoint.x()-imageStackDisplay[currentImageNum].width()/2);
    vertexRightBottom.y=marqueeHandlerControl[2].ry()-(imageCentralPoint.y()-imageStackDisplay[currentImageNum].height()/2);

    return;
}

////! [9]
//void ScribbleArea::clearImage()
////! [9] //! [10]
//{
//    image.fill(qRgb(255, 255, 255));
//    modified = true;
//    update();
//}
////! [10]


////! [17]
//void ScribbleArea::drawLineTo(const QPoint &endPoint)
////! [17] //! [18]
//{
//    QPainter painter(&image);
//    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
//                        Qt::RoundJoin));
//    painter.setRenderHints(QPainter::Antialiasing);
//    painter.drawLine(lastPoint, endPoint);
//    modified = true;

//    int rad = (myPenWidth / 2) + 2;
//    update(QRect(lastPoint, endPoint).normalized()
//                                     .adjusted(-rad, -rad, +rad, +rad));
//    lastPoint = endPoint;
//}
////! [18]

////! [19]
//void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
////! [19] //! [20]
//{
//    if (image->size() == newSize)
//        return;

//    QImage newImage(newSize, QImage::Format_RGB32);
//    newImage.fill(qRgb(255, 255, 255));
//    QPainter painter(&newImage);
//    painter.drawImage(QPoint(0, 0), *image);
//    *image = newImage;
//}
////! [20]

//! [21]
//void ScribbleArea::print()
//{
//#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
//    QPrinter printer(QPrinter::HighResolution);

//    QPrintDialog printDialog(&printer, this);
////! [21] //! [22]
//    if (printDialog.exec() == QDialog::Accepted) {
//        QPainter painter(&printer);
//        QRect rect = painter.viewport();
//        QSize size = image.size();
//        size.scale(rect.size(), Qt::KeepAspectRatio);
//        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//        painter.setWindow(image.rect());
//        painter.drawImage(0, 0, image);
//    }
//#endif // QT_NO_PRINTER
//}
//! [22]


QImage ScribbleArea::CVMatToQImage(const cv::Mat& imgMat)
{
    cv::Mat rgb;
    cvtColor(imgMat, rgb, CV_BGR2RGB);

    return QImage((const unsigned char*)rgb.data, rgb.cols, rgb.rows, QImage::Format_RGB888);
}

QImage ScribbleArea::IplImage2QImage(const IplImage *iplImage, double mini, double maxi)
{
    uchar *qImageBuffer = NULL;

    int width = iplImage->width;

    /* Note here that OpenCV image is stored so that each lined is
    32-bits aligned thus
    * explaining the necessity to "skip" the few last bytes of each
    line of OpenCV image buffer.
    */
    int widthStep = iplImage->widthStep;
    int height = iplImage->height;

    switch (iplImage->depth)
    {
    case IPL_DEPTH_8U:
        if(iplImage->nChannels == 1)
        {
            /* OpenCV image is stored with one byte grey pixel. We convert it
                to an 8 bit depth QImage.
                */

            qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const uchar *iplImagePtr = (const uchar *) iplImage->imageData;

            for(int y = 0; y < height; y++)
            {
                // Copy line by line
                memcpy(QImagePtr, iplImagePtr, width);
                QImagePtr += width;
                iplImagePtr += widthStep;
            }

        }
        else if(iplImage->nChannels == 3)
        {
            /* OpenCV image is stored with 3 byte color pixels (3 channels).
                        We convert it to a 32 bit depth QImage.
                        */
            qImageBuffer = (uchar *) malloc(width*height*4*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const uchar *iplImagePtr = (const uchar *) iplImage->imageData;
            for(int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    // We cannot help but copy manually.
                    QImagePtr[0] = iplImagePtr[0];
                    QImagePtr[1] = iplImagePtr[1];
                    QImagePtr[2] = iplImagePtr[2];
                    QImagePtr[3] = 0;

                    QImagePtr += 4;
                    iplImagePtr += 3;
                }
                iplImagePtr += widthStep-3*width;
            }

        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=8U and %d channels ", iplImage->nChannels);
        }
        break;
                case IPL_DEPTH_16U:
        if(iplImage->nChannels == 1)
        {
            /* OpenCV image is stored with 2 bytes grey pixel. We convert it
                to an 8 bit depth QImage.
                */
            qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            //const uint16_t *iplImagePtr = (const uint16_t *);
            const unsigned int *iplImagePtr = (const unsigned int *)iplImage->imageData;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    // We take only the highest part of the 16 bit value. It is
                    //similar to dividing by 256.
                    *QImagePtr++ = ((*iplImagePtr++) >> 8);
                }
                iplImagePtr += widthStep/sizeof(unsigned int)-width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=16U and %d channels ", iplImage->nChannels);

        }
        break;
                case IPL_DEPTH_32F:
        if(iplImage->nChannels == 1)
        {
            /* OpenCV image is stored with float (4 bytes) grey pixel. We
                convert it to an 8 bit depth QImage.
                */
            qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const float *iplImagePtr = (const float *) iplImage->imageData;
            for(int y = 0; y < height; y++)
            {
                for(int x = 0; x < width; x++)
                {
                    uchar p;
                    float pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);
                    if(pf < 0) p = 0;
                    else if(pf > 255) p = 255;
                    else p = (uchar) pf;

                    *QImagePtr++ = p;
                }
                iplImagePtr += widthStep/sizeof(float)-width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=32F and %d channels ", iplImage->nChannels);
        }
        break;
                   case IPL_DEPTH_64F:
        if(iplImage->nChannels == 1)
        {
            /* OpenCV image is stored with double (8 bytes) grey pixel. We
                    convert it to an 8 bit depth QImage.
                    */
            qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
            uchar *QImagePtr = qImageBuffer;
            const double *iplImagePtr = (const double *) iplImage->imageData;
            for(int y = 0; y < height; y++)
            {
                for(int x = 0; x < width; x++)
                {
                    uchar p;
                    double pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);

                    if(pf < 0) p = 0;
                    else if(pf > 255) p = 255;
                    else p = (uchar) pf;

                    *QImagePtr++ = p;
                }
                iplImagePtr += widthStep/sizeof(double)-width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=64F and %d channels ", iplImage->nChannels);
        }
        break;
                default:
        qDebug("IplImageToQImage: image format is not supported : depth=%d and %d channels ", iplImage->depth, iplImage->nChannels);
    }

    QImage qImage;
    QVector<QRgb> vcolorTable;
    if(iplImage->nChannels == 1)
    {
        // We should check who is going to destroy this allocation.
        QRgb *colorTable = new QRgb[256];
        for(int i = 0; i < 256; i++)
        {
            colorTable[i] = qRgb(i, i, i);
            vcolorTable[i] = colorTable[i];
        }
        qImage = QImage(qImageBuffer, width, height, QImage::Format_Indexed8).copy();
        qImage.setColorTable(vcolorTable);
    }
    else
    {
        qImage = QImage(qImageBuffer, width, height, QImage::Format_RGB32).copy();
    }
    free(qImageBuffer);
    return qImage;
}
