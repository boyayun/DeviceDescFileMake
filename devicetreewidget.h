#ifndef DEVICETREEWIDGET_H
#define DEVICETREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QDebug>

class MyTreeWidgetItem:public QTreeWidgetItem
{
public:
    MyTreeWidgetItem(const QStringList &strings);
    int getDeviceId() const;
    void setDeviceId(int value);

private:
    int deviceId;


};


class DeviceTreeWidget : public QTreeWidget
{
public:
    DeviceTreeWidget(QMap<int,QString>lists);
    void updateTreeWidget(QMap<int,QString>lists);
    
};

class DeviceTreeWidgetItem:public QTreeWidgetItem
{
    enum ItemType
    {
        DeviceParameter = 1,
        DeviceInformation = 2
    };

public:

    DeviceTreeWidgetItem();
};

#endif // DEVICETREEWIDGET_H
