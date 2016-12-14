#include "userinformationwidget.h"
#include "ui_userinformationwidget.h"

UserInformationWidget::UserInformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInformationWidget)
{
    ui->setupUi(this);
}

UserInformationWidget::UserInformationWidget(UserInformation *userInfo, QWidget *parent):
    QWidget(parent),
    ui(new Ui::UserInformationWidget)
{
    ui->setupUi(this);

    ui->userLineEdit->setText(userInfo->getUserName());
    ui->nameLineEdit->setText(userInfo->getUserName());
    ui->factureIdLineEdit->setText(userInfo->getManufactureId());
    ui->phoneLineEdit->setText("1111111111111");

}

UserInformationWidget::~UserInformationWidget()
{
    delete ui;
}
