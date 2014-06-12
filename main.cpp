#include <QApplication>
#include <QSplashScreen>
#include <QThread>


#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);
//    QSplashScreen *splash = new QSplashScreen;
//    splash->setPixmap(QPixmap(":images/bg.png"));
//    splash->show();
    app.setOrganizationName("PMIG Project");
    app.setApplicationName("PMIG");
    MainWindow mainWin;
    mainWin.resize(1024,576);
    mainWin.setWindowState(Qt::WindowMaximized);
    mainWin.show();
    return app.exec();
}
