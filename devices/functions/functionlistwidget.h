#ifndef FUNCTIONLISTWIDGET_H
#define FUNCTIONLISTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QSqlResult>
#include <QList>
#include <QPushButton>
#include <QComboBox>
#include <QHeaderView>
#include <QFileDialog>

#include "config.h"
#include "functionstruct.h"
#include "editbutton.h"
#include "mypushbutton.h"
#include "devices/functions/parameterwidget.h"
#include "devices/functions/addfunctionswidget.h"
#include "headinformation.h"
#include "devices/filegenerater.h"

#define COMMAND_COLUMN 0
#define NAME_COLUMN  1
#define DIRECTION_COLUMN  2
#define OPERATION_COLUMN 3




class FunctionListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionListWidget(QWidget *parent = 0);
    explicit FunctionListWidget(int deviceid,QWidget *parent = 0);
private:
    void init();
    void clear();
    void getHeadInformation();
    void updateFunctionNumber();
    QVBoxLayout *mainLayout;
    QHBoxLayout *operationLayout;
    QTableWidget *tableWidget;
    QList <FunctionStruct *> functionList;
    QPushButton *addFunctionButton;
    QPushButton *saveFunctionButton;
    QPushButton *upmoveButton;
    QPushButton *downmoveButton;
    QPushButton *createButton;
    QPushButton *deleteButton;
    AddFunctionsWidget *addFunctionWidget;
    int deviceClass1;
    int deviceClass2;

    int deviceId;

    void insertAFunction(FunctionStruct * f);

    bool upDateFunctionToDataBase(FunctionStruct *f);
    bool insertFunctionToDataBase(FunctionStruct *f);
    bool deleteFunctionFromDataBase();

    QTableWidgetItem * getNewTableWidgetItem(FunctionStruct *f);


signals:

    void signalParameters(FunctionStruct *f);

public slots:
    void slotUpdateDeviceID(int deviceId);

    void slotCellClicked(int,int);

    void slotEditClicked(FunctionStruct *f);



private slots:
    void slotAddFunctions(bool);

    void slotSaveFunctions(bool);

    void slotAddFunctions(QList<int> l);

    void slotUpMovedClicked(bool);

    void slotDownMovedClicked(bool);

    void slotCreateClicked(bool);

    void slotDeleteClicked(bool);

    void slotRowClicked(QModelIndex i);



};

#endif // FUNCTIONLISTWIDGET_H
