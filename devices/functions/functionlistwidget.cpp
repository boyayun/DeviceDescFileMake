#include "functionlistwidget.h"

FunctionListWidget::FunctionListWidget(QWidget *parent) : QWidget(parent)
{
    this->mainLayout = new QVBoxLayout();
    this->operationLayout = new QHBoxLayout();
    this->tableWidget = new QTableWidget();
    //connect(tableWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(slotRowClicked(QModelIndex)));
    init();
    mainLayout->addWidget(tableWidget);
    this->mainLayout->addLayout(operationLayout);


    this->setLayout(mainLayout);
}

FunctionListWidget::FunctionListWidget(int deviceid, QWidget *parent):QWidget(parent)
{
    PRINTLOG(deviceid);
    this->deviceId = deviceid;
    this->addFunctionWidget = NULL;
    this->mainLayout = new QVBoxLayout();
    this->tableWidget = new QTableWidget();
    connect(tableWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(slotRowClicked(QModelIndex)));
    init();
    mainLayout->addWidget(tableWidget);

    this->setLayout(mainLayout);
    slotUpdateDeviceID(1);
}
void FunctionListWidget::init()
{
    QStringList headers;
    headers<<"Command"<<"Name"<<"Direction"<<"Operation";
    this->tableWidget->setColumnCount(4);
    this->tableWidget->setHorizontalHeaderLabels(headers);
    //    this->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    this->tableWidget->setAlternatingRowColors(true);
    //    this->tableWidget->verticalHeader()->setHidden(true);
    this->saveFunctionButton = new QPushButton("保存");
    connect(saveFunctionButton,SIGNAL(clicked(bool)),this,SLOT(slotSaveFunctions(bool)));
    this->addFunctionButton = new QPushButton("增加");
    connect(addFunctionButton,SIGNAL(clicked(bool)),this,SLOT(slotAddFunctions(bool)));
    this->upmoveButton = new QPushButton("上移");
    connect(upmoveButton,SIGNAL(clicked(bool)),this,SLOT(slotUpMovedClicked(bool)));
    this->downmoveButton = new QPushButton("下移");
    connect(downmoveButton,SIGNAL(clicked(bool)),this,SLOT(slotDownMovedClicked(bool)));
    this->createButton = new QPushButton("生成");
    connect(createButton,SIGNAL(clicked(bool)),this,SLOT(slotCreateClicked(bool)));
    this->deleteButton = new QPushButton("删除");
    connect(deleteButton,SIGNAL(clicked(bool)),this,SLOT(slotDeleteClicked(bool)));

    this->operationLayout->addWidget(this->upmoveButton);
    this->operationLayout->addWidget(this->downmoveButton);
    this->operationLayout->addWidget(this->saveFunctionButton);
    this->operationLayout->addWidget(this->addFunctionButton);
    this->operationLayout->addWidget(this->createButton);
    this->operationLayout->addWidget(this->deleteButton);
    connect(this->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slotCellClicked(int,int)));
//    connect(this->tableWidget,SIGNAL(entered(QModelIndex)),this,SLOT(slotRowClicked(QModelIndex )));
}


void FunctionListWidget::clear()
{
    int n = tableWidget->rowCount();
    for(int i=0;i<n;i++)
    {
        tableWidget->removeRow(0);
    }

}

void FunctionListWidget::getHeadInformation()
{

}

void FunctionListWidget::updateFunctionNumber()
{
    int startFunctionNumber = 1;
    for(int i=0;i<functionList.size();i++)
    {
        if(i == 0)
        {
            if(functionList.at(i)->getVarIndex() == 31)
            {
                startFunctionNumber = 0;
            }
        }
        functionList.at(i)->setFunctionNumber(startFunctionNumber);
        startFunctionNumber++;
        PRINTLOG(startFunctionNumber);
    }

}

void FunctionListWidget::insertAFunction(FunctionStruct *f)
{
    functionList.append(f);
    f->setClass1(deviceClass1);
    f->setClass2(deviceClass2);

    PRINTLOG(f->getParameters());
    int rowCount = tableWidget->rowCount();
    qDebug()<<rowCount;
    this->tableWidget->insertRow(rowCount);
    this->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(f->getVarIndex())));
