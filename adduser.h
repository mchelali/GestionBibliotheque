#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>
#include "database.h"

namespace Ui {
class AddUser;
}

class AddUser : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = 0);
    ~AddUser();

private slots:
    void on_cancel_clicked();

    void on_save_clicked();

private:
    Ui::AddUser *ui;
};

#endif // ADDUSER_H
