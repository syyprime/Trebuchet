#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>


namespace Ui {
class resultDialog;
}

class resultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit resultDialog(QWidget *parent = 0);
    ~resultDialog();
    void afficheScore(int score); //l'affichage de la note finale de 10 coups
    bool newGmae ; //boolean pour le controle d'un nouveau tour
    int finalScore; //la note finale
    QString text; //le texte indiquant des resultats finals

private slots:
    void on_pushButton_clicked(); //recommencer un nouveau tour

private:
    Ui::resultDialog *ui;

};

#endif // RESULTDIALOG_H
