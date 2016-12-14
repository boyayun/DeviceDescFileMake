#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

#include "config.h"



class UserInformation
{
public:
    UserInformation(int userId);
    QString getUserName() const;
    void setUserName(const QString &value);

    QString getManufactureId() const;
    void setManufactureId(const QString &value);


    QMap<int, QString> getDevices() const;
    void setDevices(const QMap<int, QString> &value);

    int getUserId() const;
    void setUserId(int value);

    void updateNewInformation();

private:
    int userId;
    QString userName;
    QString manufactureId;
    QMap <int,QString> devices;

    bool getInformationFromId();
    bool getUserInfo();
    bool getDeviceInfo();


};

#endif // USERINFORMATION_H
