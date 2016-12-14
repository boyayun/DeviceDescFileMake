#ifndef USERINFORMATIONWIDGET_H
#define USERINFORMATIONWIDGET_H

#include <QWidget>
#include "user/userinformation.h"

namespace Ui {
class UserInformationWidget;
}

class UserInformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserInformationWidget(QWidget *parent = 0);
    UserInformationWidget(UserInformation *userInfo, QWidget *parent = 0);
    ~UserInformationWidget();

private:
    Ui::UserInformationWidget *ui;
};

#endif // USERINFORMATIONWIDGET_H
