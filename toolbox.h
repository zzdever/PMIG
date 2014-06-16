#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QObject>
#include <QHash>
#include <QToolBar>
#include <QList>
#include <QSpinBox>
#include <QDebug>

#include "shared/hoverpoints.h"

class ToolType{
public:
    enum toolType{
        Brush=0,
        Erase=1,
        Marquee=2,
        Pen=3,
        Transform=4,
        Lasso=5
    };
};

class ToolBox:public QObject
{
private:
    void setupMarqueeToolBar(QWidget *parent);
    void setupBrushToolBar(QWidget *parent);
    void setupPenToolBar(QWidget *parent);

public:
    QList<QToolBar*> toolBarList;
    QToolBar *currentToolBar;

    QToolBar *marqueeToolBar;
    QToolBar *brushToolBar;
    QToolBar *penToolBar;

    QSpinBox *penSize;

    ToolBox(QWidget *parent);
    ~ToolBox();
};

class ToolTweak:public QToolBar
{
    Q_OBJECT
public:
    ToolTweak(const QString &title, QWidget *parent);

};


//+++++++++++++Brush+Tool+++++++++++++++++++++++++++++++++++++
class BrushToolBase
{
protected:
    static int brushSize;
    static int lineType;
    static bool antiAliasing;
};


class BrushToolTweak
        :public ToolTweak,
        protected BrushToolBase
{
    Q_OBJECT
public:
    BrushToolTweak(QWidget *parent);

private slots:
    void setBrushSize(int value){brushSize=value;}
    void setLineType(int value){lineType=value;}
    void setAntiAliasing(bool value){antiAliasing=value;}
};

class BrushToolFunction
        :public QObject,
        protected BrushToolBase
{
    Q_OBJECT
public:
    BrushToolFunction(QWidget *parent);

    int getBrushSize() const {return brushSize;}
    int getLineType() const {return lineType;}
    bool getAntiAliasing() const {return antiAliasing;}

};


//+++++++++++ERASE+TOOL+++++++++++++++++++++++++++++++++++++++
class EraseToolBase
{
protected:
    static int eraseSize;
    static int eraseShape;
};


class EraseToolTweak
        :public ToolTweak,
        protected EraseToolBase
{
    Q_OBJECT
public:
    EraseToolTweak(QWidget *parent);

private slots:
    void setEraseSize(int value){eraseSize=value;}
    void setEraseShape(int value){eraseShape=value;}

signals:
};

class EraseToolFunction
        :public QObject,
        protected EraseToolBase
{
    Q_OBJECT
public:
    EraseToolFunction(QWidget *parent);

    int getEraseSize()const {return eraseSize;}
    bool getEraseShape()const {return eraseShape;}

};


//+++++++++++++Marquee+Tool+++++++++++++++++++++++++++++++++++++
class MarqueeToolBase
{
protected:
    static int selectionType;
};


class MarqueeToolTweak
        :public ToolTweak,
        protected MarqueeToolBase
{
    Q_OBJECT
public:
    MarqueeToolTweak(QWidget *parent);

private slots:
    void setSelectionType(int value){selectionType=value;}

};

class MarqueeToolFunction
        :public QObject,
        protected MarqueeToolBase
{
    Q_OBJECT
public:
    MarqueeToolFunction(QWidget *parent);

    int getSelectionType() const {return selectionType;}

};


//+++++++++++++Transform+Tool+++++++++++++++++++++++++++++++++++++
class TransformToolBase
{
protected:
};


class TransformToolTweak
        :public ToolTweak,
        protected TransformToolBase
{
    Q_OBJECT
public:
    TransformToolTweak(QWidget *parent);

private slots:
    //void setSelectionType(int value){selectionType=value;}

};

class TransformToolFunction
        :public QObject,
        protected TransformToolBase
{
    Q_OBJECT
public:
    TransformToolFunction(QWidget *parent);

    //int getSelectionType() const {return selectionType;}

};


//+++++++++++++Lasso+Tool+++++++++++++++++++++++++++++++++++++
class LassoToolBase
{
protected:
    static bool magnetic;
};


class LassoToolTweak
        :public ToolTweak,
        protected LassoToolBase
{
    Q_OBJECT
public:
    LassoToolTweak(QWidget *parent);

private slots:
    void setMagnetic(bool value){magnetic=value;}

};

class LassoToolFunction
        :public QObject,
        protected LassoToolBase
{
    Q_OBJECT
public:
    LassoToolFunction(QWidget *parent);

    bool getMagnetic() const {return magnetic;}

};


//+++++++++++++Pen+Tool+++++++++++++++++++++++++++++++++++++
class PenToolBase
{
protected:
};


class PenToolTweak
        :public ToolTweak,
        protected PenToolBase
{
    Q_OBJECT
public:
    PenToolTweak(QWidget *parent);

private slots:
    //void setSelectionType(int value){selectionType=value;}

};

class PenToolFunction
        :public QObject,
        protected PenToolBase
{
    Q_OBJECT
public:
    PenToolFunction(QWidget *parent);

    HoverPoints *penHandler;
    QPolygonF penHandlerControl;
    QMenu *penMenu;
    //int getSelectionType() const {return selectionType;}
private slots:
    void updatePenHandlerControlPoints(QPolygonF points) {
        penHandlerControl=points;
        penHandler->setPoints(points);
    }
    void makeSelection_() {emit makeSelection();}
    void strokePath_() {emit makeSelection(); emit strokePath(); }
    void fillPath_() {emit makeSelection(); emit fillPath();}
public slots:
    void clearPoints() {
        penHandlerControl.clear();
        penHandler->setPoints(penHandlerControl);
    }

signals:
    void makeSelection();
    void strokePath();
    void fillPath();
};
#endif // TOOLBOX_H