//    this->tableWidget->setItem(rowCount,1,new QTableWidgetItem(f->getFunctionName()));
    this->tableWidget->setItem(rowCount,1,getNewTableWidgetItem(f));
    this->tableWidget->setItem(rowCount,2,new QTableWidgetItem(QString::number(f->getDirection())));
    MyPushButton *myButton = new MyPushButton(f);
    this->tableWidget->setCellWidget(rowCount,3,myButton);
    connect(myButton,SIGNAL(signalClicked(FunctionStruct*)),this,SLOT(slotEditClicked(FunctionStruct*)));
    updateFunctionNumber();
}

bool FunctionListWidget::upDateFunctionToDataBase(FunctionStruct *f)
{
    int id = f->getId();
    QSqlQuery query;
    QString str = "update function set direction = '"+
            QString::number(f->getDirection())+
            "', parameter = '" + f->getParameters()+
            "', function_number= '" + QString::number(f->getFunctionNumber())+
            "'where id = '" + QString::number(id)+"';";
    return query.exec(str);


}

bool FunctionListWidget::insertFunctionToDataBase(FunctionStruct *f)
{
    QSqlQuery query;
    QString str = "insert into generalcommand.function (device_id,gcmd_id,direction,parameter,dis_type,function_number)  values (:device_id, :gcmd_id, :direction, :parameter,:dis_type,:function_number)";
    //   QString str = "insert into generalcommand.test (a,b)  values (:a, :b)";


    qDebug()<<str;
    qDebug()<<query.prepare(str);
    query.bindValue(":device_id",this->deviceId);
    qDebug()<<f->getId();
    query.bindValue(":gcmd_id",f->getVarIndex());
    qDebug()<<f->getVarIndex()<<f->getDirection()<<f->getParameters();
    query.bindValue(":direction",f->getDirection());
    query.bindValue(":parameter",f->getParameters());
    query.bindValue(":dis_type",f->getDisType());
    query.bindValue(":function_number",f->getFunctionNumber());
    qDebug()<< query.exec();
    query.clear();
    query.exec("select last_insert_id() as lastid");
    query.next();
    f->setId(query.value("lastid").toInt());
    return true;

}

bool FunctionListWidget::deleteFunctionFromDataBase()
{
    PRINTLOG("");
    QList<int> idList;
    for(int i=0;i<functionList.size();i++)
    {
        idList.append(functionList.at(i)->getId());
        PRINTLOG(idList.last());
    }

    QSqlQuery query;
    QString str = "select id from function where device_id = :deviceid";
    QString deleteStr = "delete from function where id = :id";
    qDebug()<<query.prepare(str);
    query.bindValue(":deviceid",this->deviceId);
    qDebug()<<query.exec();
    while(query.next())
    {
        qDebug()<<query.value("id").toInt();
//        if(!idList.contains(query.value("id").toInt()))
//        {

//            QSqlQuery que;
//            que.prepare(deleteStr);
//            que.bindValue(":id",query.value("id").toInt());
//            que.exec();
//        }
        if(idList.contains(query.value("id").toInt()))
        {
           PRINTLOG("NODelete");
           PRINTLOG(query.value("id").toInt());
        }
        else
        {
            PRINTLOG("DELETE");
           PRINTLOG(query.value("id").toInt());
           QSqlQuery que;
           que.prepare(deleteStr);
           que.bindValue(":id",query.value("id").toInt());
           qDebug()<<que.exec();

        }
    }
    return true;
}

QTableWidgetItem *FunctionListWidget::getNewTableWidgetItem(FunctionStruct *f)
{
   QTableWidgetItem *item = new QTableWidgetItem(f->getFunctionName());
   item->setTextAlignment(Qt::AlignCenter);
   QString tipStr;
   tipStr ="Name:" + f->getFunctionName()
           + "\n Type:" + QString::number(f->getVarType());
   this->setToolTip(tipStr);

   return item;
}

