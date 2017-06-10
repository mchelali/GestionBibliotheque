#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include "addbook.h"
#include "modbook.h"
#include "adduser.h"
#include "moduser.h"
#include <QSqlQueryModel>
#include <piechart.h>
#include <QMessageBox>
#include <QSqlError>
#include "addauthor.h"
#include "modauthor.h"
#include "empreinter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cancel_clicked()
{
    close();
}

void MainWindow::on_connect_clicked()
{
    QString pseudo,password;

    pseudo = ui->peudo->text();
    password = ui->password->text();

    if (!dbConnect()){
            qDebug() << " not  Connected to database";
            return;
        }

        QSqlQuery qry;
        qry.prepare("SELECT Pseudo_ad,Password_ad,Rang_ad,ID_adherent,Nom_ad,Prenom_ad FROM Adherent WHERE Pseudo_ad='"+pseudo+
                    "' AND Password_ad='"+password+"';");

        if (qry.exec()){

            if(qry.next()){

                qDebug() << "username : " << qry.value(0).toString() << "\npassword : "
                         << qry.value(1).toString() <<
                          "admin "<< qry.value(2);
                if (qry.value(2).toString()=="Admin"){
                    ui->stackedWidget->setCurrentIndex(1);

                    //count the total of Domaine
                    qry.prepare("SELECT COUNT(*) FROM (SELECT Domaine,COUNT(*) FROM Livres GROUP BY (Domaine));");
                    qry.exec();
                    qry.next();
                    QVector<double> domaine(qry.value(0).toInt());
                    QVector<QColor> colors(qry.value(0).toInt());
                    QVector<QString> info(qry.value(0).toInt());

                    qry.prepare("SELECT Domaine,COUNT(*) FROM Livres GROUP BY (Domaine)");
                    qry.exec();
                    int i=0;

                    while (qry.next()) {
                        domaine[i] = qry.value(1).toFloat();
                        colors[i] = QColor(qrand()%256, qrand()%256, qrand()%256, 255);
                        info[i] = qry.value(0).toString();
                        i++;
                    }
                    qDebug() << colors;
                    ui->Piewidget->setData(info, domaine, colors);
                }else{
                    ui->stackedWidget->setCurrentIndex(2);

                    user = qry.value(4).toString()+" "+qry.value(5).toString();
                    id = qry.value(3).toInt();
                    ui->user->setText("Bienvenue cher "+user);

                    //view data in the listview
                    QSqlQueryModel *modal=new QSqlQueryModel();
                    //QSqlQuery qry;

                    qry.prepare("SELECT Titre,SousTitre,Domaine FROM Livres");
                    qry.exec();
                    modal->setQuery(qry);
                    ui->BookView->setModel(modal);
                    ui->BookView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);

                }

                dbclose();

            }else{

                qDebug() << "username or password are incorrect";
                dbclose();

            }
        }
        ui->peudo->setText("");
        ui->password->setText("");
}

void MainWindow::on_log_out2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    // Coté admin pour la gestion des ressoures :)
    if (!(ui->listWidget->row(item)==5)){
        //Aller vers la prtie selectioné :) (Livres, Auteurs....) pour la gestion
        int partie =  ui->listWidget->row(item);
        ui->stackedWidget_2->setCurrentIndex(partie);

        dbConnect();

        QSqlQuery qry;
        QSqlQueryModel *model=new QSqlQueryModel();

        switch (partie){
            case 0:{
            //count the total of Domaine
                qry.prepare("SELECT COUNT(*) FROM (SELECT Domaine,COUNT(*) FROM Livres GROUP BY (Domaine));");
                qry.exec();
                qry.next();
                QVector<double> domaine(qry.value(0).toInt());
                QVector<QColor> colors(qry.value(0).toInt());
                QVector<QString> info(qry.value(0).toInt());
                qry.prepare("SELECT Domaine,COUNT(*) FROM Livres GROUP BY (Domaine)");
                qry.exec();
                int i=0;

                while (qry.next()) {
                    domaine[i] = qry.value(1).toFloat();
                    colors[i] = QColor(qrand()%256, qrand()%256, qrand()%256, 255);
                    info[i] = qry.value(0).toString();
                    i++;
                }
                qDebug() << colors;
                ui->Piewidget->setData(info, domaine, colors);
            }
            case 1:{
                //qDebug() << "1-Oglet livre";
                qry.prepare("SELECT * FROM Livres");
                qry.exec();
                model->setQuery(qry);
                ui->AdminBook->setModel(model);
                ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
                break;
            }
            case 2:{
                //liste des livres emprunté
                qry.prepare("SELECT * FROM Emprunte");
                qry.exec();
                model->setQuery(qry);
                ui->TakeBook->setModel(model);
                ui->TakeBook->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                break;
            }
            case 3:{
                //qDebug() << "2-Oglet users";
                qry.prepare("SELECT * FROM Adherent");
                qry.exec();
                model->setQuery(qry);
                ui->AdminUsers->setModel(model);
                ui->AdminUsers->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
                break;
            }
            case 4: {
                //qDebug() << "3-Oglet autuer ";
                qry.prepare("SELECT * FROM Auteur");
                qry.exec();
                model->setQuery(qry);
                ui->AdminAuthor->setModel(model);
                ui->AdminAuthor->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
                break;
            }
        }
    }else{
        // Revenir vers la page d'indentification
        ui->stackedWidget->setCurrentIndex(0);
    }
    dbclose();
}

