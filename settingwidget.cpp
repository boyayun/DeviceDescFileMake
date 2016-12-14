#include "settingwidget.h"
#include "ui_settingwidget.h"

SettingWidget::SettingWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWidget)
{

    ui->setupUi(this);
    this->setWindowTitle("Setting");
    QSettings s("setting.ini",QSettings::IniFormat); //配置文件读取配置信息
    this->ui->AddresslineEdit->setText(s.value("Address").toString());
    this->ui->portlineEdit->setText(s.value("Port").toString());
}

SettingWidget::~SettingWidget()
{
    delete ui;
}

void SettingWidget::on_pushButton_clicked()
{
    QSettings settings("setting.ini",QSettings::IniFormat);
    settings.setObjectName("setting.ini");
    settings.setValue("Address",this->ui->AddresslineEdit->text());
    settings.setValue("Port",this->ui->portlineEdit->text());
    settings.setValue("CurrentUser","");
    QMessageBox::warning(this,"Success!","Please Restore the application!");
    this->close();
}
