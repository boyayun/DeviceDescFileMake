#include "parameterwidget.h"
#include "ui_parameterwidget.h"

ParameterWidget::ParameterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParameterWidget)
{
    ui->setupUi(this);
    this->ui->stackedWidget->setCurrentIndex(4);
}

ParameterWidget::ParameterWidget(FunctionStruct *f, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ParameterWidget)
{
    ui->setupUi(this);
    this->ui->NumberScallingcomboBox->setVisible(false);
    this->setWindowTitle(f->getFunctionName());
    this->ui->CommandlineEdit->setEnabled(false);
    this->ui->NamelineEdit->setEnabled(false);
    this->functionStruct = f;
    this->intvalidator = new QIntValidator(0,10000,this);
    this->enumSpacerItem = new QSpacerItem(20,40);
    init();
   // connect(this->ui->EnumspinBox,SIGNAL(valueChanged(int)),this,SLOT(slotEnumNumChanged(int)));
    connect(this->ui->alarmSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotAlarmNumChanged(int)));
    connect(this->ui->AlarmlistWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(slotAlarmItemClicked(QListWidgetItem*)));
}

ParameterWidget::~ParameterWidget()
{
    delete ui;
}

void ParameterWidget::GetAllInformation()
{
   this->functionStruct->setFunctionName(ui->NamelineEdit->text());

   this->functionStruct->setVarIndex(ui->CommandlineEdit->text().toInt());
   if(this->ui->WriteradioButton->isChecked())
   {
       this->functionStruct->setDirection(1);
   }
   else if(this->ui->ReadradioButton->isChecked())
   {
       this->functionStruct->setDirection(2);

   }
   else if(this->ui->ReadWriteradioButton->isChecked())
   {
       this->functionStruct->setDirection(3);
   }

}

void ParameterWidget::GeneraterParameterStrings()
{
   on_SaveButton_clicked();

}

void ParameterWidget::connectSignals()
{
    connect(this->ui->EnumspinBox,SIGNAL(valueChanged(int)),this,SLOT(slotEnumNumChanged(int)));

}

