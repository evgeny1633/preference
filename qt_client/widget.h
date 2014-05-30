#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QMenuBar *mainbar;

    QMenu *bullet;

    QHBoxLayout *basicgrid;
    QVBoxLayout *lv_grid;
    QHBoxLayout *lh_grid;
    QVBoxLayout *rv_grid;
};

#endif // WIDGET_H
