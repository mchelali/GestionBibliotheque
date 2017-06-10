#ifndef EMPREINTER_H
#define EMPREINTER_H

#include <QDialog>
#include "database.h"
#include <QPainter>
#include <QPrinter>
#include <QPrintPreviewWidget>
#include <QDate>

namespace Ui {
class empreinter;
}

class empreinter : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit empreinter(QWidget *parent = 0);
    ~empreinter();
    void setdata(int id_livre, QString titre, QString sousTitre, QString Domaine, QPixmap cover, QString student, int id_student);

private slots:
    void on_close_clicked();
    void on_confirmation_clicked();
    void quitance(QPrinter *printer);

private:
    Ui::empreinter *ui;
    int id_book;
    QString title,SubTitle,Domain;
    QPixmap cov;
    int id_user;
    QString user;
    QPrinter *printer;
    QPainter *paint;
    QPrintPreviewWidget *printPreview;
    QDate date_emp;
};

#endif // EMPREINTER_H
