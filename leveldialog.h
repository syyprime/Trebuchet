#ifndef LEVELDIALOG_H
#define LEVELDIALOG_H

#include <QDialog>


namespace Ui {
class levelDialog;
}

class levelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit levelDialog(QWidget *parent = 0);
    ~levelDialog();
    int getLevel(){return level;} //obtenir le niveau de difficulté

private slots:
    void on_easyButton_clicked(); //établir le niveau facile de difficulté

    void on_middleButton_clicked(); //établir le niveau moyen de difficulté

    void on_hardButton_clicked(); //établir le niveau difficile de difficulté

private:
    Ui::levelDialog *ui;
    int level; //indiquer le niveau de difficulté [1--easy(par défaut),2--middle,3--hard]
};

#endif // LEVELDIALOG_H
