#include "managefunctionwidget.h"

ManageFunctionWidget::ManageFunctionWidget(FunctionStruct *f, QWidget *parent):QWidget(parent)
{

    this->functionStruct = f;
    parameterWidget = new ParameterWidget(f,this);
    parameterWidget->indexenable(true);
   typeCombox = new QComboBox(this);
   saveButton = new QPushButton("Save",this);
   vboxLayout = new QVBoxLayout();
   this->setLayout(vboxLayout);
    vboxLayout->addWidget(typeCombox);
    vboxLayout->addWidget(saveButton);
   vboxLayout->addWidget(parameterWidget);

//   typeCombox->addItems(QStringList()<<"开关型"<<"数字型"<<"枚举型"<<"报警型"<<"日期型"<<"时间型");
   typeCombox->addItems(QStringList()<<"Switch"<<"Number"<<"Enumberate"<<"Alarm"<<"Date"<<"Time");
   connect(this->typeCombox,SIGNAL(activated(int)),this,SLOT(slotTypeChanged(int)));
   connect(this->saveButton,SIGNAL(clicked(bool)),this,SLOT(slotSaveClicked(bool)));
   parameterWidget->connectSignals();
}

void ManageFunctionWidget::writeToDataBase()
{
    QSqlQuery query1;
    QSqlQuery query2;
    QString str1;
    QString str2;
    str1 = "insert into general_command (id,gcmd_name_ch,gcmd_type,device_class)"
          "values (:id,:name,:type,:class);";
    str2 = "insert into generalcommand.parameter_default (general_command_id,direction,parameter,distype)"
           "values (:gcmd,:direction,:para,:distype);";
    qDebug()<<query1.prepare(str1);
    qDebug()<<query2.prepare(str2);
    query1.bindValue(":id",functionStruct->getVarIndex());
    query1.bindValue(":name",functionStruct->getFunctionName());
    query1.bindValue(":type",functionStruct->getVarType());
    query1.bindValue(":class",0);

    query2.bindValue(":gcmd",functionStruct->getVarIndex());
    query2.bindValue(":direction",functionStruct->getDirection());
    query2.bindValue(":para",functionStruct->getParameters());
     
    query2.bindValue(":distype",functionStruct->getDisType());
    if(!query1.exec())
    {
        QMessageBox::information(NULL,"Error","Write To DataBase gcmd_command Failed!");
    }
     if(!query2.exec())
     {

         QMessageBox::information(NULL,"Error","Write To DataBase parameter_default Failed!");
    }

}

void ManageFunctionWidget::slotTypeChanged(int type)
{
    qDebug()<<type+1;
    this->parameterWidget->typeChanged(type + 1);

}

void ManageFunctionWidget::slotSaveClicked(bool)
{
    this->parameterWidget->GetAllInformation();
    this->parameterWidget->GeneraterParameterStrings();
    qDebug()<<"\n\n*****************************************";
    qDebug()<<"FunctionName:"<<functionStruct->getFunctionName();
    qDebug()<<"VarIndex    :"<<functionStruct->getVarIndex();
    qDebug()<<"VarType     :"<<functionStruct->getVarType();
    qDebug()<<"Direction   :"<<functionStruct->getDirection();
    qDebug()<<"Parameters  :"<<functionStruct->getParameters();
    qDebug()<<"*****************************************";
    writeToDataBase();
}

