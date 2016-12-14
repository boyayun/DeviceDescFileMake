#ifndef THIRDPARAMETERSWIDGET_H
#define THIRDPARAMETERSWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "functionstruct.h"

namespace Ui {
class ThirdParametersWidget;
}

class ThirdParametersWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ThirdParametersWidget(QWidget *parent = 0);
    explicit ThirdParametersWidget(QString parameterstr,QWidget *parent = 0);
    ~ThirdParametersWidget();


    QString getParameterStr() const;
    void setParameterStr(const QString &value);

    bool getIsNeedSave() const;

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::ThirdParametersWidget *ui;
    QList<QLineEdit*> parameterEditList;
    FunctionStruct *functionStruct;
    QString parameterStr;
    bool isNeedSave;
};

#endif // THIRDPARAMETERSWIDGET_H
