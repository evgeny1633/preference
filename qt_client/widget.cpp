#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //this->setFixedSize(800,600);
    this->setWindowTitle("Potissimum");

    //Сетка приложения

    //Сетка аватара
    ava_grid = new QHBoxLayout();

    QPixmap avatar;
    avatar.load(":/avatars/default.jpg");

    ava = new QLabel();
    ava -> resize(avatar.size());
    ava -> setPixmap(avatar);

    ava_grid -> addWidget(ava);
    ava_grid -> addStretch(2);

    //Сетка обновления

    upd_grid = new QHBoxLayout();

    //QDate up_date = QDate::currentDate();
    up_date = new QDateTime(QDateTime::currentDateTime());
    QString str = up_date->toString(Qt::LocalDate);

    update_button = new QPushButton("last update");
    update_label = new QLabel(str);

    upd_grid -> addWidget(update_button);
    upd_grid -> addWidget(update_label);

    // Левая панель

    lv_grid = new QVBoxLayout();
    lv_grid -> addStretch(1);
    lv_grid -> addLayout(ava_grid);
    lv_grid -> addStretch(5);
    lv_grid -> addLayout(upd_grid);

    rv_grid = new QVBoxLayout();

    frame = new QFrame();
    frame -> setFrameStyle(QFrame::VLine | QFrame::Raised);

    basicgrid = new QHBoxLayout(this);
    basicgrid -> addLayout(lv_grid);
    basicgrid -> addWidget(frame);
    basicgrid -> addStretch(5);
    basicgrid -> addLayout(rv_grid);

    //Верхняя полоска

    mainbar = new QMenuBar(this);

    bullet = new QMenu("Пуля");

    mainbar->addMenu(bullet);
    mainbar->show();
}

Widget::~Widget()
{

}
