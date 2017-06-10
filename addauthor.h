#ifndef ADDAUTHOR_H
#define ADDAUTHOR_H

#include <QDialog>
#include "database.h"

namespace Ui {
class AddAuthor;
}

class AddAuthor : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit AddAuthor(QWidget *parent = 0);
    ~AddAuthor();

private slots:
    void on_cancel_clicked();

    void on_save_clicked();

private:
    Ui::AddAuthor *ui;
};

#endif // ADDAUTHOR_H
