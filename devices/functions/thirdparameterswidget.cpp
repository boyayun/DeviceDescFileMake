#include "thirdparameterswidget.h"
#include "ui_thirdparameterswidget.h"
#include <QDebug>

ThirdParametersWidget::ThirdParametersWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThirdParametersWidget)
{
    ui->setupUi(this);
    this->ui->spinBox->setValue(5);
    this->isNeedSave = false;
}

ThirdParametersWidget::ThirdParametersWidget(QString parameterstr, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ThirdParametersWidget)
{
    ui->setupUi(this);
    QJsonDocument doc = QJsonDocument::fromJson(parameterstr.toUtf8());
    QJsonObject jsonObj = doc.object();
        QJsonArray array = jsonObj.value("thirdParameters").toArray();
        this->ui->spinBox->setValue(array.size());
        for(int i=0;i<array.size();i++)
        {
           this->parameterEditList.at(i)->setText(array.at(i).toString());
        }
}

ThirdParametersWidget::~ThirdParametersWidget()
{
    delete ui;
}

void ThirdParametersWidget::on_spinBox_valueChanged(int arg1)
{
    int n = this->parameterEditList.size();
    qDebug()<<n<<arg1;
    while(n > arg1)
    {
        delete parameterEditList.last();
        this->parameterEditList.removeLast();
        n--;
    }
    while( n < arg1 )
    {
        QLineEdit *lineEdit = new QLineEdit();
        lineEdit->setPlaceholderText("请输入参数");
        this->parameterEditList.append(lineEdit);
        this->ui->scrollAreaWidgetContents->layout()->addWidget(lineEdit);
        n++;
    }
    this->isNeedSave = false;


}

void ThirdParametersWidget::on_pushButton_clicked()
{
    QJsonObject jsonObj;
    QJsonArray jsonArray;
    for(int i=0;i<this->ui->spinBox->value();i++)
    {
        jsonArray.append(this->parameterEditList.at(i)->text());
    }
    jsonObj.insert("thirdParameters",jsonArray);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);
    this->parameterStr = QString(jsonDoc.toJson());
    this->isNeedSave = true;

   this->close();
}
bool ThirdParametersWidget::getIsNeedSave() const
{
    return isNeedSave;
}

QString ThirdParametersWidget::getParameterStr() const
{
    return parameterStr;
}

void ThirdParametersWidget::setParameterStr(const QString &value)
{
    parameterStr = value;
}



