#ifndef MODAUTHOR_H
#define MODAUTHOR_H

#include <QDialog>
#include "database.h"

namespace Ui {
class ModAuthor;
}

class ModAuthor : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit ModAuthor(QWidget *parent = 0, QString id = "");
    ~ModAuthor();

private slots:
    void on_cancel_clicked();

    void on_save_clicked();

private:
    Ui::ModAuthor *ui;
    int idAuthor;
};

#endif // MODAUTHOR_H