void ParameterWidget::init()
{
    PRINTLOG("");
    this->ui->NamelineEdit->setText(functionStruct->getFunctionName());
    this->ui->CommandlineEdit->setText(QString::number(functionStruct->getVarIndex()));
    switch (functionStruct->getDirection()) {
    case 1:
        this->ui->WriteradioButton->setChecked(true);
        break;
    case 2:

        this->ui->ReadradioButton->setChecked(true);
        break;
    case 3:
        this->ui->ReadWriteradioButton->setChecked(true);
        break;
    default:
        break;
    }
    int vartype = functionStruct->getVarType();
    if(vartype == 1)
    {
        cmdParameters cp = functionStruct->getCmdParameters();
        this->ui->stackedWidget->setCurrentIndex(0);
        this->ui->cmdParameter1LineEdit->setText(QString::number(cp.parameter1));
        this->ui->cmdParameter2LineEdit->setText(QString::number(cp.parameter2));
        this->ui->cmdDis1LineEdit->setText(cp.dis1);
        this->ui->cmdDis2LineEdit->setText(cp.dis2);
        if(cp.defaultValue == cp.parameter1)
        {
            this->ui->cmdDefault1Radibtn->setChecked(true);
        }
        else
        {
            this->ui->cmdDefault2Radibtn->setChecked(true);
        }

    }
    else if(vartype == 2)
    {
        numberParamters np = functionStruct->getNumberParameters();
        PRINTLOG(np.step);
        this->ui->stackedWidget->setCurrentIndex(1);
        this->ui->NumberMaxlineEdit->setText(QString::number(np.max));
        this->ui->NumberMinlineEdit->setText(QString::number(np.min));
        this->ui->NumberDefaultlineEdit->setText(QString::number(np.defaultValue));
        this->ui->NumberScallinglineEdit->setText(QString::number(np.scalling));
        this->ui->NumberSteplineEdit->setText(QString::number(np.step));
        this->ui->NumberUnitcomboBox->setCurrentIndex(np.uint);

    }
    else if(vartype == 3)
    {
        enumParameters ep = functionStruct->getenumParameters();

        this->ui->stackedWidget->setCurrentIndex(2);
        this->addEnumItem(ep);
    }
    else if(vartype == 4)
    {
        this->ui->stackedWidget->setCurrentIndex(3);
        alarmParameters ap = functionStruct->getAlarmParameters();
        this->addAlarmItem(ap);
    }
    else if(vartype == 5)
    {
        this->ui->stackedWidget->setCurrentIndex(4);
        dateParameters dp = functionStruct->getDateParameters();
        this->ui->dateMaxEdit->setDate(dp.maxDate);
        this->ui->dateMinEdit->setDate(dp.minDate);
        this->ui->dateEdit->setDate(dp.date);
        this->ui->dateYDislineEdit->setText(dp.yearDis);
        this->ui->dateMDislineEdit->setText(dp.monthDis);
        this->ui->dateDDislineEdit->setText(dp.dayDis);
        this->ui->dateYSteplineEdit->setText(QString::number(dp.yearStep));
        this->ui->dateMSteplineEdit->setText(QString::number(dp.monthStep));
        this->ui->dateDSteplineEdit->setText(QString::number(dp.dayStep));
    }
    else if(vartype == 6)
    {
        this->ui->stackedWidget->setCurrentIndex(5);
        timeParameters tp = functionStruct->getTimeParameters();
        this->ui->timeMaxEdit->setTime(tp.maxTime);
        this->ui->timeMinEdit->setTime(tp.minTime);
        this->ui->timeEdit->setTime(tp.time);
        this->ui->timeHDisLineedit->setText(tp.hourDis);
        this->ui->timeMDisLineedit->setText(tp.minuteDis);
        this->ui->timeSDisLineedit->setText(tp.secondDis);
        this->ui->timeHStepLineedit->setText(QString::number(tp.hourStep));
        this->ui->timeMStepLineedit->setText(QString::number(tp.minuteStep));
        this->ui->timeSStepLineedit->setText(QString::number(tp.secondStep));
    }
    else if(vartype == 7)
    {
        alarmParameters ap = functionStruct->getAlarmParameters();
        QTableWidget *table = this->ui->AlarmtableWidget;
        for(int i=0;i<ap.alarmClass.size();i++)
        {
            table->insertRow(i);
            table->setItem(i,0,new QTableWidgetItem(QString::number(ap.alarmCode.at(i))));
            table->item(i,0)->setFlags(table->item(i,0)->flags()&(~Qt::ItemIsEditable));
            table->setItem(i,1,new QTableWidgetItem(ap.alarmName.at(i)));
            table->setItem(i,2,new QTableWidgetItem(QString::number(ap.alarmClass.at(i))));
        }
        this->updateAlarmListWidget();
        this->ui->stackedWidget->setCurrentIndex(6);

    }

}

void ParameterWidget::updateAlarmListWidget()
{
    PRINTLOG(functionStruct->getClass1());
    QSqlQuery query;
    QString str;
    int deviceClass = functionStruct->getClass1();
//    str = "select * from general_command where ((device_class = :deviceclass or device_class = 0 ) and gcmd_type = 4)";
//    str = "select * from general_command,parameter_default where ((device_class = "+QString::number(deviceClass)+" or device_class = 0 ) and gcmd_type = 4);";
    str = "select * from general_command,parameter_default where ((device_class = "+QString::number(deviceClass)+" or device_class = 0 ) and gcmd_type = 4 "
                                                                                                                " and general_command.id = parameter_default.general_command_id);";
//    qDebug()<<query.prepare(str);
//    query.bindValue(":deviceclass",deviceClass);
    qDebug()<<query.exec(str);
    while(query.next())
    {
        PRINTLOG(query.value("gcmd_name_ch").toString());
//        this->ui->AlarmlistWidget->addItem(new QListWidgetItem(query.value("gcmd_name_ch").toString()));

        FunctionStruct f;
        f.setParameters(query.value("parameter").toString());
        alarmParameters ap = f.getAlarmParameters();


        ui->AlarmlistWidget->addItem(new AlarmListWidgetItem(query.value("gcmd_name_ch").toString(),
                                                             query.value("id").toInt(),
                                                             ap.alarmClass.at(0)));
    }
}

