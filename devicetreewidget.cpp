#include "devicetreewidget.h"

DeviceTreeWidget::DeviceTreeWidget(QMap<int, QString> lists)
{
    this->setHeaderLabel("DeviceName");

    for(QMap<int,QString>::iterator it = lists.begin();it!=lists.end();it++)
    {
//        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(it.value()));
        MyTreeWidgetItem *item = new MyTreeWidgetItem(QStringList(it.value()));
        item->setDeviceId(it.key());
        this->addTopLevelItem(item);
        item->addChild(new QTreeWidgetItem(QStringList("DeviceInformation")));
        item->addChild(new QTreeWidgetItem(QStringList("ParametersInformation")));
    }
}

void DeviceTreeWidget::updateTreeWidget(QMap<int, QString> lists)
{
    this->clear();
    this->setHeaderLabel("DeviceName");
    for(QMap<int,QString>::iterator it = lists.begin();it!=lists.end();it++)
    {
        MyTreeWidgetItem *item = new MyTreeWidgetItem(QStringList(it.value()));
        item->setDeviceId(it.key());
        this->addTopLevelItem(item);
        item->addChild(new QTreeWidgetItem(QStringList("DeviceInformation")));
        item->addChild(new QTreeWidgetItem(QStringList("ParametersInformation")));
    }
}



DeviceTreeWidgetItem::DeviceTreeWidgetItem()
{

}


MyTreeWidgetItem::MyTreeWidgetItem(const QStringList &strings):QTreeWidgetItem(strings)
{

}
int MyTreeWidgetItem::getDeviceId() const
{
    return deviceId;
}

void MyTreeWidgetItem::setDeviceId(int value)
{
    deviceId = value;
}

