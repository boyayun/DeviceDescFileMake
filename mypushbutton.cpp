#include "mypushbutton.h"

MyPushButton::MyPushButton():QPushButton()
{

    connect(this,SIGNAL(clicked(bool)),this,SLOT(slottest(bool)));
}

MyPushButton::MyPushButton(FunctionStruct *f):QPushButton()
{
    this->functionStruct = f;
    connect(this,SIGNAL(clicked(bool)),this,SLOT(slottest(bool)));
    this->setText("编辑");
}

MyPushButton::~MyPushButton()
{
    PRINTLOG("DESCTRUCT");

}
FunctionStruct *MyPushButton::getFunctionStruct() const
{
    return functionStruct;
}

void MyPushButton::setFunctionStruct(FunctionStruct *value)
{
    functionStruct = value;
}


void MyPushButton::slottest(bool b)
{
    emit signalClicked(this->functionStruct);

}

