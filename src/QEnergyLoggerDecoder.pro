#-------------------------------------------------
#
# Project created by QtCreator 2017-12-02T21:36:12
#
#-------------------------------------------------

QT       += core gui

DEFINES += USE_LOCAL_QCUSTOMPLOT


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QEnergyLoggerDecoder
TEMPLATE = app


SOURCES += main.cpp\
    qcustomplot.cpp \
        qenergyloggerdecoder.cpp \
    eldatahandler.cpp \
    elfileparser.cpp

HEADERS  += qenergyloggerdecoder.h \
    eldatahandler.h \
    elfileparser.h \
    qcustomplot.h
