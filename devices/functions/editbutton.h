#ifndef EDITBUTTON_H
#define EDITBUTTON_H

#include<QPushButton>
#include<QWidget>
#include <QObject>
#include <functionstruct.h>
#include "config.h"


class EditButton : public QPushButton
{
public:
    EditButton(QWidget *parent = 0);
    EditButton(FunctionStruct * f,QWidget *parent = 0);
    ~EditButton();
private:
    FunctionStruct *functionStruct;
public slots:
    void slotClicked(bool);
    void slotTest(bool);
signals:
    void signalClicked(FunctionStruct *f);

};

#endif // EDITBUTTON_H