void FunctionListWidget::slotUpdateDeviceID(int deviceId)
{
    clear();
    this->functionList.clear();
    this->tableWidget->clearContents();
    QSqlQuery query2;
    query2.exec("select * from device where id = '"+ QString::number(deviceId)+"';");
    query2.next();
    this->deviceClass1 = query2.value("device_class2_class1_id").toInt();
    this->deviceClass2 = query2.value("device_class2_class2_id").toInt();
    qDebug()<<deviceClass1<<deviceClass2;

    this->deviceId = deviceId;
    QSqlQuery query;
    QString str = "select *,function.id as funcId from general_command,function where general_command.id = function.gcmd_id and function.device_id = "
            + QString::number(deviceId) + " order by function_number ;";
    query.exec(str);
    while(query.next())
    {
        FunctionStruct *f = new FunctionStruct();
        f->setClass1(deviceClass1);
        f->setClass2(deviceClass2);
        f->setVarIndex(query.value("id").toInt());
        f->setId(query.value("funcId").toInt());
        f->setFunctionName(query.value("gcmd_name_ch").toString());
        f->setVarType(query.value("gcmd_type").toInt());
        f->setDirection(query.value("direction").toInt());
        f->setParameters(query.value("parameter").toString());
        f->setDisType(query.value("dis_type").toInt());
        f->setFunctionNumber(query.value("function_number").toInt());
        this->functionList.append(f);
        int rowCount = tableWidget->rowCount();
        this->tableWidget->insertRow(rowCount);
        this->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(f->getVarIndex())));

//        QTableWidgetItem *functionNameItem = new QTableWidgetItem(f->getFunctionName());
        QTableWidgetItem *functionNameItem = getNewTableWidgetItem(f);

        this->tableWidget->setItem(rowCount,1,functionNameItem);
        this->tableWidget->setItem(rowCount,2,new QTableWidgetItem(QString::number(f->getDirection())));
        MyPushButton *myButton = new MyPushButton(f);
        this->tableWidget->setCellWidget(rowCount,3,myButton);
        connect(myButton,SIGNAL(signalClicked(FunctionStruct*)),this,SLOT(slotEditClicked(FunctionStruct*)));
    }
    //    int rowCount = tableWidget->rowCount();
    //    this->tableWidget->insertRow(rowCount);
    //    this->addFunctionButton = new QPushButton("增加");
    //    this->saveFunctionButton = new QPushButton("保存");

    //    this->upmoveButton = new QPushButton("上移");
    //    this->downmoveButton = new QPushButton("下移");
    //    this->tableWidget->setCellWidget(rowCount,0,upmoveButton);
    //    this->tableWidget->setCellWidget(rowCount,1,downmoveButton);

    //    connect(upmoveButton,SIGNAL(clicked(bool)),this,SLOT(slotUpMovedClicked(bool)));
    //    connect(downmoveButton,SIGNAL(clicked(bool)),this,SLOT(slotDownMovedClicked(bool)));

    //    this->tableWidget->setCellWidget(rowCount,2,saveFunctionButton);
    //    connect(saveFunctionButton,SIGNAL(clicked(bool)),this,SLOT(slotSaveFunctions(bool)));
    //    connect(addFunctionButton,SIGNAL(clicked(bool)),this,SLOT(slotAddFunctions(bool)));
    //    this->tableWidget->setCellWidget(rowCount,3,addFunctionButton);
}

void FunctionListWidget::slotCellClicked(int i, int j)
{
}


void FunctionListWidget::slotEditClicked(FunctionStruct *f)
{
    emit signalParameters(f);

}

void FunctionListWidget::slotSaveFunctions(bool)
{
    //将数据保存至数据库

    for(int i=0; i<functionList.size();i++)
    {
        if(functionList.at(i)->getId() == 0)
        {
            //插入数据库

            insertFunctionToDataBase(functionList.at(i));
        }
        else
        {
            //更新数据库内容
            upDateFunctionToDataBase(functionList.at(i));
        }
    }
    deleteFunctionFromDataBase();
}

void FunctionListWidget::slotAddFunctions(bool)
{
    PRINTLOG("AddFunctions");
    this->addFunctionWidget = new AddFunctionsWidget(this->deviceClass1,this->deviceClass2);
    connect(this->addFunctionWidget,SIGNAL(signalAddFunctions(QList<int>)),this,SLOT(slotAddFunctions(QList<int>)));
    this->addFunctionWidget->show();

}

