#-------------------------------------------------
#
# Project created by QtCreator 2014-05-29T23:31:50
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_client
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    statistics.cpp

HEADERS  += widget.h \
    statistics.h

RESOURCES += \
    avatars.qrc
