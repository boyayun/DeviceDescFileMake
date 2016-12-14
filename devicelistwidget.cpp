#include "devicelistwidget.h"

DeviceListWidget::DeviceListWidget(QMap<int, QString> lists):QListWidget()
{
    for(QMap<int,QString>::iterator it = lists.begin();it!=lists.end();it++)
    {
        this->addItem(new QListWidgetItem(it.value()));
    }
}

