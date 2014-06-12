#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QHash>
#include <QWidget>

#include <cv.h>
#include <highgui.h>


#include "scribblearea.h"
#include "toolbox.h"
#include "opencvprocess.h"

class ToolBar;
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ScribbleArea *centerScribbleArea;
    QHash<ToolType::toolType, ToolTweak*> toolsToolBar;
    ToolType::toolType currentToolType;
    QToolBar *toolBox;

    //QList<ToolBar*> toolBars;

    QMenu *fileMenu;
    QMenu *mainWindowMenu;
    QMenu *windowWidgetMenu;
    QMenu *aboutMenu;

    QSignalMapper *mapper;
    QList<QDockWidget*> extraDockWidgets;
    QAction *createDockWidgetAction;
    QMenu *destroyDockWidgetMenu;

public:
    MainWindow(QWidget *parent = 0);
    //~MainWindow();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    void openFile();
    void saveFile();
    bool saveWrite(const QByteArray);
    bool maybeSave();
    //void penColor();

    void saveLayout();
    void loadLayout();


    //void createDockWidget();
    //void destroyDockWidget(QAction *action);

    void about();

private:
    IplImage *cvImg;

    void setupToolBar();
    void setupMenuBar();
    void setupWindowWidgets();
    void setDockOptions();
    void switchToolsToolBar(ToolType::toolType newToolType);

    //QMenu *saveAsMenu;
    //QMenu *fileMenu;

//    QAction *printAct;
//    QAction *clearScreenAct;

private slots:
    void setToolMarquee(bool toggle){
        if(toggle) switchToolsToolBar(ToolType::Marquee);
    }
    void setToolBrush(bool toggle){
        if(toggle) switchToolsToolBar(ToolType::Brush);
    }
    void setToolPen(bool toggle){
        if(toggle) switchToolsToolBar(ToolType::Pen);
    }
    void setToolErase(bool toggle){
        if(toggle) switchToolsToolBar(ToolType::Erase);
    }

signals:

};


#endif