void FunctionListWidget::slotAddFunctions(QList<int> l)
{
    PRINTLOG(l);
    QString str = "(";
    for(int i=0;i<l.size();i++)
    {
        str += QString::number(l.at(i)) ;
        if(i != l.size()-1)
        {
            str += ",";

        }
    }
    str +=")";
    PRINTLOG(str);
    str = "SELECT * FROM general_command ,parameter_default where id in " +
            str + "and general_command.id = parameter_default.general_command_id";
    QSqlQuery query;
    query.exec(str);
    while(query.next())
    {
        PRINTLOG(query.value("gcmd_name_ch").toString());
        FunctionStruct *f = new FunctionStruct();
        f->setId(0);
        // this->functionList.append(f);
        f->setFunctionName(query.value("gcmd_name_ch").toString());
        f->setDirection(query.value("direction").toInt());
        f->setVarIndex(query.value("id").toInt());
        f->setVarType(query.value("gcmd_type").toInt());
        f->setParameters(query.value("parameter").toString());
        f->setDisType(query.value("distype").toInt());
        PRINTLOG(f->getDisType());
        insertAFunction(f);
    }
}

void FunctionListWidget::slotUpMovedClicked(bool)
{
    PRINTLOG(this->tableWidget->currentRow());
    int currentRow = this->tableWidget->currentRow();
    if(currentRow > 0)
    {
        QTableWidgetItem *commandItem = tableWidget->takeItem(currentRow,COMMAND_COLUMN);
        QTableWidgetItem *nameItem = tableWidget->takeItem(currentRow,NAME_COLUMN);
        QTableWidgetItem *directionItem = tableWidget->takeItem(currentRow,DIRECTION_COLUMN);
        QTableWidgetItem *precommandItem = tableWidget->takeItem(currentRow-1,COMMAND_COLUMN);
        QTableWidgetItem *prenameItem = tableWidget->takeItem(currentRow-1,NAME_COLUMN);
        QTableWidgetItem *predirectionItem = tableWidget->takeItem(currentRow-1,DIRECTION_COLUMN);

        tableWidget->setItem(currentRow,COMMAND_COLUMN,precommandItem);
        tableWidget->setItem(currentRow,NAME_COLUMN,prenameItem);
        tableWidget->setItem(currentRow,DIRECTION_COLUMN,predirectionItem);
        tableWidget->setItem(currentRow -1,COMMAND_COLUMN,commandItem);
        tableWidget->setItem(currentRow -1,NAME_COLUMN,nameItem);
        tableWidget->setItem(currentRow -1,DIRECTION_COLUMN,directionItem);

        MyPushButton *currentPushButton = (MyPushButton *)(tableWidget->cellWidget(currentRow,OPERATION_COLUMN));
        MyPushButton *prePushButton = (MyPushButton *)(tableWidget->cellWidget(currentRow-1,OPERATION_COLUMN));
        FunctionStruct *f = currentPushButton->getFunctionStruct();

        currentPushButton->setFunctionStruct(prePushButton->getFunctionStruct());
        prePushButton->setFunctionStruct(f);


        functionList.insert(currentRow-1,functionList.takeAt(currentRow));
        for(int i=0;i<functionList.size();i++)
            qDebug()<<functionList.at(i)->getFunctionName();

        this->tableWidget->selectRow(currentRow-1);
    }
    updateFunctionNumber();

}

