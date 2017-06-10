#include "addauthor.h"
#include "ui_addauthor.h"
#include <QDebug>

AddAuthor::AddAuthor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAuthor)
{
    ui->setupUi(this);
}

AddAuthor::~AddAuthor()
{
    delete ui;
}

void AddAuthor::on_cancel_clicked()
{
    close();
}

void AddAuthor::on_save_clicked()
{
    QString name_author=ui->name_autor->text();
    QString prenom=ui->prenom_author->text();
    QString paye=ui->paye_author->text();

    if(name_author!=NULL && prenom!=NULL && paye!=NULL){
        dbConnect();

        QSqlQuery qry;

        qDebug() << "tous les change sont remplis :)";
        //INSERT INTO "main"."Auteur" ("Nom_auteur","Prenom_auteur","Paye_auteur") VALUES (?1,?2,?3)
        qry.prepare("INSERT INTO 'Auteur' ('Nom_auteur', 'Prenom_auteur', 'Paye_auteur') "
                    "VALUES (:nom, :prenom, :paye)");
        qry.bindValue(":nom", name_author);
        qry.bindValue(":prenom", prenom);
        qry.bindValue(":paye", paye);
        qry.exec();

    }
    dbclose();

    close();
}
