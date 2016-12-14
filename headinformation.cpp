#include "headinformation.h"

HeadInformation::HeadInformation()
{
    this->filever = DD_FILE_VER;

}

void HeadInformation::getInfoFromDeviceId(int id)
{
    QSqlQuery query;
    QString str = "select * from device where id = (:id)";
    query.prepare(str);
    query.bindValue(":id",id);
    query.exec();
    while(query.next())
    {
        this->setDeviceName(query.value("device_name").toString());
        int class2=query.value("device_class2_class2_id").toInt();
        int class1 = query.value("device_class2_class1_id").toInt();
        this->setDeviceClass1(class1);
        this->setDeviceClass2(class2);
        this->setManufactureCode(query.value("manufactureID").toString());

        this->setDevcieMode(query.value("device_model").toString());
        this->setDevcieModeID(query.value("device_modelID").toString());
        this->setUrl(query.value("url").toString());
        this->setUpdate(query.value("up").toBool());
        this->setSync(query.value("syn").toBool());
    }

}
QString HeadInformation::getDeviceName() const
{
    return deviceName;
}

void HeadInformation::setDeviceName(const QString &value)
{
    deviceName = value;
}
QString HeadInformation::getDevcieMode() const
{
    return devcieMode;
}

void HeadInformation::setDevcieMode(const QString &value)
{
    devcieMode = value;
}
QString HeadInformation::getDevcieModeID() const
{
    return devcieModeID;
}

void HeadInformation::setDevcieModeID(const QString &value)
{
    devcieModeID = value;
}
int HeadInformation::getDeviceClass1() const
{
    return deviceClass1;
}

void HeadInformation::setDeviceClass1(int value)
{
    deviceClass1 = value;
}
int HeadInformation::getDeviceClass2() const
{
    return deviceClass2;
}

void HeadInformation::setDeviceClass2(int value)
{
    deviceClass2 = value;
}
QString HeadInformation::getManufactureCode() const
{
    return manufactureCode;
}

void HeadInformation::setManufactureCode(const QString &value)
{
    manufactureCode = value;
}
QString HeadInformation::getUrl() const
{
    return url;
}

void HeadInformation::setUrl(const QString &value)
{
    url = value;
}
bool HeadInformation::getUpdate() const
{
    return update;
}

void HeadInformation::setUpdate(bool value)
{
    update = value;
}
bool HeadInformation::getSync() const
{
    return sync;
}

void HeadInformation::setSync(bool value)
{
    sync = value;
}
int HeadInformation::getFilever() const
{
    return filever;
}

void HeadInformation::setFilever(int value)
{
    filever = value;
}











