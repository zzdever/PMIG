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

    IplImage *img = cvLoadImage(fileName.toStdString().c_str());
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
    if(totalImageNum<=0) return;

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
    if(totalImageNum<=0) return;

    if(somethingSelected == false) return;

//    foreach(CvPoint tmp, irregularSelectionPoints){
//        qDebug()<<"("<<tmp.x<<","<<tmp.y<<")";
//    }
    int size=3;
    int lineType=CV_AA;

    for(int i=0;i<irregularSelectionPoints.length()-1;i++){
        cvLine(imageStackEdit[currentImageNum], irregularSelectionPoints[i], irregularSelectionPoints[i+1],
               cvScalar(fgColor.blue(),fgColor.green(), fgColor.red(), fgColor.alpha()), size, lineType);
    }
    cvLine(imageStackEdit[currentImageNum], irregularSelectionPoints[irregularSelectionPoints.length()-1],
            irregularSelectionPoints[0],cvScalar(fgColor.blue(),fgColor.green(), fgColor.red(), fgColor.alpha()), size, lineType);

    updateDisplay(currentImageNum);
}

void ScribbleArea::fillSelectedArea(void)
{
    if(totalImageNum<=0) return;

    if(somethingSelected == false) return;

//    foreach(CvPoint tmp, irregularSelectionPoints){
//        qDebug()<<"("<<tmp.x<<","<<tmp.y<<")";
//    }
    Ipl2Mat();
    drawMask();
    tmpImage.setTo(cv::Scalar(fgColor.blue(),fgColor.green(),fgColor.red()));
    Mat2Ipl();

    updateDisplay(currentImageNum);
}




void ScribbleArea::blackAndWhite(void){
    if(totalImageNum<=0) return;

    if(somethingSelected == false) return;

    Ipl2Mat();
    drawMask();
    cv::cvtColor(tmpImage,tmpImage,CV_BGR2GRAY);
    cv::cvtColor(tmpImage,tmpImage,CV_GRAY2BGR);
    Mat2Ipl();

    updateDisplay(currentImageNum);
}

void ScribbleArea::gaussianBlur(void){
    if(totalImageNum<=0) return;

    Ipl2Mat();
    drawMask();

    QDialog getSizeDialog(this);
    getSizeDialog.setWindowTitle("Gaussian Blur");

    QGridLayout *dialogLayout = new QGridLayout(&getSizeDialog);
    dialogLayout->addWidget(new QLabel(tr("Input size: "), &getSizeDialog),0, 0);
    QSpinBox *sizeInputBox = new QSpinBox;
    sizeInputBox->setValue(5);    /// @note Value step need to be set
    sizeInputBox->setRange(0,100);   /// @note Range need to be set
    dialogLayout->addWidget(sizeInputBox,0,1);

    QPushButton *okButton = new QPushButton(tr("Ok"), &getSizeDialog);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), &getSizeDialog);
    connect(okButton, SIGNAL(clicked()), &getSizeDialog, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), &getSizeDialog, SLOT(reject()));
    okButton->setDefault(true);
    dialogLayout->addWidget(cancelButton);
    dialogLayout->addWidget(okButton);

    if(!getSizeDialog.exec()) return;

    int size=sizeInputBox->value();

    cv::GaussianBlur( tmpImage, tmpImage, cv::Size( size, size ), 0, 0 );
    Mat2Ipl();

    updateDisplay(currentImageNum);
}

void ScribbleArea::cannyEdge(void){
    if(totalImageNum<=0) return;

    Ipl2Mat();
    drawMask();

    QDialog getSizeDialog(this);
    getSizeDialog.setWindowTitle("Canny Edge");

    QGridLayout *dialogLayout = new QGridLayout(&getSizeDialog);
    dialogLayout->addWidget(new QLabel(tr("Input size: "), &getSizeDialog),0, 0);
    QSpinBox *sizeInputBox = new QSpinBox;
    sizeInputBox->setValue(50);
    sizeInputBox->setRange(0,100);  /// @note Range need to be set
    dialogLayout->addWidget(sizeInputBox,0,1);

    QPushButton *okButton = new QPushButton(tr("Ok"), &getSizeDialog);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), &getSizeDialog);
    connect(okButton, SIGNAL(clicked()), &getSizeDialog, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), &getSizeDialog, SLOT(reject()));
    okButton->setDefault(true);
    dialogLayout->addWidget(cancelButton);
    dialogLayout->addWidget(okButton);

    if(!getSizeDialog.exec()) return;

    int threshold=sizeInputBox->value();

    cv::Canny(tmpImage,tmpImage,threshold,threshold*3);
    cv::cvtColor(tmpImage,tmpImage,CV_GRAY2BGR);
    qDebug()<<tmpImage.channels();
    Mat2Ipl();

    updateDisplay(currentImageNum);
}

