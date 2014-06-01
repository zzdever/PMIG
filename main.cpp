#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QString str;
    QTextStream stream(&str);
    stream << hex<<100;
    qDebug()<<str;

    return a.exec();
}
