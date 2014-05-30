#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(800,600);
    this->setWindowTitle("Potissimum");

    lh_grid = new QHBoxLayout(this);

    lv_grid = new QVBoxLayout(this);
    lv_grid -> addLayout(lh_grid);
    lv_grid -> addStretch(1);

    rv_grid = new QVBoxLayout(this);

    basicgrid = new QHBoxLayout(this);
    basicgrid -> addLayout(lv_grid);
    basicgrid -> addStretch(1);
    basicgrid -> addLayout(rv_grid);

    mainbar = new QMenuBar(this);

    bullet = new QMenu("Пуля");

    mainbar->addMenu(bullet);
    mainbar->show();
}

Widget::~Widget()
{

}
