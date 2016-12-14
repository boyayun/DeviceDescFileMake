#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include "functionstruct.h"

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton();
    MyPushButton(FunctionStruct *f);
    ~MyPushButton();
    FunctionStruct *getFunctionStruct() const;
    void setFunctionStruct(FunctionStruct *value);

private:
    FunctionStruct *functionStruct;
private slots:
    void slottest(bool b);
signals:
    void signalClicked(FunctionStruct *);

};

#endif // MYPUSHBUTTON_H
