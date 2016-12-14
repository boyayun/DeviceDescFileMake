#include "deviceinformationwidget.h"
#include "ui_deviceinformationwidget.h"

deviceInformationWidget::deviceInformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deviceInformationWidget)
{
    ui->setupUi(this);
    this->ui->frame->setStyleSheet(".QFrame{border:1px solid black}");
    this->ui->modelIDlineEdit->setEnabled(false);
    this->ui->class1LineEdit->setEnabled(false);
    this->ui->class2LineEdit->setEnabled(false);
}

deviceInformationWidget::~deviceInformationWidget()
{
    delete ui;
}

void deviceInformationWidget::updateFromDeviceID(int deviceid)
{
    this->deviceID = deviceid;
    QSqlQuery query;
    QString str = "select * from device where id = (:id)";
    query.prepare(str);
    query.bindValue(":id",deviceid);
    query.exec();
    while(query.next())
    {
        ui->namelineEdit->setText(query.value("device_name").toString());
        deviceName = query.value("device_name").toString();
        QSqlQuery query2;
        QString str2 = "select class_name from device_class1 where id  = " +query.value("device_class2_class1_id").toString();
        query2.exec(str2);
        query2.next();
        ui->class1LineEdit->setText(query2.value("class_name").toString());
        int class2=query.value("device_class2_class2_id").toInt();
        this->class_2 = class2;
        int class1 = query.value("device_class2_class1_id").toInt();
        this->class_1 = class1;
        str2 = "select class2_name from device_class2 where class1_id  = " +
                QString::number(class1)+ " and class2_id = "+ QString::number(class2);

        QSqlQuery query3;
        query3.exec(str2);
        query3.next();
        ui->class2LineEdit->setText(query3.value("class2_name").toString());
        ui->modelLineedit->setText(query.value("device_model").toString());
        ui->modelIDlineEdit->setText(query.value("device_modelID").toString());
        this->deviceModel = query.value("device_model").toString();
        ui->manufactureIDLineedit->setText(query.value("manufactureID").toString());
        this->deviceModelID = query.value("device_modelID").toString();
        ui->urllineEdit->setText(query.value("url").toString());
        this->url = query.value("url").toString();
        ui->updateCheckbox->setChecked(query.value("up").toBool());
        this->isUpdate = query.value("up").toBool();
        ui->synCheckBox->setChecked(query.value("syn").toBool());
        this->isSync = query.value("syn").toBool();
    }

}

void deviceInformationWidget::on_savepushButton_clicked()
{


}
