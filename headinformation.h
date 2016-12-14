#ifndef HEADINFORMATION_H
#define HEADINFORMATION_H

#include <QObject>
#include "config.h"
#include <QSqlQuery>

class HeadInformation
{
public:
    HeadInformation();

    void getInfoFromDeviceId(int id);
    QString getDeviceName() const;
    void setDeviceName(const QString &value);

    QString getDevcieMode() const;
    void setDevcieMode(const QString &value);

    QString getDevcieModeID() const;
    void setDevcieModeID(const QString &value);

    int getDeviceClass1() const;
    void setDeviceClass1(int value);

    int getDeviceClass2() const;
    void setDeviceClass2(int value);

    QString getManufactureCode() const;
    void setManufactureCode(const QString &value);

    QString getUrl() const;
    void setUrl(const QString &value);

    bool getUpdate() const;
    void setUpdate(bool value);

    bool getSync() const;
    void setSync(bool value);

    int getFilever() const;
    void setFilever(int value);

private:
    QString deviceName;
    QString devcieMode;
    QString devcieModeID;
    int deviceClass1;
    int deviceClass2;
    QString manufactureCode;
    QString url;
    int filever;

    bool update;
    bool sync;
};

#endif // HEADINFORMATION_H
