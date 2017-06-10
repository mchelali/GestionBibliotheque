#include "empreinter.h"
#include "ui_empreinter.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>
#include <QMessageBox>

empreinter::empreinter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::empreinter)
{
    ui->setupUi(this);

    date_emp = QDate::currentDate();
}

empreinter::~empreinter()
{
    delete ui;
}

void empreinter::setdata(int id_livre, QString titre, QString sousTitre, QString Domaine, QPixmap cover, QString student, int id_student)
{
    id_book = id_livre;
    title=titre;
    SubTitle=sousTitre;
    Domain=Domaine;
    cov=cover;

    id_user = id_student;
    user=student;

    //set file to print
    printer = new QPrinter(QPrinter::HighResolution);//QPrinter::HighResolution
    printer->setFullPage(true);
    printer->setPaperSize(QPrinter::A4);

    paint = new QPainter();


    qDebug() << "page rect "<< printer->pageRect().operator +=( QMargins(20,20,0,0));
    qDebug() << "resolution " << printer->resolution();

    printPreview = new QPrintPreviewWidget(printer);

    QHBoxLayout *layoutv = new QHBoxLayout(ui->widget);
    layoutv->addWidget(printPreview);

    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(quitance(QPrinter*)));

    printPreview->show();
    printPreview->fitInView();

}

void empreinter::quitance(QPrinter *printer){
    paint->begin(printer);

    //paint->drawLine(printer->pageRect().width() / 2, 0, printer->pageRect().width() / 2, printer->pageRect().height());
    //paint->drawLine(0, printer->pageRect().height() / 2, printer->pageRect().width(), printer->pageRect().height() / 2);

    QFont f;
    f.setPointSize(15);
    f.setBold(true);
    paint->setFont(f);

    paint->drawText( printer->pageRect().operator -=( QMargins(50,50,50,50)), Qt::AlignHCenter, "Quitance d'emprunte");

    f.setPointSize(13);
    f.setBold(false);
    paint->setFont(f);

    paint->drawPixmap(50, 150, 900, 1200, cov);

    paint->drawLine(1000, 150, printer->pageRect().width() ,150);

    paint->drawText(printer->pageRect().operator -=( QMargins(1000,200,0,0)), "Titre du livre : "+ title);
    paint->drawText(printer->pageRect().operator -=( QMargins(1000,350,0,0)), "Sous titre du livre : "+ SubTitle);

    paint->drawLine(1000, 500, printer->pageRect().width() ,500);

    paint->drawText(printer->pageRect().operator -=( QMargins(1000,570,0,0)), "Nom de l'étudiant : "+ user);

    paint->drawLine(1000, 700, printer->pageRect().width() ,700);

    paint->drawText(printer->pageRect().operator -=( QMargins(1000,770,0,0)), "Date d'emprunte : "+ date_emp.toString("dddd dd MM yyyy"));
    paint->drawText(printer->pageRect().operator -=( QMargins(1000,870,0,0)), "Date limite pour rendre le livre : "+ date_emp.addDays(7).toString("dddd dd MM yyyy"));

    paint->drawLine(1000, 1000, printer->pageRect().width() ,1000);

    paint->drawText(printer->pageRect().operator -=( QMargins(1000,1070,0,0)), "Note : Veillez présenter cette quittance au prés de l'administration de la bibliothèque pour emprunter ce livre."
                    "Et aussi  pour le rendre avant la date limite.");

    paint->drawLine(1000, 1350, printer->pageRect().width() ,1350);

    paint->end();
}

void empreinter::on_close_clicked()
{
    close();
}

void empreinter::on_confirmation_clicked()
{

    qDebug() << date_emp.toString("dddd dd MM yyyy");
    qDebug() << date_emp.addDays(7).toString("dddd dd MM yyyy");

    dbConnect();

    QSqlQuery qry;
    qry.prepare("SELECT nbr_exemplaire FROM Livres WHERE ID_Livre="+QString::number(id_book));
    qry.exec();
    qry.next();
    int x = qry.value(0).toInt();
    if (x>0){
        qry.prepare("UPDATE Livres "
                    "SET nbr_exemplaire=:nbr "
                    "WHERE ID_livre="+QString::number(id_book));
        qry.bindValue(":nbr", x-1);
        qry.exec();

        qry.prepare("INSERT INTO Emprunte ('ID_adherent', 'ID_livre', 'dateEmp', 'dateRend') VALUES (:idU, :idB, :emp , :rend )");
        qry.bindValue(":idU", id_user);
        qry.bindValue(":idB", id_book);
        qry.bindValue(":emp", date_emp);
        qry.bindValue(":rend", date_emp.addDays(7));

        if(!qry.exec()){
            qDebug() << qry.lastError() ;
        }else{
            qDebug() << "livre emprunté est sauvegardé";
            printPreview->print();
        }
    }else{
        QMessageBox msg;
        msg.setText("Ce livre n'est malheureusement pas disponible");
        msg.exec();
    }

    dbclose();
    close();
}
