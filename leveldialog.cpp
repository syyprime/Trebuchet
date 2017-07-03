#include "leveldialog.h"
#include "ui_leveldialog.h"

levelDialog::levelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::levelDialog)
{
    level=1;
    ui->setupUi(this);
}

levelDialog::~levelDialog()
{
    delete ui;
}

void levelDialog::on_easyButton_clicked()
{
    level = 1;
    ui->easyButton->setStyleSheet("color: red");
    ui->middleButton->setStyleSheet("color:black");
    ui->hardButton->setStyleSheet("color: black");
}

void levelDialog::on_middleButton_clicked()
{
    level = 2;
    ui->easyButton->setStyleSheet("color: black");
    ui->middleButton->setStyleSheet("color:red");
    ui->hardButton->setStyleSheet("color: black");
}

void levelDialog::on_hardButton_clicked()
{
    level = 3;
    ui->easyButton->setStyleSheet("color: black");
    ui->middleButton->setStyleSheet("color:black");
    ui->hardButton->setStyleSheet("color: red");
}
