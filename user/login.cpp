#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("Login");
   // this->setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    this->ui->registerpushButton->setFlat(true);

}

Login::~Login()
{
    delete ui;
}

void Login::on_loginBtn_clicked()
{

    QString username = ui->userNameLedt->text();
    QString passwd = ui->passwdLedt->text();
//    QSqlQuery query;
//    query.exec("select * from user");
//    while(query.next())
//    {
//        qDebug()<<query.value("id").toString();
//    }

//    QString sqlStr = "select user_function ('"+username+"','"+passwd+"') as id";

    QString sqlStr = "select id from user where user_name = :username and user_passwd = :userpasswd";


    QSqlQuery query;
    query.prepare(sqlStr);
    query.bindValue(":username",username);
    query.bindValue(":userpasswd",passwd);
    qDebug()<<query.exec();
    query.next();
//    query.exec(sqlStr);
//    query.next();
    qDebug()<<"xx"<<query.value("id").toInt();
    int state = query.value("id").toInt();
    if(state)//登录成功
    {
        emit signalLoginState(query.value(0).toInt());
        close();
    }
    else
    {

    }

}

void Login::on_cancleBtn_clicked()
{
    exit(-1);
}

void Login::keyPressEvent(QKeyEvent *e)
{

}
