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

/// Create a MainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

    ScribbleArea *centerScribbleArea;
    QHash<ToolType::toolType, ToolTweak*> toolsToolBar;
    ToolType::toolType currentToolType;
    QToolBar *toolBox;

    //QList<ToolBar*> toolBars;

    QMenu *fileMenu;
    QMenu *filterMenu;
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
//    void keyPressEvent(QKeyEvent *event);

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

    /// Create toolbar on the top and on the left(toolbox)
    void setupToolBar();
    /// Create menus
    void setupMenuBar();
    /// Create widgets on the right
    void setupWindowWidgets();
    void setDockOptions();
    /// Switch to a new toolbar on the box when the tool changes
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
    void setToolTransform(bool toggle){
        if(toggle) {switchToolsToolBar(ToolType::Transform);
            centerScribbleArea->setTransformSelectionState();
        }
    }
    void setToolLasso(bool toggle){
        if(toggle) switchToolsToolBar(ToolType::Lasso);
    }

    void setColor(int);
    //void setBgColor(void);

signals:
    void updateColorIcon(int, QColor);
    //void updateBgColorIcon();
};


#endif