void FunctionListWidget::slotDownMovedClicked(bool)
{
    int currentRow = this->tableWidget->currentRow();
    int total = this->tableWidget->rowCount();
    PRINTLOG("NEXT");
    //    this->insertAFunction(functionList.at(tableWidget->currentRow()));
    // QTableWidgetItem * commandItem = tableWidget->takeItem(0,0);
    // PRINTLOG(commandItem->text());
    if(currentRow < total-1 && currentRow >= 0)
    {

        QTableWidgetItem *commandItem = tableWidget->takeItem(currentRow,COMMAND_COLUMN);
        QTableWidgetItem *nameItem = tableWidget->takeItem(currentRow,NAME_COLUMN);
        QTableWidgetItem *directionItem = tableWidget->takeItem(currentRow,DIRECTION_COLUMN);
        QTableWidgetItem *nextcommandItem = tableWidget->takeItem(currentRow+1,COMMAND_COLUMN);
        QTableWidgetItem *nextnameItem = tableWidget->takeItem(currentRow+1,NAME_COLUMN);
        QTableWidgetItem *nextdirectionItem = tableWidget->takeItem(currentRow+1,DIRECTION_COLUMN);

        tableWidget->setItem(currentRow,COMMAND_COLUMN,nextcommandItem);
        tableWidget->setItem(currentRow,NAME_COLUMN,nextnameItem);
        tableWidget->setItem(currentRow,DIRECTION_COLUMN,nextdirectionItem);
        tableWidget->setItem(currentRow +1,COMMAND_COLUMN,commandItem);
        tableWidget->setItem(currentRow +1,NAME_COLUMN,nameItem);
        tableWidget->setItem(currentRow +1,DIRECTION_COLUMN,directionItem);

        MyPushButton *currentPushButton = (MyPushButton *)(tableWidget->cellWidget(currentRow,OPERATION_COLUMN));
        MyPushButton *nextPushButton = (MyPushButton *)(tableWidget->cellWidget(currentRow+1,OPERATION_COLUMN));
        FunctionStruct *f = currentPushButton->getFunctionStruct();
        currentPushButton->setFunctionStruct(nextPushButton->getFunctionStruct());
        nextPushButton->setFunctionStruct(f);
        functionList.insert(currentRow+1,functionList.takeAt(currentRow));
        for(int i=0;i<functionList.size();i++)
             qDebug()<<functionList.at(i)->getFunctionName();
        this->tableWidget->selectRow(currentRow+1);
    }
    updateFunctionNumber();

}

void FunctionListWidget::slotCreateClicked(bool)
{

    PRINTLOG(this->deviceId);
    HeadInformation h;
    h.getInfoFromDeviceId(deviceId);
    PRINTLOG(h.getDeviceName());


    QString defaultFileName;

    defaultFileName  = "TCLSH"+h.getManufactureCode();
    if(QString::number(h.getDeviceClass1()).length() == 1)
    {
        defaultFileName = defaultFileName + "0" + QString::number(h.getDeviceClass1());
    }
    else
    {
        defaultFileName = defaultFileName + QString::number(h.getDeviceClass1());
    }
    if(QString::number(h.getDeviceClass2()).length() == 1)
    {
        defaultFileName = defaultFileName + "0" + QString::number(h.getDeviceClass2());
    }
    else
    {
        defaultFileName = defaultFileName + QString::number(h.getDeviceClass2());
    }

    int cl = 6-h.getDevcieModeID().length();
    while(cl >0)
    {
        defaultFileName +="0";
        cl--;
    }
    defaultFileName = defaultFileName + h.getDevcieModeID();


    if(QString::number(h.getFilever()).length()<2)
    {
        defaultFileName = defaultFileName + QString("0") + QString::number(h.getFilever());
    }
    else
    {
        defaultFileName = defaultFileName + QString::number(h.getFilever());
    }




    QString fileName = QFileDialog::getSaveFileName(this,"SaveFile",defaultFileName,"xml(*.xml);;Json(.json)");
    if(fileName.isNull())
    {
        return;
    }

    if(QFileInfo(fileName).suffix() == "json")
    {
//        GenerateDescriptionFile g;
//        g.CreateJson(head,list);
    }
    else if (QFileInfo(fileName).suffix() == "xml")
    {
//       GenerateDescriptionFile g(Pos);
//       g.CreateXml(fileName,head,list);

            FileGenerater g;

            g.CreateXml(fileName,&h,&functionList);
            g.CreateDocument(fileName,&functionList);
    }
    else
    {

    }


//    FileGenerater g;

//    g.CreateXml("a.xml",&h,&functionList);
}

void FunctionListWidget::slotDeleteClicked(bool)
{
    PRINTLOG("");
    int currentRow = this->tableWidget->currentRow();
    PRINTLOG(currentRow);
    if(currentRow >= 0)
    {
        delete functionList.takeAt(currentRow);
        tableWidget->removeRow(currentRow);
        if(currentRow >= tableWidget->rowCount())
             tableWidget->selectRow(currentRow-1);
        else
            tableWidget->selectRow(currentRow);

    }
    updateFunctionNumber();

}

void FunctionListWidget::slotRowClicked(QModelIndex i)
{
    qDebug()<<i;
}


