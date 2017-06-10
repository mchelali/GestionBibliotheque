#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>
#include <QString>
#include <QByteArray>
#include "database.h"

namespace Ui {
class AddBook;
}

class AddBook : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit AddBook(QWidget *parent = 0);
    ~AddBook();

private slots:
    void on_CancelBook_clicked();
    void on_addCover_clicked();
    void on_ADD_clicked();  
    void on_cancel_clicked();
    void on_add_clicked();
    void on_save_clicked();

private:
    Ui::AddBook *ui;

    QString titre,sousTitre,domaine,nbr_exemp,defBook;
    QByteArray cover;

};

#endif // ADDBOOK_H
