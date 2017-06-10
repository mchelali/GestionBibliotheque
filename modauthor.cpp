#include "modauthor.h"
#include "ui_modauthor.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ModAuthor::ModAuthor(QWidget *parent, QString id) :
    QDialog(parent),
    ui(new Ui::ModAuthor)
{
    ui->setupUi(this);
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT ID_auteur,Nom_auteur,Prenom_auteur FROM Auteur WHERE ID_auteur= :info OR Nom_auteur= :info OR Prenom_auteur= :info");
    qry.bindValue(":info",id);
    if (!qry.exec()) qDebug() << qry.lastError();
    qry.next();
    idAuthor = qry.value(0).toInt();

    qry.prepare("SELECT ID_auteur,Nom_auteur,Prenom_auteur,Paye_auteur FROM Auteur WHERE ID_auteur="+QString::number(idAuthor));
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    }else{
        qry.next();

        ui->name_autor->setText( qry.value(1).toString());
        ui->prenom_author->setText( qry.value(2).toString());
        ui->paye_author->setText( qry.value(3).toString());
    }


    dbclose();
}

ModAuthor::~ModAuthor()
{
    delete ui;
}

void ModAuthor::on_cancel_clicked()
{
    close();
}

void ModAuthor::on_save_clicked()
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("UPDATE Auteur "
                "SET Nom_auteur=:nom, Prenom_auteur=:prenom, Paye_auteur=:paye "
                "WHERE ID_auteur="+QString::number(idAuthor));
    qry.bindValue(":nom", ui->name_autor->text());
    qry.bindValue(":prenom", ui->prenom_author->text());
    qry.bindValue(":paye", ui->paye_author->text());

    if(!qry.exec()) qDebug() << qry.lastError();

    dbclose();

    close();

}
