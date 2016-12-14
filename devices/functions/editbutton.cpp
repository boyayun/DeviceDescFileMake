#include "editbutton.h"

EditButton::EditButton(QWidget *parent):QPushButton(parent)
{
    this->setText("edit");
    connect(this,SIGNAL(clicked(bool)),this,SLOT(slotTest(bool)));

}

EditButton::EditButton(FunctionStruct *f, QWidget *parent):QPushButton(parent)
{
    this->setText("Edit");
    this->functionStruct = f;
    PRINTLOG(f->getFunctionName());

    connect(this,SIGNAL(clicked(bool)),this,SLOT(slotClicked(bool)));

}

EditButton::~EditButton()
{
    PRINTLOG("Disconstruction");

}

void EditButton::slotClicked(bool)
{
    PRINTLOG("clicked");
    PRINTLOG(functionStruct->getFunctionName());

}

void EditButton::slotTest(bool)
{

}


