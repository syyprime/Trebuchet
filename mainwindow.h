#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>
#include <QPixmap>
#include <QWidget>
#include <QSize>
#include <QImage>
#include <QPainter>
#include <iostream>
#include<cmath>
#include<QTime>
#include "opencv2/opencv.hpp"

#include "leveldialog.h"
#include "resultdialog.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Point getPosition(){return pos;} //obtenir la position la plus possible de la main
    void getCiblePosition();//obtenir la position aléatoire du cible correspondant

    ~MainWindow();

signals:
    void throwstone(); // Il est temps de jeter le projectile
    void movement(); //L'initialiation est finie et on peut controller le mouvement
    void cibleReady(); //Pour L'affichage du cible correspondant

private slots:

    void updateImage(); //renouvellement des frames actuels
    void on_capButton_clicked(); //capturer l'image template de la main de l'utilisateur

    void on_levelButton_clicked();//apparaitre un dialogue pour établir le niveau de difficulté(easy par défaut)
    void showScore();//relié par le signal gameover() pour l'affichage de la note

    void on_resetButton_clicked();//réinitialiser les parametres pour un nouveau coup

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    //levelDialog *dia_level;

    VideoCapture videoCap; // ouverture de la caméra default
    QTimer timer;
    bool capFlag ;
    bool followFlag ;

    Mat srcMat; //frame actuel
    QImage srcQImage; //variable de type QImage de frame actuel
    QPixmap srcQPixmap;//variable de type QPixmap de frame actuel


    Mat templateImage; //template image de la main
    Rect templateRect; // le rectangle vert dans lequel capturer la main
    Rect resultRect; // le rectangle vert dans lequel situé la main detectée
    Mat resultImage; // la matrice de frame après détection
    QImage normResultQImage;// variable de type QImage du frame normalisé après détection
    QPixmap normResultQPixmap;// variable de type QPixmap du frame normalisé après  détection

    Point pos;//la position la plus possible de la main
    int m;//nommbre nous indiquant la vitesse de mouvement vertical de la main

    int frameWidth; // la taille de webcam
    int frameHeight;
    int templateWidth; // la taille de carre
    int templateHeight;

    int sum ;
    int nbCible ;
    int level ; //indiquer le niveau de difficulté [1--easy(par défaut),2--middle,3--hard]
    bool reset ; //boolean de réinitialisation
};

#endif // MAINWINDOW_H
