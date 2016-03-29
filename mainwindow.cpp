#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect to the DB:
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/i/code/sql/data.sqlite3");
    db.open();

    //set a "cursor"
    query = QSqlQuery(db);
    query.exec("SELECT id, name, email, birthdate FROM Persons");

    if(query.isActive())
    {
        ui->statusBar->showMessage("Database connection ok");
        query.first();
        FillForm();
    }
    else
    {
        ui->statusBar->showMessage(QString("Database connection error") + query.lastError().text());
        //also availble: db.lastError()
    }
    ui->Save->hide();
    newrecord = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::FillForm()
{
    ui->Name->setText(query.value(1).toString());
    ui->Email->setText(query.value(2).toString());
    ui->BirthDate->setDate(query.value(3).toDate());
    //setText(query.value(2).toDate());
}

void MainWindow::on_New_clicked()
{
    newrecord = true;
    ui->Name->clear();
    ui->Email->clear();
}

void MainWindow::on_Save_clicked()
{
    if(!newrecord)
    {
        QSqlQuery updateQuery = QSqlQuery(db);
        updateQuery.prepare("UPDATE Persons SET id = :id, name = :name, email = :email, birthdate = :birthdate WHERE id = :id");
        updateQuery.bindValue(":id", query.value(0));
        updateQuery.bindValue(":name", ui->Name->text());
        updateQuery.bindValue(":email", ui->Email->text());
        updateQuery.bindValue(":birthdate", ui->BirthDate->text());

        if(!updateQuery.exec())
            ui->statusBar->showMessage(QString("Query (update) error: ") + updateQuery.lastError().text());
        else
            ui->statusBar->showMessage(QString("Update ok"));

        query.exec("SELECT id, name, email, birthdate FROM Persons");
        query.first();
        FillForm();
    }
    else//insert
    {
        newrecord = false;

        QSqlQuery updateQuery = QSqlQuery(db);
        updateQuery.prepare("INSERT INTO Persons (name, email, birthdate) VALUES (:name, :email, :birthdate)");
        updateQuery.bindValue(":id", query.value(0));
        updateQuery.bindValue(":name", ui->Name->text());
        updateQuery.bindValue(":email", ui->Email->text());
        updateQuery.bindValue(":birthdate", ui->BirthDate->text());

        if(!updateQuery.exec())
            ui->statusBar->showMessage(QString("Query (insert) error: ") + updateQuery.lastError().text());
        else
            ui->statusBar->showMessage(QString("insert ok"));

        query.exec("SELECT id, name, email, birthdate FROM Persons");
        query.last();
        FillForm();
    }

}

void MainWindow::on_Previous_clicked()
{
    if(!query.previous())
            query.last();
    FillForm();
}

void MainWindow::on_Next_clicked()
{
    if(!query.next())
            query.first();
    FillForm();
}

void MainWindow::on_Cancel_clicked()
{
    FillForm();
    newrecord = false;
}
