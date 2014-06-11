#include "mainwindow.h"
#include "colorswatch.h"
#include "toolbar.h"

//#include <QtWidgets>
#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDockWidget>
#include <QImageWriter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <qdebug.h>


#include <QtEvents>
#include <QFrame>
#include <QMainWindow>

Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

//MainWindow::MainWindow(const QMap<QString, QSize> &customSizeHints,
//                        QWidget *parent, Qt::WindowFlags flags)
MainWindow::MainWindow()
//    : QMainWindow(parent, flags)
{
    setObjectName("MainWindow");
    setWindowTitle("PMIG");

    centerScribbleArea = new ScribbleArea(this);
    setCentralWidget(centerScribbleArea);

    //toolBoxToolBar = new ToolBox(this);
//    foreach(QToolBar* tmpToolBar, toolBoxToolBar->toolBarList)
//    {
//        addToolBar(Qt::TopToolBarArea,tmpToolBar);
//        if(toolBoxToolBar->currentToolBar != tmpToolBar)
//            tmpToolBar->setHidden(true);
//    }

//    opencvProcess = new OpencvProcess;
//    connect(opencvProcess, &OpencvProcess::updateDisplay, this, &ScribbleArea::updateDisplay);

    setupToolBar();
    setupMenuBar();
    setupWindowWidgets();

    centerScribbleArea->setFocus();
    statusBar()->showMessage(tr("Ready"));

    currentToolType = ToolType::Brush;

    toolsToolBar.insert(ToolType::Brush, new BrushToolTweak(this));
    toolsToolBar.insert(ToolType::Erase, new EraseToolTweak(this));
    foreach (ToolType::toolType tmp, toolsToolBar.keys()) {
        addToolBar(toolsToolBar[tmp]);
    }
    toolsToolBar[currentToolType]->setHidden(false);
}

MainWindow::~MainWindow()
{
    //delete toolBox;
}

void MainWindow::actionTriggered(QAction *action)
{
    qDebug("action '%s' triggered", action->text().toLocal8Bit().data());
}

void MainWindow::setupToolBar()
{
    QActionGroup *toolBoxGroup = new QActionGroup(this);
    toolBoxGroup->setExclusive(true);

    QAction *marqueeAct = new QAction(QIcon(":images/marquee-square.png"),tr("&Marquee tool (M)"),this);\
    marqueeAct->setShortcut(Qt::Key_M);
    marqueeAct->setStatusTip(tr("To select an area"));
    marqueeAct->setCheckable(true);
    connect(marqueeAct,SIGNAL(toggled(bool)),this,SLOT(setToolMarquee(bool)));

    QAction *brushAct = new QAction(QIcon(":images/brush-tool.png"),tr("&Brush tool (B)"),this);
    brushAct->setShortcut(Qt::Key_B);
    brushAct->setStatusTip(tr("To brush strokes"));
    brushAct->setCheckable(true);
    connect(brushAct,SIGNAL(toggled(bool)),this,SLOT(setToolBrush(bool)));

    QAction *penAct = new QAction(QIcon(":images/pen-tool.png"),tr("&Pen tool (P)"),this);
    penAct->setShortcut(Qt::Key_P);
    penAct->setStatusTip(tr("To draw a path"));
    penAct->setCheckable(true);
    connect(penAct,SIGNAL(toggled(bool)),this,SLOT(setToolPen(bool)));

    QAction *eraseAct = new QAction(QIcon(":images/eraser-tool.png"),tr("&Erase tool (E)"),this);
    eraseAct->setShortcut(Qt::Key_E);
    eraseAct->setStatusTip(tr("To erase an area"));
    eraseAct->setCheckable(true);
    connect(eraseAct,SIGNAL(toggled(bool)),this,SLOT(setToolErase(bool)));

    toolBoxGroup->addAction(marqueeAct);
    toolBoxGroup->addAction(brushAct);
    toolBoxGroup->addAction(penAct);
    toolBoxGroup->addAction(eraseAct);


    toolBox = addToolBar(tr("Tool box"));
    addToolBar(Qt::LeftToolBarArea, toolBox);
    toolBox->setAllowedAreas(Qt::LeftToolBarArea);
    toolBox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    toolBox->setOrientation(Qt::Vertical);
    toolBox->setFloatable(false);
    toolBox->setMovable(false);


    toolBox->addAction(marqueeAct);
    toolBox->addAction(brushAct);
    toolBox->addAction(penAct);
    toolBox->addAction(eraseAct);

//    for (int i = 0; i < 3; ++i) {
//        ToolBar *tb = new ToolBar(QString::fromLatin1("Tool Bar %1").arg(i + 1), this);
//        toolBars.append(tb);
//        addToolBar(tb);
//    }
}

