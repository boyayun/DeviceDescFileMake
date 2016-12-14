#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>
#include <functionstruct.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>
#include <QIntValidator>
#include <QButtonGroup>
#include <QSpacerItem>
#include <QList>
#include <QComboBox>
#include <QSqlQuery>
#include <QListWidgetItem>
#include <QMessageBox>

class AlarmListWidgetItem : public QListWidgetItem
{
public:
    explicit AlarmListWidgetItem(const QString text,int varIndex,int alarmclass);

    int getVarIndex() const;
    void setVarIndex(int value);


    QString getAlarmString() const;
    void setAlarmString(const QString &value);

    int getAlamClass() const;
    void setAlamClass(int value);

private:
    int varIndex;
    int alamClass;
    QString alarmString;
};

namespace Ui {
class ParameterWidget;
}

class ParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterWidget(QWidget *parent = 0);
    ParameterWidget(FunctionStruct *f,QWidget *parent=0);
    ~ParameterWidget();

    void GetAllInformation();
    void GeneraterParameterStrings();

    void connectSignals();


private slots:
    void on_SaveButton_clicked();

    void slotEnumNumChanged(int n);

    void slotAlarmNumChanged(int n);

    void slotAlarmItemClicked(QListWidgetItem *);


    void on_alarmUpMove_clicked();

    void on_alarmDowmMove_clicked();

    void on_alarmDelete_clicked();

public slots:
    void typeChanged(int);

    void indexenable(bool);

    void slotCheckHasValueRepeat();


private:
    Ui::ParameterWidget *ui;
    FunctionStruct *functionStruct;

    QList<QLabel*> textList,alarmTextList;
    QList<QLineEdit*> valueEditList,alarmNameEditList;
    QList<QLineEdit *> disEditList,alarmCodeEditList;
    QList<QComboBox *> alarmClassCombox;
    QList<QRadioButton *> defaultRadionList;
    QIntValidator *intvalidator;
    QSpacerItem *enumSpacerItem;
    QButtonGroup buttonGroup;


    void init();

    void updateAlarmListWidget();

    void addEnumItem(enumParameters e);

    void addAlarmItem(alarmParameters a);

    cmdParameters getCmdParameters();
    numberParamters getNumParameters();
    enumParameters getEnumParameters();
    alarmParameters getAlarmParameters();
    dateParameters getDataParameters();
    timeParameters getTimeParameters();

    alarmParameters getAlarmParameters2();
};

#endif // PARAMETERWIDGET_H
