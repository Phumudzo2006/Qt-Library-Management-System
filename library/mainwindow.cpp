#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton,SIGNAL(clicked()),
            this,SLOT(button1()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::button1()
{
    QString book[] = {"H_Potter","English","Math"};
    int rollNo[] = {101,203,304};

    QString ubook= ui->LineEdit->text{};
    QString uRollNo= ui->lineEdit_2->text();
    int uRollNo_int = uRollNo.toInt();
    bool found= false;
    for (int i=0; i<sizeof(uRollNo); i++){
        if(uRollNo_int==rollNo[i] && ubook==book[i]){
            QMessageBox::information (this, "Library Manegement","Book: "+book[1]+"\n"
                                          "To Student: "+uRollNo );
            found= true;
                break;
        }
        else if(!found){
            QMessageBox::information(this, "Error Box",
                                     "Book is unavailable or Rollno is incorrect");
        }
    }
}

