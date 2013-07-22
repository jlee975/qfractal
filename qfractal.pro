#-------------------------------------------------
#
# Project created by QtCreator 2013-07-20T23:01:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qfractal
TEMPLATE = app


SOURCES += main.cpp \
    docalculation.cpp \
    mandelbrot.cpp \
    qfractalview.cpp \
    qimageview.cpp \
    resizedialog.cpp

HEADERS  += \
    docalculation.h \
    mandelbrot.h \
    qfractalview.h \
    qimageview.h \
    resizedialog.h

FORMS    += \
    qfractalview.ui \
    resizedialog.ui
