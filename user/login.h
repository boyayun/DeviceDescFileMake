#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_loginBtn_clicked();

    void on_cancleBtn_clicked();

    void keyPressEvent(QKeyEvent *e);



private:
    Ui::Login *ui;

signals:
    void signalLoginState(int);
};

#endif // LOGIN_H
