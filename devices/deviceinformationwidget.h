#ifndef DEVICEINFORMATIONWIDGET_H
#define DEVICEINFORMATIONWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include "config.h"

namespace Ui {
class deviceInformationWidget;
}

class deviceInformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit deviceInformationWidget(QWidget *parent = 0);
    ~deviceInformationWidget();
    void updateFromDeviceID(int deviceid);

private slots:
    void on_savepushButton_clicked();

private:
    Ui::deviceInformationWidget *ui;
    int userId;
    int deviceID;
    int class_1;
    int class_2;
    QString deviceName;
    QString manufactureID;
    QString url;
    QString deviceModel;
    QString deviceModelID;
    bool isUpdate;
    bool isSync;
};

#endif // DEVICEINFORMATIONWIDGET_H
