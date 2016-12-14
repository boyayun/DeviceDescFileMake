#ifndef MANAGEFUNCTIONWIDGET_H
#define MANAGEFUNCTIONWIDGET_H
#include"functionstruct.h"
#include"devices/functions/parameterwidget.h"
#include <QPushButton>
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QObject>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>


class ManageFunctionWidget:public QWidget
{
    Q_OBJECT

public:
    ManageFunctionWidget(FunctionStruct *f,QWidget *parent = 0);
private:
    QVBoxLayout *vboxLayout;
    QPushButton *saveButton;
    QComboBox *typeCombox;
    ParameterWidget *parameterWidget;
    FunctionStruct *functionStruct;

    void writeToDataBase();
public slots:
    void slotTypeChanged(int);
    void slotSaveClicked(bool);



};

#endif // MANAGEFUNCTIONWIDGET_H
