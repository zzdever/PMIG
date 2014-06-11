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
    ToolBox *toolBoxToolBar;
    QHash<ToolType::toolType, ToolTweak*> toolsToolBar;
    ToolType::toolType currentToolType;
    //ToolTweak *brushToolTweak;
    QToolBar *toolBox;
    //OpencvProcess *opencvProcess;

    QTextEdit *center;
    QList<ToolBar*> toolBars;

    QMenu *fileMenu;
    QMenu *mainWindowMenu;
    QMenu *windowWidgetMenu;
    QMenu *aboutMenu;

    QSignalMapper *mapper;
    QList<QDockWidget*> extraDockWidgets;
    QAction *createDockWidgetAction;
    QMenu *destroyDockWidgetMenu;

public:
//    MainWindow(const QMap<QString, QSize> &customSizeHints,
//                QWidget *parent = 0, Qt::WindowFlags flags = 0);
    MainWindow();
    ~MainWindow();

protected:
    //void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    void openFile();
    void saveFile();
    bool saveWrite(const QByteArray);
    bool maybeSave();
    //void penColor();

    void actionTriggered(QAction *action);
    void saveLayout();
    void loadLayout();
    void setCorner(int id);
    //void switchLayoutDirection();
    void setDockOptions();

    //void createDockWidget();
    //void destroyDockWidget(QAction *action);

    void about();

private:
    IplImage *cvImg;

    void setupToolBar();
    void setupMenuBar();
    void setupWindowWidgets();
    void switchToolsToolBar(ToolType::toolType newToolType);

    //QMenu *saveAsMenu;
    //QMenu *fileMenu;



//    QAction *penColorAct;
//    QAction *penWidthAct;
//    QAction *printAct;
//    QAction *clearScreenAct;

private slots:
    void setToolMarquee(bool toggle){
        if(toggle) switchToolsToolBar(ToolType::Marquee);
//            toolBoxToolBar->currentToolBar->setHidden(true);
//            toolBoxToolBar->currentToolBar = toolBoxToolBar->marqueeToolBar;
//            toolBoxToolBar->currentToolBar->setHidden(false);
//            update();
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