void ParameterWidget::addEnumItem(enumParameters e)
{
    this->ui->EnumspinBox->setValue(e.itemNum);
    QMap<int,QString>::iterator it= e.items.begin();
    QGridLayout *gridLayout = (QGridLayout *)ui->EnumscrollAreaWidgetContents->layout();
    for(int i=0;i<e.itemNum;i++)

    {
        QLabel *l = new QLabel(QString("参数")+QString::number(i+1));
        QLineEdit *vle = new QLineEdit();
        vle->setValidator(this->intvalidator);
        vle->setText(QString::number(it.key()));
        QLineEdit *dle = new QLineEdit();
        dle->setText(it.value());
        QRadioButton *rb = new QRadioButton();
        buttonGroup.addButton(rb);
        if(it.key() == e.defaultValue)
        {
            rb->setChecked(true);
        }
        this->textList.append(l);
        this->valueEditList.append(vle);
        this->disEditList.append(dle);
        this->defaultRadionList.append(rb);
        gridLayout->addWidget(l,i+1,0,1,1);
        gridLayout->addWidget(vle,i+1,1,1,1);
        gridLayout->addWidget(dle,i+1,2,1,1);
        gridLayout->addWidget(rb,i+1,3,1,1);
        it++;

    }
    connect(ui->EnumspinBox,SIGNAL(valueChanged(int)),this,SLOT(slotEnumNumChanged(int)));
}

void ParameterWidget::addAlarmItem(alarmParameters a)
{
    int itemNum = a.alarmClass.size();
    this->ui->alarmSpinBox->setValue(itemNum);
    QGridLayout *gridLayout =(QGridLayout *) this->ui->alarmscrollAreaWidgetContents->layout();
    for(int i = 0; i< itemNum;i++)
    {
        QLabel * text = new QLabel(QString("报警")+QString::number(i+1));
        QLineEdit * alarmCode = new QLineEdit();
        QComboBox * alarmClass = new QComboBox();
        QLineEdit *alarmName = new QLineEdit();

        alarmCode->setText(QString::number(a.alarmCode.at(i)));
//        alarmClass->addItems(QStringList()<<"轻微"<<"一般"<<"严重");
        alarmClass->addItems(QStringList()<<"a"<<"b"<<"c");
        alarmClass->setCurrentIndex(a.alarmClass.at(i)-1);
        alarmName->setText(a.alarmName.at(i));

        alarmCodeEditList.append(alarmCode);
        alarmClassCombox.append(alarmClass);
        alarmNameEditList.append(alarmName);
        alarmTextList.append(text);


        gridLayout->addWidget(text,i+1,0,1,1);
        gridLayout->addWidget(alarmCode,i+1,1,1,1);
        gridLayout->addWidget(alarmName,i+1,2,1,1);
        gridLayout->addWidget(alarmClass,i+1,3,1,1);
    }


}

cmdParameters ParameterWidget::getCmdParameters()
{

    cmdParameters c;
    c.parameter1  = this->ui->cmdParameter1LineEdit->text().toInt();
    c.parameter2  = this->ui->cmdParameter2LineEdit->text().toInt();
    c.dis1 = this->ui->cmdDis1LineEdit->text();
    c.dis2 = this->ui->cmdDis2LineEdit->text();
    if(this->ui->cmdDefault1Radibtn->isChecked())
    {
       c.defaultValue = c.parameter1;
    }
    else
    {
       c.defaultValue = c.parameter2;
    }
    return c;
}

numberParamters ParameterWidget::getNumParameters()
{
    numberParamters n;
    n.max = this->ui->NumberMaxlineEdit->text().toInt();
    n.min = this ->ui->NumberMinlineEdit->text().toInt();
    n.uint = this->ui->NumberUnitcomboBox->currentIndex();
    n.defaultValue = this->ui->NumberDefaultlineEdit->text().toInt();
    n.scalling = this->ui->NumberScallinglineEdit->text().toInt();
    n.step = this->ui->NumberSteplineEdit->text().toInt();
    PRINTLOG(n.scalling);
    return n;

}

enumParameters ParameterWidget::getEnumParameters()
{
    enumParameters e;
    e.items.clear();
    e.itemNum = this->ui->EnumspinBox->value();
    for(int i=0;i<e.itemNum;i++)
    {
        e.items.insert(valueEditList.at(i)->text().toInt(),disEditList.at(i)->text());
        if(defaultRadionList.at(i)->isChecked())
        {
            e.defaultValue = valueEditList.at(i)->text().toInt();
        }
    }
    return e;
}

