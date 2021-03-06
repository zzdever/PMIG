﻿#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QWidget>
#include <QAction>
#include <QFrame>
#include <QDebug>

#include "toolbox.h"


ToolBox::ToolBox(QWidget *parent)
{
    setupMarqueeToolBar(parent);
    setupBrushToolBar(parent);
    setupPenToolBar(parent);

    currentToolBar = brushToolBar;
}

ToolBox::~ToolBox()
{
    foreach (QToolBar *tmpToolBar, toolBarList) {
        delete tmpToolBar;
    }

}

void ToolBox::setupMarqueeToolBar(QWidget *parent)
{
    marqueeToolBar = new QToolBar(("Marquee tool bar"),parent);
    marqueeToolBar->setAllowedAreas(Qt::TopToolBarArea);
    marqueeToolBar->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    marqueeToolBar->setMovable(false);
    QLabel *name = new QLabel("MARQUEE TOOL");
    name->setFixedHeight(35);
    marqueeToolBar->addWidget(name);
    marqueeToolBar->addSeparator();
    //marqueeToolBar->addWidget()
    toolBarList.append(marqueeToolBar);
}

void ToolBox::setupBrushToolBar(QWidget *parent)
{
    brushToolBar = new QToolBar(("Brush tool bar"),parent);
    brushToolBar->setAllowedAreas(Qt::TopToolBarArea);
    brushToolBar->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    brushToolBar->setMovable(false);
    QLabel *name = new QLabel("BRUSH TOOL");
    name->setFixedHeight(35);
    brushToolBar->addWidget(name);
    brushToolBar->addSeparator();
    QSpinBox *brushSize = new QSpinBox;
    brushSize->setRange(0,100);
    brushToolBar->addWidget(brushSize);

    toolBarList.append(brushToolBar);
}

void ToolBox::setupPenToolBar(QWidget *parent)
{
    penToolBar = new QToolBar(("Pen tool bar"),parent);
    penToolBar->setAllowedAreas(Qt::TopToolBarArea);
    penToolBar->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    penToolBar->setMovable(false);
    QLabel *name = new QLabel("PEN TOOL");
    name->setFixedHeight(35);
    penToolBar->addWidget(name);
    penToolBar->addSeparator();
    penSize = new QSpinBox;
    penSize->setRange(0,100);
    penToolBar->addWidget(penSize);

    toolBarList.append(penToolBar);
}

ToolTweak::ToolTweak(const QString &title, QWidget *parent)
    :QToolBar(title, parent)
{
    this->setAllowedAreas(Qt::TopToolBarArea);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->setMovable(true);
    this->setHidden(true);
    QLabel *name = new QLabel(title,this);
    name->setFixedHeight(35);
    this->addWidget(name);
    this->addSeparator();
}

//+++++++++++++Brush+Tool+++++++++++++++++++++++++++++++++++++
int BrushToolBase::brushSize=2;
int BrushToolBase::lineType;
bool BrushToolBase::antiAliasing;

BrushToolTweak::BrushToolTweak(QWidget *parent)
    :ToolTweak("BRUSH TOOL", parent)
{
    QSpinBox *sizeSpinBox = new QSpinBox(this);
    sizeSpinBox->setRange(1,100);
    sizeSpinBox->setValue(2);
    QAction *autoIncrement = new QAction(this);
    sizeSpinBox->addAction(autoIncrement);
    autoIncrement->setShortcut(Qt::Key_BracketRight);
    connect(autoIncrement, SIGNAL(triggered()),sizeSpinBox,SLOT(stepUp()));
    autoIncrement = new QAction(this);
    sizeSpinBox->addAction(autoIncrement);
    autoIncrement->setShortcut(Qt::Key_BracketLeft);
    connect(autoIncrement, SIGNAL(triggered()),sizeSpinBox,SLOT(stepDown())) ;
    this->addWidget(new QLabel("size: ",this));
    this->addWidget(sizeSpinBox);
    connect(sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setBrushSize(int)));

    this->addSeparator();

    QCheckBox *antiAliasingCheckBox = new QCheckBox(this);
    antiAliasingCheckBox->setText("Anti-Aliasing");
    this->addWidget(antiAliasingCheckBox);
    connect(antiAliasingCheckBox,SIGNAL(toggled(bool)),this, SLOT(setAntiAliasing(bool)));
}

BrushToolFunction::BrushToolFunction(QWidget *parent)
    :QObject(parent)
{
    ;
}


//+++++++++++Erase+Tool+++++++++++++++++++++++++++++++++++++++
int EraseToolBase::eraseSize=10;
int EraseToolBase::eraseShape;

EraseToolTweak::EraseToolTweak(QWidget *parent)
    :ToolTweak("ERASE TOOL", parent)
{
    QSpinBox *sizeSpinBox = new QSpinBox(this);
    sizeSpinBox->setRange(1,100);
    sizeSpinBox->setValue(10);
    this->addWidget(new QLabel("size: ",this));
    this->addWidget(sizeSpinBox);
    connect(sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setEraseSize(int)));
}

EraseToolFunction::EraseToolFunction(QWidget *parent)
    :QObject(parent)
{
    ;
}

//+++++++++++Marquee+Tool+++++++++++++++++++++++++++++++++++++++
int MarqueeToolBase::selectionType;

MarqueeToolTweak::MarqueeToolTweak(QWidget *parent)
    :ToolTweak("MARQUEE TOOL", parent)
{
    QComboBox *selectionTypeBox = new QComboBox(this);
    selectionTypeBox->addItem("Rectangle");
    //selectionTypeBox->addItem("Circle");
    this->addWidget(selectionTypeBox);

    connect(selectionTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectionType(int)));
}

MarqueeToolFunction::MarqueeToolFunction(QWidget *parent)
    :QObject(parent)
{
    ;
}


//+++++++++++Color+Swatch+++++++++++++++++++++++++++++++++++++++
//int ColorSwatchBase::colorBoxWidth=10;

//ColorSwatchTweak::ColorSwatchTweak(QWidget *parent)
//    :ToolTweak("MARQUEE TOOL", parent)
//{
//    QFrame *colorSwatchFrame = new QFrame(this);
//    for(int i=0; i<50; i++)
//    {
//        QPixmap *colorBox = new QPixmap(colorBoxWidth, colorBoxWidth);
//        colorBox->fill(QColor(i*5,i*5,i*5));
//        colorSwatchFrame
//    }



//    connect(selectionTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectionType(int)));
//}

//ColorSwatchFunction::ColorSwatchFunction(QWidget *parent)
//    :QObject(parent)
//{
//    ;
//}