void MainWindow::switchToolsToolBar(ToolType::toolType newToolType)
{
    if(!toolsToolBar.contains(newToolType))
        return;

    toolsToolBar[currentToolType]->setHidden(true);
    toolsToolBar[newToolType]->setHidden(false);
    currentToolType=newToolType;
}

void MainWindow::setupMenuBar()
{
    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *printAct;
    QAction *exitAct;

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(QKeySequence::Print);
    connect(printAct, SIGNAL(triggered()), centerScribbleArea, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


    QMenu *saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);



//    QMenu *menu = menuBar()->addMenu(tr("&File"));

//    QAction *action = menu->addAction(tr("Save layout..."));
//    connect(action, SIGNAL(triggered()), this, SLOT(saveLayout()));

//    action = menu->addAction(tr("Load layout..."));
//    connect(action, SIGNAL(triggered()), this, SLOT(loadLayout()));

//    action = menu->addAction(tr("Switch layout direction"));
//    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));

//    menu->addSeparator();

//    menu->addAction(tr("&Quit"), this, SLOT(close()));

    mainWindowMenu = menuBar()->addMenu(tr("Main window"));

    QAction *action = mainWindowMenu->addAction(tr("Animated docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AnimatedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Allow nested docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowNestedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Allow tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowTabbedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Force tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & ForceTabbedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Vertical tabs"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & VerticalTabs);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

//    QMenu *toolBarMenu = menuBar()->addMenu(tr("Tool bars"));
//    for (int i = 0; i < toolBars.count(); ++i)
//        toolBarMenu->addMenu(toolBars.at(i)->menu);

    windowWidgetMenu = menuBar()->addMenu(tr("&Window"));

    aboutMenu = new QMenu(tr("&About"), this);
    QAction *aboutAct = new QAction(tr("About PMIG"), this);
    aboutMenu->addAction(aboutAct);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    QAction *aboutQtAct = new QAction(tr("About QT"), this);
    aboutMenu->addAction(aboutQtAct);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    menuBar()->addMenu(aboutMenu);
}

void MainWindow::setDockOptions()
{
    DockOptions opts;
    QList<QAction*> actions = mainWindowMenu->actions();

    if (actions.at(0)->isChecked())
        opts |= AnimatedDocks;
    if (actions.at(1)->isChecked())
        opts |= AllowNestedDocks;
    if (actions.at(2)->isChecked())
        opts |= AllowTabbedDocks;
    if (actions.at(3)->isChecked())
        opts |= ForceTabbedDocks;
    if (actions.at(4)->isChecked())
        opts |= VerticalTabs;

    QMainWindow::setDockOptions(opts);
}

void MainWindow::saveLayout()
{
    QString fileName
        = QFileDialog::getSaveFileName(this, tr("Save layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    bool ok = file.putChar((uchar)geo_data.size());
    if (ok)
        ok = file.write(geo_data) == geo_data.size();
    if (ok)
        ok = file.write(layout_data) == layout_data.size();

    if (!ok) {
        QString msg = tr("Error writing to %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void MainWindow::loadLayout()
{
    QString fileName
        = QFileDialog::getOpenFileName(this, tr("Load layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    uchar geo_size;
    QByteArray geo_data;
    QByteArray layout_data;

    bool ok = file.getChar((char*)&geo_size);
    if (ok) {
        geo_data = file.read(geo_size);
        ok = geo_data.size() == geo_size;
    }
    if (ok) {
        layout_data = file.readAll();
        ok = layout_data.size() > 0;
    }

    if (ok)
        ok = restoreGeometry(geo_data);
    if (ok)
        ok = restoreState(layout_data);

    if (!ok) {
        QString msg = tr("Error reading %1")
                        .arg(fileName);
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

QAction *addAction(QMenu *menu, const QString &text, QActionGroup *group, QSignalMapper *mapper,
                    int id)
{
    bool first = group->actions().isEmpty();
    QAction *result = menu->addAction(text);
    result->setCheckable(true);
    result->setChecked(first);
    group->addAction(result);
    QObject::connect(result, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(result, id);
    return result;
}

void MainWindow::setupWindowWidgets()
{
    qRegisterMetaType<QDockWidget::DockWidgetFeatures>();

    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setCorner(int)));

    /*
    QMenu *corner_menu = dockWidgetMenu->addMenu(tr("Top left corner"));
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);
    ::addAction(corner_menu, tr("Top dock area"), group, mapper, 0);
    ::addAction(corner_menu, tr("Left dock area"), group, mapper, 1);

    corner_menu = dockWidgetMenu->addMenu(tr("Top right corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    ::addAction(corner_menu, tr("Top dock area"), group, mapper, 2);
    ::addAction(corner_menu, tr("Right dock area"), group, mapper, 3);

    corner_menu = dockWidgetMenu->addMenu(tr("Bottom left corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    ::addAction(corner_menu, tr("Bottom dock area"), group, mapper, 4);
    ::addAction(corner_menu, tr("Left dock area"), group, mapper, 5);

    corner_menu = dockWidgetMenu->addMenu(tr("Bottom right corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    ::addAction(corner_menu, tr("Bottom dock area"), group, mapper, 6);
    ::addAction(corner_menu, tr("Right dock area"), group, mapper, 7);

    dockWidgetMenu->addSeparator();
    */

    static const struct Set {
        const char * name;
        uint flags;
        Qt::DockWidgetArea area;
    } sets [] = {
        { "Layer", 0, Qt::RightDockWidgetArea },
        { "ColorSwatch", 0, Qt::RightDockWidgetArea },
        { "Graph", 0, Qt::RightDockWidgetArea },
        { "Curve", 0, Qt::RightDockWidgetArea },
        { "Character", 0, Qt::RightDockWidgetArea },
        { "Info", 0, Qt::RightDockWidgetArea }
    };
    const int setCount = sizeof(sets) / sizeof(Set);

    for (int i = 0; i < setCount; ++i) {
        ColorSwatch *swatch = new ColorSwatch(tr(sets[i].name), this, Qt::WindowFlags(sets[i].flags));
//        if (i%2)
//            swatch->setWindowIcon(QIcon(QPixmap(":/res/qt.png")));
//        if (qstrcmp(sets[i].name, "Blue") == 0) {
//            BlueTitleBar *titlebar = new BlueTitleBar(swatch);
//            swatch->setTitleBarWidget(titlebar);
//            connect(swatch, SIGNAL(topLevelChanged(bool)), titlebar, SLOT(updateMask()));
//            connect(swatch, SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)), titlebar, SLOT(updateMask()), Qt::QueuedConnection);

//        }

        addDockWidget(sets[i].area, swatch);
        windowWidgetMenu->addAction(swatch->windowWidgetAction);
    }
}

void MainWindow::setCorner(int id)
{
    switch (id) {
        case 0:
            QMainWindow::setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
            break;
        case 1:
            QMainWindow::setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
            break;
        case 2:
            QMainWindow::setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);
            break;
        case 3:
            QMainWindow::setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
            break;
        case 4:
            QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
            break;
        case 5:
            QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
            break;
        case 6:
            QMainWindow::setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
            break;
        case 7:
            QMainWindow::setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
            break;
    }
}

//void MainWindow::showEvent(QShowEvent *event)
//{
//    QMainWindow::showEvent(event);
//}



void MainWindow::closeEvent(QCloseEvent *event)
//! [1] //! [2]
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
//! [2]

//! [3]
void MainWindow::openFile()
//! [3] //! [4]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
        {
            centerScribbleArea->openImage(fileName);
//            cvImg = cvLoadImage(&(fileName.toStdString()[0]));
//            centerScribbleArea->openImage(fileName, cvImg);
        }
    }
}
//! [4]



bool MainWindow::saveWrite(const QByteArray fileFormat)
//! [19] //! [20]
{

    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return centerScribbleArea->saveImage(fileName, fileFormat.constData());
    }
}

void MainWindow::saveFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    MainWindow::saveWrite(fileFormat);
}

bool MainWindow::maybeSave()
//! [17] //! [18]
{
    if (centerScribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
              | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveWrite("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}



//! [7]
//void MainWindow::penColor()
////! [7] //! [8]
//{
//    QColor newColor = QColorDialog::getColor(centerScribbleArea->penColor());
//    if (newColor.isValid())
//        centerScribbleArea->setPenColor(newColor);
//}
////! [8]

////! [9]
//void MainWindow::penWidth()
////! [9] //! [10]
//{
//    bool ok;
//    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
//                                        tr("Select pen width:"),
//                                        centerScribbleArea->penWidth(),
//                                        1, 50, 1, &ok);
//    if (ok)
//        centerScribbleArea->setPenWidth(newWidth);
//}
////! [10]

//! [11]
void MainWindow::about()
//! [11] //! [12]
{
    QMessageBox::about(this, tr("About PMIG"),
            tr("<p><b>PMIG</b> is a simple image processing tool.</p>"
               "<p>If you are curious about the name, "
               "it's a tricky reverse of the powerful open software GIMP.</p>"));
}


//! [18]

//! [19]


/*
void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

class CreateDockWidgetDialog : public QDialog
{
public:
    CreateDockWidgetDialog(QWidget *parent = 0);

    QString objectName() const;
    Qt::DockWidgetArea location() const;

private:
    QLineEdit *m_objectName;
    QComboBox *m_location;
};

CreateDockWidgetDialog::CreateDockWidgetDialog(QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(new QLabel(tr("Object name:")), 0, 0);
    m_objectName = new QLineEdit;
    layout->addWidget(m_objectName, 0, 1);

    layout->addWidget(new QLabel(tr("Location:")), 1, 0);
    m_location = new QComboBox;
    m_location->setEditable(false);
    m_location->addItem(tr("Top"));
    m_location->addItem(tr("Left"));
    m_location->addItem(tr("Right"));
    m_location->addItem(tr("Bottom"));
    m_location->addItem(tr("Restore"));
    layout->addWidget(m_location, 1, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    layout->addLayout(buttonLayout, 2, 0, 1, 2);
    buttonLayout->addStretch();

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    buttonLayout->addWidget(cancelButton);
    QPushButton *okButton = new QPushButton(tr("Ok"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    buttonLayout->addWidget(okButton);

    okButton->setDefault(true);
}

QString CreateDockWidgetDialog::objectName() const
{
    return m_objectName->text();
}

Qt::DockWidgetArea CreateDockWidgetDialog::location() const
{
    switch (m_location->currentIndex()) {
        case 0: return Qt::TopDockWidgetArea;
        case 1: return Qt::LeftDockWidgetArea;
        case 2: return Qt::RightDockWidgetArea;
        case 3: return Qt::BottomDockWidgetArea;
        default:
            break;
    }
    return Qt::NoDockWidgetArea;
}

void MainWindow::createDockWidget()
{
    CreateDockWidgetDialog dialog(this);
    int ret = dialog.exec();
    if (ret == QDialog::Rejected)
        return;

    QDockWidget *dw = new QDockWidget;
    dw->setObjectName(dialog.objectName());
    dw->setWindowTitle(dialog.objectName());
    dw->setWidget(new QTextEdit);

    Qt::DockWidgetArea area = dialog.location();
    switch (area) {
        case Qt::LeftDockWidgetArea:
        case Qt::RightDockWidgetArea:
        case Qt::TopDockWidgetArea:
        case Qt::BottomDockWidgetArea:
            addDockWidget(area, dw);
            break;
        default:
            if (!restoreDockWidget(dw)) {
                QMessageBox::warning(this, QString(), tr("Failed to restore dock widget"));
                delete dw;
                return;
            }
            break;
    }

    extraDockWidgets.append(dw);
    destroyDockWidgetMenu->setEnabled(true);
    destroyDockWidgetMenu->addAction(new QAction(dialog.objectName(), this));
}

void MainWindow::destroyDockWidget(QAction *action)
{
    int index = destroyDockWidgetMenu->actions().indexOf(action);
    delete extraDockWidgets.takeAt(index);
    destroyDockWidgetMenu->removeAction(action);
    action->deleteLater();

    if (destroyDockWidgetMenu->isEmpty())
        destroyDockWidgetMenu->setEnabled(false);
}
*/

