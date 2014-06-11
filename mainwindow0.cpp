#if 0
//! [0]
#include <QtWidgets>
#include <QPainter>
#include <QBrush>
#include "mainwindow.h"

#include <QDebug>

#define TILE_SIZE 100


//! [0]

//! [1]
MainWindow::MainWindow():
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene,this))
//! [1] //! [2]
{
//    QDockWidget *w = new QDockWidget();

//    marqueeAct = new QAction(QIcon(":images/marquee-square.png"),tr("&Marquee tool (M)"),this);
//    marqueeAct->setStatusTip(tr("To select an area"));

//    brushAct = new QAction(QIcon(":images/bg.png"),tr("&Brush tool (B)"),this);
//    brushAct->setStatusTip(tr("To brush strokes"));

//    w->addAction(marqueeAct);
//    w->addAction(brushAct);
//    addDockWidget(Qt::LeftDockWidgetArea, w);

    //scene->setSceneRect(0,0,500,500);
    setCentralWidget(view);
    QPixmap bg(TILE_SIZE, TILE_SIZE);
    QPainter bgPainter(&bg);
    bgPainter.setPen(QPen(Qt::white,0));
    bgPainter.setBrush(QBrush(QColor(245,245,245)));
    bgPainter.drawRect(0, 0, TILE_SIZE/2, TILE_SIZE/2);
    bgPainter.setBrush(QBrush(Qt::white));
    bgPainter.drawRect(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2);
    bgPainter.setBrush(QBrush(Qt::white));
    bgPainter.drawRect(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2);
    bgPainter.setBrush(QBrush(QColor(245,245,245)));
    bgPainter.drawRect(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2);
    view->setBackgroundBrush(QBrush(bg));

    bgImg.load(":images/bg.png");
    scene->addPixmap(bgImg);
textEdit = new QPlainTextEdit;

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

//    connect(textEdit->document(), SIGNAL(contentsChanged()),
//            this, SLOT(documentWasModified()));

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}
//! [2]

//! [3]
void MainWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
//! [4]

//! [5]
void MainWindow::newFile()
//! [5] //! [6]
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }
}
//! [6]

//! [7]
void MainWindow::open()
//! [7] //! [8]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}
//! [8]

//! [9]
bool MainWindow::save()
//! [9] //! [10]
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}
//! [10]

//! [11]
bool MainWindow::saveAs()
//! [11] //! [12]
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}
//! [12]

//! [13]
void MainWindow::about()
//! [13] //! [14]
{
    QMessageBox::about(this, tr("About PMIG"),
             tr("<b>PMIG</b> is a simple image "
                "processing tool.\nIf you are curious about the name, "
                "it's a tricky reverse of the powerful open software GIMP."));
}
//! [14]

//! [15]
void MainWindow::documentWasModified()
//! [15] //! [16]
{
    //setWindowModified(textEdit->document()->isModified());
}
//! [16]

//! [17]
void MainWindow::createActions()
//! [17] //! [18]
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

//! [19]
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
//! [18] //! [19]

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

//! [20]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
//! [20]
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

//! [21]
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
//! [21]
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//! [22]
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
//! [22]

//! [23]
    cutAct->setEnabled(false);
//! [23] //! [24]
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));


    toolBoxGroup = new QActionGroup(this);
    toolBoxGroup->setExclusive(true);

    marqueeAct = new QAction(QIcon(":images/marquee-square.png"),tr("&Marquee tool (M)"),this);\
    marqueeAct->setShortcut(Qt::Key_M);
    marqueeAct->setStatusTip(tr("To select an area"));
    marqueeAct->setCheckable(true);
    connect(marqueeAct,SIGNAL(triggered()),this,SLOT(switchTopToolBar()));

    brushAct = new QAction(QIcon(":images/brush-tool.png"),tr("&Brush tool (B)"),this);
    brushAct->setShortcut(Qt::Key_B);
    brushAct->setStatusTip(tr("To brush strokes"));
    brushAct->setCheckable(true);

    toolBoxGroup->addAction(marqueeAct);
    toolBoxGroup->addAction(brushAct);
}
//! [24]

//! [25] //! [26]
void MainWindow::createMenus()
//! [25] //! [27]
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
//! [28]
    fileMenu->addAction(openAct);
//! [28]
    fileMenu->addAction(saveAct);
//! [26]
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
//! [27]

//! [29] //! [30]
void MainWindow::createToolBars()
{
    toolBox = addToolBar(tr("Tool box"));
    addToolBar(Qt::LeftToolBarArea, toolBox);
    toolBox->setAllowedAreas(Qt::LeftToolBarArea);
    toolBox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    toolBox->setOrientation(Qt::Vertical);
    toolBox->setFloatable(false);
    toolBox->setMovable(false);


//! [29] //! [31]
    toolBox->addAction(marqueeAct);
    toolBox->addAction(brushAct);
//! [31]

//    editToolBar = addToolBar(tr("Edit"));
//    editToolBar->addAction(cutAct);
//    editToolBar->addAction(copyAct);
//    editToolBar->addAction(pasteAct);
}
//! [30]

void MainWindow::switchTopToolBar()
{
//    toolBox->setDisabled(true);
    toolBox->setHidden(true);
    toolBox->setVisible(false);
    qDebug()<<sender()->objectName();
    update();
}

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    statusBar()->showMessage(tr("Ready"));
}
//! [33]

//! [34] //! [35]
void MainWindow::readSettings()
//! [34] //! [36]
{
    QSettings settings("QtProject", "PMIG");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 450)).toSize();
    resize(size);
    move(pos);
}
//! [35] //! [36]

//! [37] //! [38]
void MainWindow::writeSettings()
//! [37] //! [39]
{
    QSettings settings("QtProject", "PMIG");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
{
    if(0){// (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("PMIG"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
//! [41]

//! [42]
void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("PMIG"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [43]

//! [44]
bool MainWindow::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("PMIG"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
//! [45]

//! [46]
void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    //textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled";
    setWindowFilePath(shownName);
}
//! [47]

//! [48]
QString MainWindow::strippedName(const QString &fullFileName)
//! [48] //! [49]
{
    return QFileInfo(fullFileName).fileName();
}
//! [49]


#endif
