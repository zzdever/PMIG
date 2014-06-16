#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QWidget>
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


//+++++++++++ERASE+TOOL+++++++++++++++++++++++++++++++++++++++
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

//+++++++++++Marquee+TOOL+++++++++++++++++++++++++++++++++++++++
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

//+++++++++++Transform+TOOL+++++++++++++++++++++++++++++++++++++++

TransformToolTweak::TransformToolTweak(QWidget *parent)
    :ToolTweak("TRANSFORM TOOL", parent)
{
    //temporary
    this->addSeparator();
    QLabel *note = new QLabel("Sorry, currently size free transformation is NOT supported",this);
    this->addWidget(note);
    //temporary
}

TransformToolFunction::TransformToolFunction(QWidget *parent)
    :QObject(parent)
{
    ;
}


//+++++++++++Lasso+TOOL+++++++++++++++++++++++++++++++++++++++
bool LassoToolBase::magnetic=false;

LassoToolTweak::LassoToolTweak(QWidget *parent)
    :ToolTweak("LASSO TOOL", parent)
{
    QCheckBox *magneticCheckBox = new QCheckBox(this);
    magneticCheckBox->setText("Magnetic");
    this->addWidget(magneticCheckBox);
    connect(magneticCheckBox,SIGNAL(toggled(bool)),this, SLOT(setMagnetic(bool)));
}

LassoToolFunction::LassoToolFunction(QWidget *parent)
    :QObject(parent)
{
    ;
}


//+++++++++++Pen+TOOL+++++++++++++++++++++++++++++++++++++++

PenToolTweak::PenToolTweak(QWidget *parent)
    :ToolTweak("PEN TOOL", parent)
{
    ;
}

PenToolFunction::PenToolFunction(QWidget *parent)
    :QObject(parent)
{
    penHandler = new HoverPoints(parent, HoverPoints::CircleShape);
    penHandler->setConnectionType(HoverPoints::LineConnection);
    penHandler->setCloseType(HoverPoints::Close);
    penHandler->setEditable(false);
    penHandler->setPointSize(QSize(10, 10));
    penHandler->setShapeBrush(QBrush(QColor(0, 0, 0, 255)));
    penHandler->setShapePen(QPen(QColor(0, 0, 0, 150)));
    penHandler->setConnectionPen(QPen(QColor(0, 0, 0, 150)));
    connect(penHandler, SIGNAL(pointsChanged(QPolygonF)),this, SLOT(updatePenHandlerControlPoints(QPolygonF)));

    penHandlerControl.clear();

    penMenu = new QMenu;
    QAction *action = new QAction("Clear pen Points", penMenu);
    connect(action, SIGNAL(triggered()), this, SLOT(clearPoints()));
    penMenu->addAction(action);
    penMenu->addSeparator();
    action = new QAction("Make selection", penMenu);
    connect(action, SIGNAL(triggered()),this,SLOT(makeSelection_()));
    penMenu->addAction(action);
    action = new QAction("Stroke path", penMenu);
    connect(action, SIGNAL(triggered()),this,SLOT(strokePath_()));
    penMenu->addAction(action);
    action = new QAction("Fill shape", penMenu);
    connect(action, SIGNAL(triggered()),this, SLOT(fillPath_()));
    penMenu->addAction(action);
}
