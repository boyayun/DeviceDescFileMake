#include "userinformation.h"

UserInformation::UserInformation(int userId)
{
    this->userId = userId;
    getInformationFromId();

}
QString UserInformation::getUserName() const
{
    return userName;
}

void UserInformation::setUserName(const QString &value)
{
    userName = value;
}
QString UserInformation::getManufactureId() const
{
    return manufactureId;
}

void UserInformation::setManufactureId(const QString &value)
{
    manufactureId = value;
}
QMap<int, QString> UserInformation::getDevices() const
{
    return devices;
}

void UserInformation::setDevices(const QMap<int, QString> &value)
{
    devices = value;
}
int UserInformation::getUserId() const
{
    return userId;
}

void UserInformation::setUserId(int value)
{
    userId = value;
}

void UserInformation::updateNewInformation()
{
   getInformationFromId();
}




bool UserInformation::getInformationFromId()
{
    getUserInfo();
    getDeviceInfo();
    QMap<int,QString>::iterator i;
    for(i = devices.begin();i!= devices.end();i++)
    {
        qDebug()<<i.key()<<i.value();
    }
    return true;

}

bool UserInformation::getUserInfo()
{
    QSqlQuery query;
    QString queryStr = "select * from " + TABLE_USERINFO + " where user_id = "+QString::number(userId);
    bool isSuccess = query.exec(queryStr);
    if(isSuccess)
    {
        while(query.next())
        {
            setUserName(query.value("name").toString());
            setManufactureId(query.value("manufacture_id").toString());
        }
    }
    else
    {
        qDebug()<<__FILE__<<__LINE__<<"getDeviceInfo Error";
        return false;
    }
    return true;
}

bool UserInformation::getDeviceInfo()
{
    QSqlQuery query;
    QString queryStr = "select * from " + TABLE_DEVICE + " where user_id = "+QString::number(userId);
    bool isSuccess = query.exec(queryStr);
    if(isSuccess)
    {
        while(query.next())
        {
            int deviceId = query.value("id").toInt();
            QString deviceName = query.value("device_name").toString();
            devices.insert(deviceId,deviceName);
        }
    }
    else
    {
        qDebug()<<__FILE__<<__LINE__<<"getDeviceInfo Error";
        return false;
    }
    return true;
}




