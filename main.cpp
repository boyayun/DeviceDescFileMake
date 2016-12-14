#include "mainwindow.h"
#include <QApplication>
#include "formtest.h"
#include "devices/functions/functionlistwidget.h"
#include "mypushbutton.h"
#include <QDir>
#include <QSqlDatabase>
#include <QSettings>
#include <settingwidget.h>
#include <QProgressBar>
bool connectToDataBase()
{
    QSqlDatabase dataBase = QSqlDatabase::addDatabase("QMYSQL");
    QSettings s("setting.ini",QSettings::IniFormat); //配置文件读取配置信息
    dataBase.setHostName(s.value("Address").toString());
    dataBase.setPort(s.value("Port").toInt());
    dataBase.setDatabaseName("generalcommand");
    dataBase.setUserName("jiaqi");
    dataBase.setPassword("123456");
    try
    {
         bool isOpen = dataBase.open();
         if(isOpen)
         {
            QSqlQuery query;
            return query.exec("select 1");
         }
         else
         {
             return false;
         }
    }
    catch (...)
    {
         return false;
    }
    return true;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString strLibPath(QDir::toNativeSeparators(QApplication::applicationDirPath())+
                       QDir::separator()+"plugins");
    PRINTLOG(strLibPath);
    qApp->addLibraryPath(strLibPath);
    qApp->addLibraryPath("./plugins");
    if(connectToDataBase())
    {
        MainWindow *w = new MainWindow();
        w->show();
    }
    else
    {
        SettingWidget *s = new SettingWidget();
        s->show();
    }
//    FormTest t;
//    t.show();
    return a.exec();
}