void ScribbleArea::erodeFilter(void){
    if(totalImageNum<=0) return;

    Ipl2Mat();
    drawMask();

    QDialog getSizeDialog(this);
    getSizeDialog.setWindowTitle("Eroder Filter");

    QGridLayout *dialogLayout = new QGridLayout(&getSizeDialog);
    dialogLayout->addWidget(new QLabel(tr("Input size: "), &getSizeDialog),0, 0);
    QSpinBox *sizeInputBox = new QSpinBox;
    sizeInputBox->setValue(1);
    sizeInputBox->setRange(0,100);  /// @note Range need to be set
    dialogLayout->addWidget(sizeInputBox,0,1);

    QPushButton *okButton = new QPushButton(tr("Ok"), &getSizeDialog);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), &getSizeDialog);
    connect(okButton, SIGNAL(clicked()), &getSizeDialog, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), &getSizeDialog, SLOT(reject()));
    okButton->setDefault(true);
    dialogLayout->addWidget(cancelButton);
    dialogLayout->addWidget(okButton);

    if(!getSizeDialog.exec()) return;

    int size=sizeInputBox->value();


    cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                                                 cv::Size( 2*size + 1, 2*size+1 ),
                                                 cv::Point( size, size ) );
    cv::erode(tmpImage,tmpImage,element);
    Mat2Ipl();

    updateDisplay(currentImageNum);
}

void ScribbleArea::dilateFilter(void){
    if(totalImageNum<=0) return;

    Ipl2Mat();
    drawMask();

    QDialog getSizeDialog(this);
    getSizeDialog.setWindowTitle("Dilate Filter");

    QGridLayout *dialogLayout = new QGridLayout(&getSizeDialog);
    dialogLayout->addWidget(new QLabel(tr("Input size: "), &getSizeDialog),0, 0);
    QSpinBox *sizeInputBox = new QSpinBox;
    sizeInputBox->setValue(1);
    sizeInputBox->setRange(0,100);  /// @note Range need to be set
    dialogLayout->addWidget(sizeInputBox,0,1);

    QPushButton *okButton = new QPushButton(tr("Ok"), &getSizeDialog);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), &getSizeDialog);
    connect(okButton, SIGNAL(clicked()), &getSizeDialog, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), &getSizeDialog, SLOT(reject()));
    okButton->setDefault(true);
    dialogLayout->addWidget(cancelButton);
    dialogLayout->addWidget(okButton);

    if(!getSizeDialog.exec()) return;

    int size=sizeInputBox->value();


    cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                                                 cv::Size( 2*size + 1, 2*size+1 ),
                                                 cv::Point( size, size ) );
    cv::dilate(tmpImage,tmpImage,element);
    Mat2Ipl();

    updateDisplay(currentImageNum);
}

void ScribbleArea::grabcutFilter(void){
    if(totalImageNum<=0) return;

    Ipl2Mat();
    drawMask(cv::GC_PR_FGD);

    cv::Mat bkgModel,fgrModel;

    cv::grabCut(tmpImage,  // input image
                mask, // segmentation result
                cv::Rect(),bkgModel,fgrModel,3,cv::GC_INIT_WITH_MASK);
    mask=mask&1;

    cv::Mat tmp=cv::Mat(imageStackEdit[currentImageNum]);
    tmp.setTo(cv::Scalar(bgColor.blue(),bgColor.green(),bgColor.red()));
    IplImage tmpIpl=tmp;
    cvConvertImage(&tmpIpl,imageStackEdit[currentImageNum]);

    Mat2Ipl();

    updateDisplay(currentImageNum);
}

void ScribbleArea::Ipl2Mat(){
    cv::Mat tmp=cv::Mat(imageStackEdit[currentImageNum]);
    tmpImage=tmp.clone();
}

void ScribbleArea::drawMask(int value){
    mask=cv::Mat(tmpImage.rows,tmpImage.cols, CV_8U);
    mask.setTo(0);
    if(somethingSelected == false){
        mask.setTo(value);
    }
    else{
        if(selectionType==TwoPointsSelection){
            readjustRect();
            cv::rectangle(mask,cv::Point(vertexLeftTop),cv::Point(vertexRightBottom),value,-1);
        }
        else{
            foreach(CvPoint tmp, irregularSelectionPoints){
                qDebug()<<"("<<tmp.x<<","<<tmp.y<<")";
            }
            cv::Point points[1][irregularSelectionPoints.length()];
            for(int i=0;i<irregularSelectionPoints.length();i++){
                points[0][i]=irregularSelectionPoints[i];
            }
            const cv::Point* ppt[1] = { points[0] };
            int npt[] = { irregularSelectionPoints.length() };

            cv::fillPoly( mask,
                      ppt,
                      npt,
                      1,
                      value);
        }
    }
}

void ScribbleArea::Mat2Ipl(){
    cv::Mat tmp=cv::Mat(imageStackEdit[currentImageNum]);
    tmpImage.copyTo(tmp,mask);
    IplImage tmpIpl=tmp;
    cvConvertImage(&tmpIpl,imageStackEdit[currentImageNum]);
}

void ScribbleArea::dragMoveSelectedArea()
{
    if(isMousePressed==true); // display move

    else;  //actual move


}

void ScribbleArea::rotateSelectedArea()
{
    if(isMousePressed==true); // display move

    else;  //actual move
}
