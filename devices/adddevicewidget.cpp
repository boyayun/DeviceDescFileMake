#include "adddevicewidget.h"
#include "ui_adddevicewidget.h"

AddDeviceWidget::AddDeviceWidget(UserInformation *userInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDeviceWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Create a New Device!");
    this->userInformation = userInfo;
    this->ui->deviceNameLineEdit->setPlaceholderText("Please input device name!");
    this->ui->DeviceModelLineEdit->setPlaceholderText("Please input model");
    this->ui->ManufactureIDLineEdit->setPlaceholderText("Please input manufactureid");
    init();
    autoInput();
    this->slotClass1Changed(0);
}

AddDeviceWidget::~AddDeviceWidget()
{
    delete ui;
}

void AddDeviceWidget::init()
{
    QSqlQuery query;
    QString str = "select * from device_class1 where id > 0 order by id";
    query.exec(str);
    while(query.next())
    {
        this->ui->class1comboBox->addItem(query.value("class_name").toString());
    }
    connect(ui->class1comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotClass1Changed(int)));

}

void AddDeviceWidget::autoInput()
{
    this->ui->ManufactureIDLineEdit->setText(userInformation->getManufactureId());
}

int AddDeviceWidget::getModelID()
{
    QSqlQuery query;
    QString str("select count(device_modelID) as numid ,max(device_modelID) as maxid from device where user_id = :userid");
    query.prepare(str);
    query.bindValue(":userid",userInformation->getUserId());
    query.exec();
    query.next();
    int count = query.value("numid").toInt();
    int maxid = query.value("maxid").toInt();
    int ret = 1;

    if(count > 0)
    {
        ret = maxid+1;
    }
    return ret;
}

bool AddDeviceWidget::isPassInputCheck()
{
    if(ui->deviceNameLineEdit->text().isEmpty())
    {
        return false;
    }
    else if(ui->DeviceModelLineEdit->text().isEmpty())
    {
        return false;
    }
    else if(ui->ManufactureIDLineEdit->text().isEmpty())
    {
        return false;
    }
    else
    {

    }
    return true;

}

void AddDeviceWidget::slotClass1Changed(int class1)
{
    ui->class2comboBox->clear();
    class1 +=1;
    QSqlQuery query;
    QString str = "select * from device_class2 where class1_id = (:class1_id) order by class2_id";
    query.prepare(str);
    query.bindValue(":class1_id",class1);
    query.exec();
    while(query.next())
    {
        this->ui->class2comboBox->addItem(query.value("class2_name").toString());
    }
}

void AddDeviceWidget::on_createpushButton_clicked()
{
    if(isPassInputCheck())
    {
    QSqlQuery query;
    QString str = "insert into device (device_name,user_id,device_class2_class1_id,device_class2_class2_id"
                  ",device_model,device_modelID,manufactureID,up,syn,url)"
                  " values(:devicename,:userid,:class1,:class2,:model,:modelID,:manuID,:up,:syn,:url)";
    query.prepare(str);
    query.bindValue(":devicename",ui->deviceNameLineEdit->text());
    query.bindValue(":userid",userInformation->getUserId());
    query.bindValue(":class1",ui->class1comboBox->currentIndex()+1);
    query.bindValue(":class2",ui->class2comboBox->currentIndex()+1);
    query.bindValue(":model",ui->DeviceModelLineEdit->text());
    query.bindValue(":modelID",getModelID());
    query.bindValue(":manuID",ui->ManufactureIDLineEdit->text());
    query.bindValue(":up",ui->updatecheckBox->isChecked()?1:0);
    query.bindValue(":syn",ui->synccheckBox->isChecked()?1:0);
    query.bindValue(":url",ui->urllineEdit->text());
    if(!query.exec())
    {
        QMessageBox::warning(NULL,"CreatFileError","Write to DataBaseError");
    }
    emit signalUpdateTreeList();
    this->close();
    }
}