void MainWindow::on_addBook_clicked()
{
    //Ajouter un livre dans la partie administration
    AddBook window(this);
    window.setModal(true);
    if (window.exec()){
        qDebug()<< "ajouter un livre";
    }
    dbConnect();
    QSqlQuery qry;
    //view data in the listview
    QSqlQueryModel *modal=new QSqlQueryModel();
    //QSqlQuery qry;

    qry.prepare("SELECT * FROM Livres");
    qry.exec();
    modal->setQuery(qry);
    ui->AdminBook->setModel(modal);
    ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_BookView_clicked(const QModelIndex &index)
{
    //dans cette fonciton on vas afficher les info d'un livre  dans la partie utilisateur
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT Titre,Cover,resumer,SousTitre,Domaine,ID_livre FROM Livres WHERE Titre='"+index.sibling(index.row(), 0).data().toString()+"' AND SousTitre='"+index.sibling(index.row(), 1).data().toString()+"'");
    qry.exec();

    qry.next();
    QByteArray array = qry.value(1).toByteArray();
    QPixmap cov;
    cover.loadFromData(array);
    cov.loadFromData(array);

    ui->Couverture->setPixmap(cov);
    ui->Couverture->setScaledContents(true);

    titre=qry.value(0).toString();
    soustitre=qry.value(3).toString();
    domaine=qry.value(4).toString();
    id_book = qry.value(5).toInt();

    ui->titre->setText(qry.value(0).toString());
    ui->description->setText(qry.value(2).toString());
    dbclose();

}

void MainWindow::on_mod_book_clicked()
{
    //Modifier un livre selectionner
    //this return QModelIndex -------> row =ui->AdminBook->selectionModel()->currentIndex();
    QString val = ui->AdminBook->model()->data(ui->AdminBook->selectionModel()->currentIndex()).toString();
    modbook win(0,val);
    win.setModal(true);
    if(win.exec()){

    }
    //raffrechir l'affichage
    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *modal= new QSqlQueryModel();
    qry.prepare("SELECT * FROM Livres");
    qry.exec();
    modal->setQuery(qry);
    ui->AdminBook->setModel(modal);
    ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}


void MainWindow::on_deleteBook_clicked()
{
    //supprimer un livre----- coté admin

    // en premier lieu, rrecuperer le Id du livre
    QString val= ui->AdminBook->model()->data( ui->AdminBook->currentIndex()).toString();
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT ID_livre,Titre,SousTitre FROM Livres WHERE (ID_livre= :info OR Titre= :info OR SousTitre= :info)");
    qry.bindValue(":info", val);
    if(!qry.exec()) qDebug() << qry.lastError();
    qry.next();
    int row = qry.value(0).toInt();

    //supprimer un livre----- coté admin
    QMessageBox::StandardButton msgBox;
    msgBox= QMessageBox::question(this, "supprimer livre", "Voullez vous vraiment supprimer le livre "+qry.value(1).toString()+", "+qry.value(2).toString(),
                                  QMessageBox::Yes|QMessageBox::No);
    if(msgBox == QMessageBox::Yes){

        QSqlQuery qry;
        qry.prepare("DELETE FROM Livres WHERE ID_livre="+QString::number(row));
        qry.exec();

        QSqlQueryModel *modal=new QSqlQueryModel();
        qry.prepare("SELECT * FROM Livres");
        qry.exec();
        modal->setQuery(qry);
        ui->AdminBook->setModel(modal);
        ui->AdminBook->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);

    }
    dbclose();
}

