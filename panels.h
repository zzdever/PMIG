#ifndef PANELS_H
#define PANELS_H

#include <QDockWidget>
#include <QFrame>

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QActionGroup)
QT_FORWARD_DECLARE_CLASS(QMenu)

class Panel : public QDockWidget
{
    Q_OBJECT

public:
    explicit Panel(const QString &panelName, QWidget *parent = 0, Qt::WindowFlags flags = 0);

    QMenu *menu;
    QAction *windowWidgetAction;

protected:
    virtual void resizeEvent(QResizeEvent *e);


};

class PanelFrame : public QFrame
{
    Q_OBJECT
public:
    PanelFrame(const QString &panelName, QWidget *parent);


public slots:

protected:
    void paintEvent(QPaintEvent *);
};

#endif // PANELS_H
