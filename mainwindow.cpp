#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addDeviceWidget = NULL;
    this->parameterWidget = NULL;
//    writeSettings();
    setting();
}

MainWindow::MainWindow(int, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addDeviceWidget = NULL;
    login = new Login();
    connect(login,SIGNAL(signalLoginState(int)),this,SLOT(slotLoginSuccess(int)));

    login->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setting()
{
    login = new Login();
    connect(login,SIGNAL(signalLoginState(int)),this,SLOT(slotLoginSuccess(int)));

    mainSplitter = new QSplitter(Qt::Horizontal);
    userSplitter = new QSplitter(Qt::Vertical);
    stackWidget = new QStackedWidget();

    mainSplitter->addWidget(userSplitter);

    functionListWidget = new FunctionListWidget();
    functionListWidget->setEnabled(false);

    devInfoWidget = new deviceInformationWidget();
    connect(functionListWidget,SIGNAL(signalParameters(FunctionStruct*)),this,SLOT(slotParameters(FunctionStruct*)));
    mainSplitter->setStretchFactor(0,3);
    mainSplitter->addWidget(stackWidget);
    stackWidget->addWidget(functionListWidget);
    stackWidget->addWidget(devInfoWidget);
    mainSplitter->setStretchFactor(1,4);




    this->setCentralWidget(mainSplitter);

    login->exec();

    connect(ui->action_N,SIGNAL(triggered(bool)),this,SLOT(slotAddDevice(bool)));
    connect(ui->actionManage,SIGNAL(triggered(bool)),this,SLOT(slotManage(bool)));
    connect(ui->action_Q,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->action,SIGNAL(triggered(bool)),this,SLOT(slotAuthor(bool)));
    connect(ui->action_H,SIGNAL(triggered(bool)),this,SLOT(slotHelp(bool)));

}

void MainWindow::writeSettings()
{
    QSettings settings("setting.ini",QSettings::IniFormat);
    settings.setObjectName("setting.ini");
    settings.setValue("Address","127.0.0.1");
    settings.setValue("Port",3306);
    settings.setValue("CurrentUser","");
}

void MainWindow::slotLoginSuccess(int id)
{
    if(id == 1)     //超级用户
    {
        this->ui->actionManage->setEnabled(true);
    }
    else
    {
        this->ui->actionManage->setEnabled(false);
    }
    userInfo = new UserInformation(id);//登录成功，获取用户信息
    userInformationWidget = new UserInformationWidget(userInfo);
    userSplitter->addWidget(userInformationWidget);

    deviceTreeWidget = new DeviceTreeWidget(userInfo->getDevices());
    connect(deviceTreeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slotDeviceItemClicked(QTreeWidgetItem*,int)));

    userSplitter->addWidget(deviceTreeWidget);
    userSplitter->setStretchFactor(0,1);
    userSplitter->setStretchFactor(1,5);
    userSplitter->setAutoFillBackground(true);

}

void MainWindow::slotDeviceItemClicked(QTreeWidgetItem *widgetItem, int )
{
    MyTreeWidgetItem *myWidgetItem = (MyTreeWidgetItem *)widgetItem->parent();
    if(widgetItem->parent())
    {
        if(widgetItem->text(0).contains("D"))
        {
            this->stackWidget->setCurrentWidget(devInfoWidget);
            this->devInfoWidget->updateFromDeviceID(myWidgetItem->getDeviceId());
        }
        else
        {
        qDebug()<<widgetItem->parent()->text(0);
        this->functionListWidget->slotUpdateDeviceID(myWidgetItem->getDeviceId());
        this->functionListWidget->setEnabled(true);
        this->stackWidget->setCurrentWidget(functionListWidget);
        }
    }
}

void MainWindow::slotParameters(FunctionStruct *f)
{
    PRINTLOG(f->getFunctionName());
    if(this->parameterWidget)
    {
        delete this->parameterWidget;
    }
    parameterWidget = new ParameterWidget(f);
    parameterWidget->show();
}

void MainWindow::slotAddDevice(bool)
{
    if(NULL == this->addDeviceWidget)
    {
        addDeviceWidget = new AddDeviceWidget(userInfo,this);
        connect(addDeviceWidget,SIGNAL(signalUpdateTreeList()),this,SLOT(slotUpdateDeviceList()));
    }
    else
    {
    }
    addDeviceWidget->exec();
}

void MainWindow::slotUpdateDeviceList()
{
    userInfo->updateNewInformation();
    this->deviceTreeWidget->updateTreeWidget(userInfo->getDevices());
}

void MainWindow::slotManage(bool)
{
    FunctionStruct *f = new FunctionStruct();
    f->setVarType(1);
    f->setFunctionName("");
    f->setDirection(3);
    f->setVarIndex(500);
    ManageFunctionWidget *m = new ManageFunctionWidget(f);
    m->show();
}

void MainWindow::slotHelp(bool)
{
    QDesktopServices::openUrl(QUrl("help.html"));
}

void MainWindow::slotAuthor(bool)
{
    AboutDialog abouts;
    abouts.exec();
}


