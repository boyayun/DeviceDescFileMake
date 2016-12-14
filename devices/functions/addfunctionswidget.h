#ifndef ADDFUNCTIONSWIDGET_H
#define ADDFUNCTIONSWIDGET_H

#include <QWidget>
#include "config.h"
#include <QSqlQuery>
#include <QListWidgetItem>
#include <QList>
class MyListWidgetItem : public QListWidgetItem
{
public:
    explicit MyListWidgetItem(const QString text,int varIndex);
    int getVarIndex() const;
    ~MyListWidgetItem();

private:
    int varIndex;
};


namespace Ui {
class AddFunctionsWidget;
}

class AddFunctionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddFunctionsWidget(QWidget *parent = 0);
    AddFunctionsWidget(int class1,int class2,QWidget *parent = 0);
    ~AddFunctionsWidget();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_searchlineEdit_textChanged(const QString &arg1);

    void on_addpushButton_clicked();

    void on_cancelpushButton_clicked();

private:
    Ui::AddFunctionsWidget *ui;
    int deviceclass1;
    int deviceclass2;
    QList<MyListWidgetItem *> listItemList;
    QList<MyListWidgetItem *> selectedListItemList;

    void init();
signals:
    void signalAddFunctions(QList<int> l);
};

#endif // ADDFUNCTIONSWIDGET_H