alarmParameters ParameterWidget::getAlarmParameters()
{
   alarmParameters a;
   for(int i=0;i<alarmCodeEditList.size();i++)
   {
       a.alarmCode.append(alarmCodeEditList.at(i)->text().toInt());
       a.alarmClass.append(alarmClassCombox.at(i)->currentIndex()+1);
       a.alarmName.append(alarmNameEditList.at(i)->text());
   }
   return a;

}

dateParameters ParameterWidget::getDataParameters()
{
    dateParameters d;
    d.maxDate = ui->dateMaxEdit->date();
    d.minDate = ui->dateMinEdit->date();
    d.date = ui->dateEdit->date();
    d.dayDis = ui->dateDDislineEdit->text();
    d.monthDis = ui->dateMDislineEdit->text();
    d.yearDis = ui->dateYDislineEdit->text();
    d.dayStep = ui->dateYSteplineEdit->text().toInt();
    d.monthStep = ui->dateMSteplineEdit->text().toInt();
    d.yearStep = ui->dateYSteplineEdit->text().toInt();
    return d;

}

timeParameters ParameterWidget::getTimeParameters()
{
    timeParameters t;
    t.maxTime = ui->timeMaxEdit->time();
    t.minTime = ui->timeMinEdit->time();
    t.time = ui->timeEdit->time();
    t.hourDis = ui->timeHDisLineedit->text();
    t.secondDis = ui->timeSDisLineedit->text();
    t.minuteDis = ui->timeMDisLineedit->text();
    t.hourStep = ui->timeHStepLineedit->text().toInt();
    t.minuteStep = ui->timeMStepLineedit->text().toInt();
    t.secondStep = ui->timeSStepLineedit->text().toInt();
    return t;

}

alarmParameters ParameterWidget::getAlarmParameters2()
{
    QTableWidget *tableWidget = this->ui->AlarmtableWidget;
    alarmParameters ap;

    for(int i=0;i<tableWidget->rowCount();i++)
    {
        qDebug()<<tableWidget->item(i,0)->text();
        qDebug()<<tableWidget->item(i,1)->text();
        qDebug()<<tableWidget->item(i,2)->text();
        ap.alarmCode.append(tableWidget->item(i,0)->text().toInt());
        ap.alarmName.append(tableWidget->item(i,1)->text());
        ap.alarmClass.append(tableWidget->item(i,2)->text().toInt());
    }

    return ap;

}


void ParameterWidget::on_SaveButton_clicked()
{
    int varType = this->functionStruct->getVarType();
    PRINTLOG(varType);
    if(this->ui->ReadradioButton->isChecked())
    {
       functionStruct->setDirection(2);
    }
    else if(this->ui->WriteradioButton->isChecked())
    {
       functionStruct->setDirection(1);
    }
    else if(this->ui->ReadWriteradioButton->isCheckable())
    {
       functionStruct->setDirection(3);
    }
    if(varType == 1)
    {
        cmdParameters c = getCmdParameters();
        this->functionStruct->setParameter(c);
    }
    else if(varType ==2)
    {
        PRINTLOG("NUMBER");
        numberParamters n = getNumParameters();
        this->functionStruct->setParameter(n);

    }
    else if(varType == 3)
    {
        QList<int> valueList;
        bool hasRepeatValue = false;
        for(int i=0; i< valueEditList.size();i++)
        {
            valueList.append(valueEditList.at(i)->text().toInt());
        }
        qSort(valueList.begin(),valueList.end());

        if(valueList.size()==1)
        {
            hasRepeatValue = false;
        }
        else
        {
            for(int i=0;i<valueList.size()-1;i++)
            {
               if(valueList.at(i) == valueList.at(i+1))
               {
                  hasRepeatValue = true;
                  break;
               }

            }
        }
        if(hasRepeatValue)
        {
            QMessageBox::warning(this,"参数重复","有重复参数");
        }
        else
        {
        enumParameters e = getEnumParameters();

        this->functionStruct->setParameter(e);
        }

    }
    else if(varType == 4)
    {
        alarmParameters a = getAlarmParameters();
        this->functionStruct->setParameter(a);

    }
    else if(varType == 5)
    {
        dateParameters d = getDataParameters();
        this->functionStruct->setParameter(d);

    }
    else if(varType == 6)
    {
        timeParameters t = getTimeParameters();
        this->functionStruct->setParameter(t);

    }
    else
    {
        // 报警添加类型
        PRINTLOG(functionStruct->getClass1());
        alarmParameters ap = getAlarmParameters2();

        this->functionStruct->setParameter(ap);

    }

}

