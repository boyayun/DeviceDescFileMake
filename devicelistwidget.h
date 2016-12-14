#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMap>

class DeviceListWidget : public QListWidget
{
public:
    DeviceListWidget(QMap<int,QString> lists);
};

#endif // DEVICELISTWIDGET_H
