#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QSplitter>
#include<QWidgetItem>
#include <QStackedWidget>
#include <QDesktopServices>


#include "user/login.h"
#include "user/userinformation.h"
#include "user/userinformationwidget.h"
#include "devicelistwidget.h"
#include "devicetreewidget.h"
#include "config.h"
#include "devices/functions/functionlistwidget.h"
#include "functionstruct.h"
#include "devices/functions/parameterwidget.h"
#include "devices/deviceinformationwidget.h"
#include "devices/adddevicewidget.h"
#include "managefunctionwidget.h"
#include "settingwidget.h"
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    MainWindow(int,QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Login *login;
    QSplitter *mainSplitter;
    QSplitter *userSplitter;
    UserInformation *userInfo;
    UserInformationWidget *userInformationWidget;
    DeviceListWidget *deviceListWidget;
    DeviceTreeWidget *deviceTreeWidget;
    FunctionListWidget *functionListWidget;
    deviceInformationWidget *devInfoWidget;
    QStackedWidget *stackWidget;
    AddDeviceWidget *addDeviceWidget;
    ParameterWidget *parameterWidget;

    void setting();
    void writeSettings();

private slots:
    void slotLoginSuccess(int id);
    void slotDeviceItemClicked(QTreeWidgetItem* widgetItem,int n);

    void slotParameters(FunctionStruct *f);

    void slotAddDevice(bool);

    void slotUpdateDeviceList();

    void slotManage(bool);

    void slotHelp(bool);

    void slotAuthor(bool);


};

#endif // MAINWINDOW_H
