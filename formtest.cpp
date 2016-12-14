#include "formtest.h"
#include "ui_formtest.h"
#include <QDebug>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QComboBox>

FormTest::FormTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTest)
{
    ui->setupUi(this);
}

FormTest::~FormTest()
{
    delete ui;
}

void FormTest::on_pushButton_clicked()
{
    qDebug()<<"xx";
    this->ui->tableWidget->insertRow(0);
//    this->ui->tableWidget->setItem(0,0,new QTableWidgetItem("1"));
//    this->ui->tableWidget->setCellWidget(0,0,new QLabel("1"));

    QTableWidgetItem *t = new QTableWidgetItem("1");
  //  t->setFlags(!Qt::ItemIsEditable);
    this->ui->tableWidget->setItem(0,0,t);

    this->ui->tableWidget->setCellWidget(0,1,new QPushButton("Ab"));
    this->ui->tableWidget->setCellWidget(0,2,new QComboBox());

//    this->ui->tableWidget->setItem(2,0,new QTableWidgetItem("2"));
//    this->ui->tableWidget->setItem(3,0,new QTableWidgetItem("3"));
}
