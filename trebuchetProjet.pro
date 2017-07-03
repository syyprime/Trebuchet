#-------------------------------------------------
#
# Project created by QtCreator 2017-05-19T09:42:46
#
#-------------------------------------------------

#QT       += core gui
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = trebuchetProjet
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    leveldialog.cpp \
    resultdialog.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    leveldialog.h \
    resultdialog.h

FORMS    += mainwindow.ui \
    leveldialog.ui \
    resultdialog.ui

INCLUDEPATH += C:\opencv\build\install\include
LIBS += -LC:\opencv\build\install\x86\mingw\lib \
 -lopencv_core2413 \
 -lopencv_highgui2413 \
 -lopencv_imgproc2413 \
 -lopencv_features2d2413 \
 -lopencv_calib3d2413

RESOURCES += \
    logo.qrc
