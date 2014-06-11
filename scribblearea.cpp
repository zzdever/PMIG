﻿#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#endif

#include "scribblearea.h"

//! [0]
ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent),
      opencvProcess(new OpencvProcess(this))
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    isMouseMoving = false;
    isMousePressed = false;
    toolType = ToolType::Brush;

    totalImageNum = 0;
    currentImageNum = -1;
    connect(opencvProcess, &OpencvProcess::updateDisplay, this, &ScribbleArea::updateDisplay);

    imageCentralPoint.setX(this->width()/2);
    imageCentralPoint.setY(this->height()/2);

}
//! [0]

//! [1]
bool ScribbleArea::openImage(const QString &fileName)
//! [1] //! [2]
{
//    if (!loadedImage.load(fileName))
//        return false;
    if(opencvProcess->openImage(&(fileName.toStdString()[0])))
    {
        totalImageNum++;
        currentImageNum = totalImageNum-1;
        opencvProcess->setCurrentImageNum(currentImageNum);
        updateDisplay(currentImageNum);
        return true;
    }
    return false;

//    QSize newSize = loadedImage.size().expandedTo(size());
//    resizeImage(&loadedImage, newSize);
//    image = loadedImage;
//    modified = false;
//    update();
//    return true;
}
//! [2]

//! [3]
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
{
    QImage visibleImage = image;
//    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}
//! [4]

////! [5]
//void ScribbleArea::setPenColor(const QColor &newColor)
////! [5] //! [6]
//{
//    myPenColor = newColor;
//}
////! [6]

////! [7]
//void ScribbleArea::setPenWidth(int newWidth)
////! [7] //! [8]
//{
//    myPenWidth = newWidth;
//}
////! [8]

////! [9]
//void ScribbleArea::clearImage()
////! [9] //! [10]
//{
//    image.fill(qRgb(255, 255, 255));
//    modified = true;
//    update();
//}
////! [10]

//! [11]
void ScribbleArea::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    //opencvProcess->mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        isMousePressed = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isMousePressed)
    {
        isMouseMoving = true;
        opencvProcess->ApplyToolFunction(toolType, currentImageNum,
            QPoint(lastPoint.x()-(imageCentralPoint.x()-imageStack[currentImageNum].width()/2),
               lastPoint.y()-(imageCentralPoint.y()-imageStack[currentImageNum].height()/2)),
            QPoint(event->pos().x()-(imageCentralPoint.x()-imageStack[currentImageNum].width()/2),
                         event->pos().y()-(imageCentralPoint.y()-imageStack[currentImageNum].height()/2)));
        lastPoint = event->pos();
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    isMousePressed = false;

    if (event->button() == Qt::LeftButton && isMouseMoving) {
        opencvProcess->ApplyToolFunction(toolType, currentImageNum,
            QPoint(lastPoint.x()-(imageCentralPoint.x()-imageStack[currentImageNum].width()/2),
               lastPoint.y()-(imageCentralPoint.y()-imageStack[currentImageNum].height()/2)),
            QPoint(event->pos().x()-(imageCentralPoint.x()-imageStack[currentImageNum].width()/2),
                         event->pos().y()-(imageCentralPoint.y()-imageStack[currentImageNum].height()/2)));
        isMouseMoving = false;
    }
    else {
        opencvProcess->ApplyToolFunction(toolType, currentImageNum,
            QPoint(event->pos().x()-(imageCentralPoint.x()-imageStack[currentImageNum].width()/2),
                         event->pos().y()-(imageCentralPoint.y()-imageStack[currentImageNum].height()/2)));
    }
}

void ScribbleArea::updateDisplay(int changedImageNum)
{
    if(changedImageNum > imageStack.size())
    {
        qDebug()<<"Out of bound, no such image opened";
        return;
    }

    if(changedImageNum == imageStack.size())
    {
        QImage newImage;
        newImage = IplImage2QImage(
                    opencvProcess->imageStack[changedImageNum], 0, 1000);
//        newImage = CVMatToQImage(opencvProcess->imageStack[changedImageNum]);
        imageStack.append(newImage);

        //resizeImage(&imageStack[0], QSize(imageStack[0].width()/2, imageStack[0].height()/2));
    }
    else
    {
        imageStack[changedImageNum] = IplImage2QImage(
                    opencvProcess->imageStack[changedImageNum], 0, 1000);
//        imageStack[changedImageNum] = CVMatToQImage(opencvProcess->imageStack[changedImageNum]);
    }

    update();
}

//! [12] //! [13]
void ScribbleArea::paintEvent(QPaintEvent *event)
//! [13] //! [14]
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
    if(imageStack.size()>0)
    {
        for(int i=0; i<imageStack.size(); i++)
        {
            painter.drawImage(imageCentralPoint.x()-imageStack[i].width()/2,
                imageCentralPoint.y()-imageStack[i].height()/2,
                imageStack[i]);
        }
    }
}
//! [14]

//! [15]
void ScribbleArea::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
//        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    imageCentralPoint.setX(this->width()/2);
    imageCentralPoint.setY(this->height()/2);
    QWidget::resizeEvent(event);
}
//! [16]

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
void ScribbleArea::print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
//! [21] //! [22]
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_NO_PRINTER
}
//! [22]


QImage ScribbleArea::CVMatToQImage(const Mat& imgMat)
{
    Mat rgb;
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