void ParameterWidget::slotEnumNumChanged(int n)
{
    PRINTLOG(n);
    int currentNum = textList.size();
    while(currentNum > n)
    {
        delete textList.last();
        delete valueEditList.last();
        delete disEditList.last();
        delete defaultRadionList.last();
        textList.removeLast();
        valueEditList.removeLast();
        disEditList.removeLast();
        defaultRadionList.removeLast();
        currentNum --;
    }
    QGridLayout *gridLayout = (QGridLayout *)ui->EnumscrollAreaWidgetContents->layout();
    while (currentNum < n )
    {

        QLabel *l = new QLabel(QString("参数")+QString::number(currentNum+1));
        QLineEdit *vle = new QLineEdit();
        vle->setValidator(this->intvalidator);
        QLineEdit *dle = new QLineEdit();
        QRadioButton *rb = new QRadioButton();
        buttonGroup.addButton(rb);
        this->textList.append(l);
        this->valueEditList.append(vle);
//        connect(vle,SIGNAL(editingFinished()),this,SLOT(slotCheckHasValueRepeat()));
        this->disEditList.append(dle);
        this->defaultRadionList.append(rb);
        gridLayout->addWidget(l,currentNum+1,0,1,1);
        gridLayout->addWidget(vle,currentNum+1,1,1,1);
        gridLayout->addWidget(dle,currentNum+1,2,1,1);
        gridLayout->addWidget(rb,currentNum+1,3,1,1);
        currentNum++;
    }
    gridLayout->addItem(enumSpacerItem,currentNum+2,1,1,1);


}

void ParameterWidget::slotAlarmNumChanged(int n)
{
    PRINTLOG(alarmCodeEditList.size());
    PRINTLOG(n);

    int currentNum = alarmCodeEditList.size();
    while(currentNum > n)
    {
        delete alarmClassCombox.last();
        delete alarmCodeEditList.last();
        delete alarmTextList.last();
        delete alarmNameEditList.last();
        alarmClassCombox.removeLast();
        alarmCodeEditList.removeLast();
        alarmTextList.removeLast();
        alarmNameEditList.removeLast();
        currentNum --;
    }
    QGridLayout *gridLayout = (QGridLayout *)ui->alarmscrollAreaWidgetContents->layout();
    while (currentNum < n )
    {
        QLabel *l = new QLabel(QString("报警")+QString::number(currentNum+1));
        QLineEdit *codeLineEdit = new QLineEdit();
        codeLineEdit->setValidator(this->intvalidator);
        QLineEdit *nameLineEdit = new QLineEdit();
        QComboBox * classCombox = new QComboBox();
        classCombox->addItems(QStringList()<<"轻微"<<"一般"<<"严重");
        this->alarmTextList.append(l);
        this->alarmClassCombox.append(classCombox);
        this->alarmNameEditList.append(nameLineEdit);
        this->alarmCodeEditList.append(codeLineEdit);
        gridLayout->addWidget(l,currentNum+1,0,1,1);
        gridLayout->addWidget(codeLineEdit,currentNum+1,1,1,1);
        gridLayout->addWidget(nameLineEdit,currentNum+1,2,1,1);
        gridLayout->addWidget(classCombox,currentNum+1,3,1,1);
        currentNum++;
    }
    gridLayout->addItem(enumSpacerItem,currentNum+2,1,1,1);

}

void ParameterWidget::slotAlarmItemClicked(QListWidgetItem *item)
{

    AlarmListWidgetItem * alarmItem = (AlarmListWidgetItem *)(item);


    QTableWidget *tableWidget = this->ui->AlarmtableWidget;
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(alarmItem->getVarIndex())));
    tableWidget->setItem(row,1,new QTableWidgetItem(alarmItem->getAlarmString()));
    tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(alarmItem->getAlamClass())));
}

void ParameterWidget::typeChanged(int type)
{
    this->functionStruct->setVarType(type);
    this->ui->stackedWidget->setCurrentIndex(type-1);
}

