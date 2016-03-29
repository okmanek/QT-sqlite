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
    query.exec("SELECT id, name, email, BirthDate FROM Persons");

    if(query.isActive())
    {
        ui->statusBar->showMessage("Database connection ok");
    }
    else
    {
        ui->statusBar->showMessage(QString("Database connection error") + query.lastError().text());
        //also availble: db.lastError()
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
