#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "openglwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    templateWidth=64;
    templateHeight=64;

    sum = 0;
    nbCible = 10;
    level = 0;
    reset = false;
    capFlag = false;
    followFlag = false;
    ui->setupUi(this);
    if(level == 0) ui->levellabel->setText("cliquez le button level pour choisir un niveau");

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(updateImage()));
    QObject::connect( this,SIGNAL(throwstone()), ui->openGLWidget, SLOT(throwout()) );
    QObject::connect( this,SIGNAL(movement()), ui->openGLWidget, SLOT(rotate()) );
    QObject::connect( this,SIGNAL(cibleReady()), ui->openGLWidget, SLOT(setUpCible()) );
    QObject::connect( ui->openGLWidget,SIGNAL(gameOver()), this, SLOT(showScore()) );

    timer.start(10);
    m=0;
    if(videoCap.open(0))
    {
       frameWidth=videoCap.get(CV_CAP_PROP_FRAME_WIDTH);
        frameHeight=videoCap.get(CV_CAP_PROP_FRAME_HEIGHT);
        frameWidth = frameWidth/2;
        frameHeight = frameHeight/2;
        videoCap.set(CV_CAP_PROP_FRAME_WIDTH,frameWidth/2);
        videoCap.set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight/2);

    }
    else
    {
        ui->frameLabel->setText("camera error");
        this->close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage()
{
    videoCap>>srcMat;
    if(!capFlag)
    {
        // basic treatment
        cv::resize(srcMat,srcMat,Size(frameWidth,frameHeight),0,0,CV_INTER_LINEAR);
        flip(srcMat,srcMat,1);
        cvtColor(srcMat, srcMat, CV_BGR2RGB);

        templateRect = Rect((frameWidth-templateWidth)/2,(frameHeight-templateHeight)/3,templateWidth,templateHeight);
        cv::rectangle(srcMat,templateRect,Scalar( 0, 0, 255),2,8,0);

        // get images of different formats
        srcQImage = QImage((uchar*)(srcMat.data), srcMat.cols, srcMat.rows, QImage::Format_RGB888);
        srcQPixmap = QPixmap::fromImage(srcQImage);

        ui->frameLabel->setPixmap(srcQPixmap);
    }


    if(capFlag)
    {
        cv::resize(srcMat,srcMat,Size(frameWidth,frameHeight),0,0,CV_INTER_LINEAR);

        flip(srcMat,srcMat,1);
        cvtColor(srcMat, srcMat, CV_BGR2RGB);

        //Mat resultImage;    // to store the matchTemplate result
        int result_cols =  srcMat.cols - templateImage.cols + 1;
        int result_rows = srcMat.rows - templateImage.rows + 1;
        resultImage.create( result_cols, result_rows, CV_32FC1 );

        // Do the Matching between the frame and the templateImage
        matchTemplate( srcMat, templateImage, resultImage, TM_CCORR_NORMED );

        // Localize the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        // Save the location fo the matched rect
        resultRect=Rect(maxLoc.x,maxLoc.y,templateWidth,templateHeight);
        cv::rectangle(srcMat,resultRect,Scalar( 0, 0,255),2,8,0);

        if((maxLoc.y > pos.y+3) && maxLoc.y >= 90)
        {
            followFlag = true;
        }

        if(followFlag)
        {
            if(maxLoc.x>pos.x+3){
                ui->openGLWidget->rightFlag = true;
                emit movement();
            }
            if(maxLoc.x+3 < pos.x){
                ui->openGLWidget->leftFlag = true;
                emit movement();
            }
            if(maxLoc.y>=130)
            {
                if(m<4)
                {
                    emit throwstone();
                    //ui->openGLWidget->speed=3-0.2*(m+1);
                    ui->openGLWidget->stopFlag=true;
                }
                //emit throwstone();
            }
            if(!ui->openGLWidget->stopFlag)
            if(maxLoc.y < pos.y-3)
            {
                m =0;
                ui->openGLWidget->upFlag = true;
                emit movement();
            }
            else
            {
    //            if(maxLoc.y==pos.y){
    //                m=0;
    //            }
                m += 1;
            }
            if(maxLoc.y > pos.y+3)
            {
                ui->openGLWidget->downFlag = true;
                emit movement();
            }
            pos = maxLoc;

            cv::rectangle(srcMat,resultRect,Scalar( 0, 255, 0),2,8,0);
        }


        // Show the result
        Mat normResultImage;
        // Normalize values
        normalize(resultImage,normResultImage,1,0,NORM_MINMAX);\
        cvtColor(normResultImage,normResultImage,CV_GRAY2RGB);
        //rectangle(normResultImage,resultRect,Scalar( 0, 0, 1),2,8,0);

        srcQImage = QImage((uchar*)(srcMat.data), srcMat.cols, srcMat.rows, QImage::Format_RGB888);
        srcQPixmap = QPixmap::fromImage(srcQImage);

        ui->frameLabel->setPixmap(srcQPixmap);

 //       ui->label->setText(QString::number(ui->openGLWidget->angle1));
    }
}

void MainWindow::on_capButton_clicked()
{
    videoCap>>srcMat;
    if(srcMat.data)
    {
        // basic treatment
        cv::resize(srcMat,srcMat,Size(frameWidth,frameHeight),0,0,CV_INTER_LINEAR);
        flip(srcMat,srcMat,1);
        cvtColor(srcMat, srcMat, CV_BGR2RGB);
        templateImage=Mat(srcMat,templateRect);
        capFlag = true;
    }

}

void MainWindow::on_levelButton_clicked()
{
    levelDialog *dia_level = new levelDialog();
    dia_level->exec();
    level = dia_level->getLevel();

    if(level == 1) ui->levellabel->setText("VOTRE NIVEAU : EASY");
    if(level == 2) ui->levellabel->setText("VOTRE NIVEAU : MIDDLE");
    if(level == 3) ui->levellabel->setText("VOTRE NIVEAU : HARD");

    getCiblePosition();
    emit cibleReady();

    QString scoreText = "votre score est : ";
    scoreText.append(QString::number(ui->openGLWidget->score));
    ui->scoreLabel->setText(scoreText);
}

void MainWindow::getCiblePosition()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    double dis = 3 +3*level+double(qrand())/(RAND_MAX + 1)*2-1;

    double anglet=double(qrand())/(RAND_MAX + 1)*M_PI+M_PI/2;
    ui->openGLWidget->tx = dis*cos(anglet);
    ui->openGLWidget->ty = dis*sin(anglet);
    //ui->label->setText(QString::number(double(qrand())/(RAND_MAX + 1)));

//    nbCible = nbCible -1;
//    QString cibleText = "Cible restant : ";
//    cibleText.append(QString::number(nbCible));
//    ui->cibleLabel->setText(cibleText);

    if(nbCible==0)
    {
        resultDialog *result_level = new resultDialog();


        result_level->finalScore = sum;

        result_level->afficheScore(sum);

        QString text = "VOTRE SCORE FINAL EST : ";
        text.append(QString::number(sum));
        result_level->text.append(text);
        result_level->exec();

        if(result_level->newGmae)
        {
            capFlag = false;
            followFlag = false;
            nbCible = 11;
            ui->openGLWidget->reset();
            getCiblePosition();
            emit cibleReady();
            this->update();
            result_level->newGmae = false;
        }
    }

    nbCible = nbCible -1;
    QString cibleText = "Cible restant : ";
    cibleText.append(QString::number(nbCible));
    ui->cibleLabel->setText(cibleText);
}

void MainWindow::showScore(){
    QString text = "votre score est : ";
    text.append(QString::number(ui->openGLWidget->score));
    ui->scoreLabel->setText(text);
    sum = sum + ui->openGLWidget->score;
}


void MainWindow::on_resetButton_clicked()
{
    capFlag = false;
    followFlag = false;
    nbCible = 10;
    ui->openGLWidget->reset();
    getCiblePosition();
    emit cibleReady();
    this->update();
}

void MainWindow::on_pushButton_clicked()
{
    capFlag = false;
    followFlag = false;
    ui->openGLWidget->reset();
    getCiblePosition();
    emit cibleReady();
    this->update();
}
