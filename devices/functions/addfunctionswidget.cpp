#include "addfunctionswidget.h"
#include "ui_addfunctionswidget.h"

AddFunctionsWidget::AddFunctionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFunctionsWidget)
{
    ui->setupUi(this);
}
AddFunctionsWidget::AddFunctionsWidget(int class1,int class2,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFunctionsWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Add a Function");
    this->deviceclass1= class1;
    this->deviceclass2 = class2;
    init();
}

AddFunctionsWidget::~AddFunctionsWidget()
{
    delete ui;
}

void AddFunctionsWidget::init()
{
    QSqlQuery query;
    QString str = "select id,gcmd_name_ch from general_command where (device_class = "
            +QString::number(deviceclass1) + " or device_class = 0) and gcmd_type <> 4   order by id" ;
    PRINTLOG(str);
    query.exec(str);
    while(query.next())
    {
        MyListWidgetItem *i = new  MyListWidgetItem(query.value("gcmd_name_ch").toString(),
                                                       query.value("id").toInt());
        this->listItemList.append(i);
        this->ui->listWidget->addItem(i);
    }
}

MyListWidgetItem::MyListWidgetItem(const QString text, int varIndex):QListWidgetItem(text)
{
    qDebug()<<varIndex;
    this->varIndex = varIndex;
}
int MyListWidgetItem::getVarIndex() const
{
    return varIndex;
}

MyListWidgetItem::~MyListWidgetItem()
{
 //  PRINTLOG("MyListWidgetItemdestruct");
}


void AddFunctionsWidget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    this->selectedListItemList.append(new MyListWidgetItem(item->text(),((MyListWidgetItem*)item)->getVarIndex()));
    this->ui->selectlistWidget->addItem(selectedListItemList.last());
}

void AddFunctionsWidget::on_searchlineEdit_textChanged(const QString &arg1)
{

    for(int i=0;i<listItemList.size();i++)
    {
        if(listItemList.at(i)->text().contains(arg1))
        {
            this->listItemList.at(i)->setHidden(false);
        }
        else
        {
            this->listItemList.at(i)->setHidden(true);
        }
    }
}

void AddFunctionsWidget::on_addpushButton_clicked()
{
    QList<int> list;
    for(int i=0;i<selectedListItemList.size();i++)
    {
        list.append(selectedListItemList.at(i)->getVarIndex());
    }
    emit signalAddFunctions(list);
    this->selectedListItemList.clear();
    this->ui->selectlistWidget->clear();
    this->close();
}

void AddFunctionsWidget::on_cancelpushButton_clicked()
{
    this->close();
}