void MainWindow::on_recherche_clicked()
{
    // Bouton pour la recherche d'unlivre coté utilisateur
    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *modal=new QSqlQueryModel();

    qDebug() << ui->domaine->currentText();
    qry.prepare("SELECT Titre,SousTitre,Domaine,Note FROM Livres WHERE Titre LIKE :ti AND Domaine=:dom");
    qry.bindValue(":ti", "%"+ui->titre_livre->text()+"%");
    qry.bindValue(":dom", ui->domaine->currentText());
    qry.exec();

    modal->setQuery(qry);
    ui->BookView->setModel(modal);
    ui->BookView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_addUser_clicked()
{
    // Affichage de la fenaitre pour ajouter un utilisateur
    AddUser add;
    add.setModal(true);
    if (add.exec()){

    }
    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *model= new QSqlQueryModel();
    qry.prepare("SELECT * FROM Adherent");
    qry.exec();
    model->setQuery(qry);
    ui->AdminUsers->setModel(model);
    ui->AdminUsers->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_DelUser_clicked()
{
    //supprimer un Utilisateur ----- coté admin
    // en premier lieu, rrecuperer le Id du livre
    QString val= ui->AdminUsers->model()->data( ui->AdminUsers->currentIndex()).toString();
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT ID_adherent,Pseudo_ad,Nom_ad,Prenom_ad,Date_Ins FROM Adherent WHERE (ID_adherent= :info OR Pseudo_ad= :info OR Nom_ad= :info OR Prenom_ad= :info OR Date_Ins= :info)");
    qry.bindValue(":info", val);
    if(!qry.exec()) qDebug() << qry.lastError();
    qry.next();
    int row = qry.value(0).toInt();


    QMessageBox::StandardButton msgBox;
    msgBox= QMessageBox::question(this, "supprimer utilisateur", "Voullez vous vraiment supprimer cet utilisateur ??",
                                  QMessageBox::Yes|QMessageBox::No);
    if(msgBox == QMessageBox::Yes){
        QSqlQuery qry;
        qry.prepare("DELETE FROM Adherent WHERE ID_adherent="+QString::number(row));
        qry.exec();

        QSqlQueryModel *modal=new QSqlQueryModel();
        qry.prepare("SELECT * FROM Adherent");
        qry.exec();
        modal->setQuery(qry);
        ui->AdminUsers->setModel(modal);
        ui->AdminUsers->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);

    }
    dbclose();
}

void MainWindow::on_modUser_clicked()
{
    //modifier lesinformation d'un utilisateur
    QString val = ui->AdminUsers->model()->data(ui->AdminUsers->selectionModel()->currentIndex()).toString();
    ModUser win(0, val);
    win.setModal(true);
    if (win.exec()){
        qDebug() << "modification en cour";
    }
    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *modal=new QSqlQueryModel();
    qry.prepare("SELECT * FROM Adherent");
    qry.exec();
    modal->setQuery(qry);
    ui->AdminUsers->setModel(modal);
    ui->AdminUsers->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_addAuthor_clicked()
{
    AddAuthor win;
    win.setModal(true);
    if(win.exec()){

    }
    //rafrichir l'affichage
    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *model= new QSqlQueryModel();

    qry.prepare("SELECT * FROM Auteur");
    qry.exec();
    model->setQuery(qry);
    ui->AdminAuthor->setModel(model);
    ui->AdminAuthor->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_DelAuthor_clicked()
{
    QString val= ui->AdminAuthor->model()->data(ui->AdminAuthor->currentIndex()).toString();
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT ID_auteur,Nom_auteur,Prenom_auteur FROM Auteur WHERE ID_auteur= :info OR Nom_auteur= :info OR Prenom_auteur= :info");
    qry.bindValue(":info",val);
    if (!qry.exec()) qDebug() << qry.lastError();
    qry.next();
    int idAuthor = qry.value(0).toInt();
    qry.prepare("DELETE FROM Auteur WHERE ID_auteur="+QString::number(idAuthor));
    if(!qry.exec()) qDebug() << qry.lastError();

    //rafrechir l'affichage
    QSqlQueryModel *model= new QSqlQueryModel();

    qry.prepare("SELECT * FROM Auteur");
    qry.exec();
    model->setQuery(qry);
    ui->AdminAuthor->setModel(model);
    ui->AdminAuthor->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_modAuthor_clicked()
{
    QString val = ui->AdminAuthor->model()->data( ui->AdminAuthor->currentIndex()).toString();

    ModAuthor win(0, val);
    win.setModal(true);
    if (win.exec()){

    }

    dbConnect();
    //rafrechir l'affichage
    QSqlQueryModel *model= new QSqlQueryModel();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM Auteur");
    qry.exec();
    model->setQuery(qry);
    ui->AdminAuthor->setModel(model);
    ui->AdminAuthor->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch);
    dbclose();
}

void MainWindow::on_emp_clicked()
{
    // empreinter le livre selectionné
    empreinter win;
    win.setdata(id_book ,titre, soustitre, domaine, cover, user, id);
    win.setModal(true);
    win.exec();

}

void MainWindow::on_TakeBook_clicked(const QModelIndex &index)
{
    //Affiche le livre emprunté et l'etudiant qui la prit
    qDebug() << index.sibling(index.row(), 0).data();//IdEmp
    qDebug() << index.sibling(index.row(), 1).data();//ID_Adherent
    qDebug() << index.sibling(index.row(), 2).data();//ID_Livre
    qDebug() << index.sibling(index.row(), 3).data();//dateEmp
    qDebug() << index.sibling(index.row(), 4).data();//dateRend
    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT Nom_ad,Prenom_ad,Rang_ad FROM Adherent WHERE ID_Adherent="+index.sibling(index.row(), 1).data().toString());
    qry.exec();
    qry.next();
    ui->NomEtudiant->setText(qry.value(0).toString());
    ui->PrenomEtudiant->setText(qry.value(1).toString());
    ui->RangEtudiant->setText(qry.value(2).toString());

    qry.prepare("SELECT Titre,SousTitre,Domaine,Cover FROM Livres WHERE ID_livre="+index.sibling(index.row(), 2).data().toString());
    qry.exec();
    qry.next();
    ui->TitreLivre->setText(qry.value(0).toString());
    ui->SubLivre->setText(qry.value(1).toString());
    ui->DomLivre->setText(qry.value(2).toString());

    QByteArray array = qry.value(3).toByteArray();
    QPixmap cover= QPixmap();
    cover.loadFromData(array);
    ui->CovLivre->setPixmap(cover);
    ui->CovLivre->setScaledContents(true);

    dbclose();

}

void MainWindow::on_rendreBook_clicked()
{
    // Rendre le livre emprunté
    qDebug() << ui->TakeBook->currentIndex().sibling(ui->TakeBook->currentIndex().row(), 0).data();//IdEmp
    qDebug() << ui->TakeBook->currentIndex().sibling(ui->TakeBook->currentIndex().row(), 1).data();//ID_Adehrent
    qDebug() << ui->TakeBook->currentIndex().sibling(ui->TakeBook->currentIndex().row(), 2).data();//ID_Livre

    dbConnect();
    QSqlQuery qry;
    QSqlQueryModel *model=new QSqlQueryModel();
    // 1 - recuperer le nombre d'exemplaire
    qry.prepare("SELECT nbr_exemplaire FROM Livres WHERE ID_livre="+ui->TakeBook->currentIndex().sibling(ui->TakeBook->currentIndex().row(), 2).data().toString());
    qry.exec();
    qry.next();
    int nbr=qry.value(0).toInt();
    //2 ajouter 1 puis le sauvegarder dans la base de donner
    qry.prepare("UPDATE Livres "
                "SET nbr_exemplaire=:nbr "
                "WHERE ID_livre="+ui->TakeBook->currentIndex().sibling(ui->TakeBook->currentIndex().row(), 2).data().toString());
    qry.bindValue(":nbr", nbr+1);
    qry.exec();
    // 3 supprimer l'enregistrement
    qry.prepare("DELETE FROM Emprunte WHERE IdEmp="+ui->TakeBook->currentIndex().sibling(ui->TakeBook->currentIndex().row(), 0).data().toString());
    qry.exec();

    // 4 rafrechir l'affichage
    qry.prepare("SELECT * FROM Emprunte");
    qry.exec();
    model->setQuery(qry);
    ui->TakeBook->setModel(model);
    ui->TakeBook->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    dbclose();
}
