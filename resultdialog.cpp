#include "resultdialog.h"
#include "ui_resultdialog.h"

resultDialog::resultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resultDialog)
{
    newGmae = false; //boolean pour le controle d'un nouveau tour
    finalScore = 0;
    ui->setupUi(this);
    ui->label->setText(text);
}

resultDialog::~resultDialog()
{
    delete ui;
}

void resultDialog::on_pushButton_clicked()
{
    newGmae = true;
    this->close();
}

void resultDialog::afficheScore(int score)
{
    finalScore = score;
    text = "VOTRE SCORE FINAL EST : ";
    text.append(QString::number(finalScore));
    ui->label->setText(text);
}
