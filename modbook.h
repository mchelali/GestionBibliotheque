#ifndef MODBOOK_H
#define MODBOOK_H

#include <QDialog>
#include "database.h"
#include <QSqlQuery>
#include <QDebug>
#include <QByteArray>

namespace Ui {
class modbook;
}

class modbook : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit modbook(QWidget *parent = 0, QString row="");
    ~modbook();

private slots:
    void on_cancel_clicked();

    void on_seveMod_clicked();

    void on_addCover_clicked();

private:
    Ui::modbook *ui;
    int idBook;
    QByteArray cover;
};

#endif // MODBOOK_H
