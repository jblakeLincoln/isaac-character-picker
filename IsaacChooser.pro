#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T00:08:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IsaacChooser
TEMPLATE = app


SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/DailySeedDialog.cpp

HEADERS  += src/MainWindow.h \
    src/DailySeedDialog.h

FORMS    += src/MainWindow.ui \
    src/DailySeedDialog.ui

RESOURCES += \
    resources.qrc

#CONFIG += static