void ParameterWidget::indexenable(bool)
{
   this->ui->CommandlineEdit->setEnabled(true);
    this->ui->NamelineEdit->setEnabled(true);
}

void ParameterWidget::slotCheckHasValueRepeat()
{
   for(int i = 0; i< valueEditList.size();i++)
   {

   }
}










AlarmListWidgetItem::AlarmListWidgetItem(const QString text, int varIndex, int alarmclass):QListWidgetItem(text)
{
    this->alarmString = text;
    this->varIndex = varIndex;
    this->alamClass = alarmclass;

}
int AlarmListWidgetItem::getVarIndex() const
{
    return varIndex;
}

void AlarmListWidgetItem::setVarIndex(int value)
{
    varIndex = value;
}

QString AlarmListWidgetItem::getAlarmString() const
{
    return alarmString;
}

void AlarmListWidgetItem::setAlarmString(const QString &value)
{
    alarmString = value;
}
int AlarmListWidgetItem::getAlamClass() const
{
    return alamClass;
}

void AlarmListWidgetItem::setAlamClass(int value)
{
    alamClass = value;
}





void ParameterWidget::on_alarmUpMove_clicked()
{
    int currentRow = ui->AlarmtableWidget->currentRow();
    if(currentRow > 0)
    {
        QTableWidgetItem *alarmcode = ui->AlarmtableWidget->takeItem(currentRow,0);
        QTableWidgetItem *alarmname = ui->AlarmtableWidget->takeItem(currentRow,1);
        QTableWidgetItem *alarmclass = ui->AlarmtableWidget->takeItem(currentRow,2);
        QTableWidgetItem *prealarmcode = ui->AlarmtableWidget->takeItem(currentRow-1,0);
        QTableWidgetItem *prealarmname = ui->AlarmtableWidget->takeItem(currentRow-1,1);
        QTableWidgetItem *prealarmclass = ui->AlarmtableWidget->takeItem(currentRow-1,2);

        ui->AlarmtableWidget->setItem(currentRow,0,prealarmcode);
        ui->AlarmtableWidget->setItem(currentRow,1,prealarmname);
        ui->AlarmtableWidget->setItem(currentRow,2,prealarmclass);
        ui->AlarmtableWidget->setItem(currentRow-1,0,alarmcode);
        ui->AlarmtableWidget->setItem(currentRow-1,1,alarmname);
        ui->AlarmtableWidget->setItem(currentRow-1,2,alarmclass);

        this->ui->AlarmtableWidget->selectRow(currentRow-1);
    }
}

void ParameterWidget::on_alarmDowmMove_clicked()
{
    int rowCount = ui->AlarmtableWidget->rowCount();
    int currentRow = ui->AlarmtableWidget->currentRow();
    if(currentRow < rowCount -1 )
    {
        QTableWidgetItem *alarmcode = ui->AlarmtableWidget->takeItem(currentRow,0);
        QTableWidgetItem *alarmname = ui->AlarmtableWidget->takeItem(currentRow,1);
        QTableWidgetItem *alarmclass = ui->AlarmtableWidget->takeItem(currentRow,2);
        QTableWidgetItem *prealarmcode = ui->AlarmtableWidget->takeItem(currentRow+1,0);
        QTableWidgetItem *prealarmname = ui->AlarmtableWidget->takeItem(currentRow+1,1);
        QTableWidgetItem *prealarmclass = ui->AlarmtableWidget->takeItem(currentRow+1,2);

        ui->AlarmtableWidget->setItem(currentRow,0,prealarmcode);
        ui->AlarmtableWidget->setItem(currentRow,1,prealarmname);
        ui->AlarmtableWidget->setItem(currentRow,2,prealarmclass);
        ui->AlarmtableWidget->setItem(currentRow+1,0,alarmcode);
        ui->AlarmtableWidget->setItem(currentRow+1,1,alarmname);
        ui->AlarmtableWidget->setItem(currentRow+1,2,alarmclass);

        this->ui->AlarmtableWidget->selectRow(currentRow+1);
    }

}

void ParameterWidget::on_alarmDelete_clicked()
{
    int currentRow = ui->AlarmtableWidget->currentRow();
    if(currentRow !=-1)
    {
        ui->AlarmtableWidget->removeRow(currentRow);
    }

}
