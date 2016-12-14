#ifndef FUNCTIONSTRUCT_H
#define FUNCTIONSTRUCT_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "config.h"
#include <QLabel>
#include <QList>
#include <QMap>
#include <QTime>
#include <QDate>
#include <QDateTime>

#define VARCMD 01
#define VARNUMBER 02
#define VARENUMBERATE 03
#define VARALARM 04
#define VARDATE 05
#define VARTIME 06
#define VARDATETIME 07

#define DISSTRING 1
#define DISPICTURE 2
#define DISDATE 3
#define DISTIME 4
#define DISNUMBER 5
#define DISCMD 6
#define DISENUMBERATE 7
#define DISSCROL 8

typedef struct _cmdParameters
{
    int parameter1;
    int parameter2;
    QString dis1;
    QString dis2;
    int defaultValue;
}cmdParameters;

typedef struct _numberParameters
{
    int max;
    int min;
    int scalling;
    int uint;
    int defaultValue;
    int step;
}numberParamters;

typedef struct _enumParameters
{
    QMap<int,QString> items;
//    QList<int> values;
//    QList<QString> item;

    int defaultValue;
    int itemNum;
}enumParameters;

typedef struct _alarmParameters
{
    QList<int> alarmCode;
    QList<QString> alarmName;
    QList<int> alarmClass;
}alarmParameters;

typedef struct _timeParameters
{
    QTime maxTime;
    QTime minTime;
    QTime time;
    int hourStep;
    int minuteStep;
    int secondStep;
    QString hourDis;
    QString minuteDis;
    QString secondDis;
}timeParameters;

typedef struct _dateParameters
{
    QDate maxDate;
    QDate minDate;
    QDate date;
    QString yearDis;
    QString monthDis;
    QString dayDis;
    int yearStep;
    int monthStep;
    int dayStep;
}dateParameters;

class FunctionStruct
{
public:
    FunctionStruct();
    int getVarIndex() const;
    void setVarIndex(int value);

    QString getFunctionName() const;
    void setFunctionName(const QString &value);

    int getFunctionNumber() const;
    void setFunctionNumber(int value);

    int getDirection() const;
    void setDirection(int value);

    int getVarType() const;
    void setVarType(int value);

    int getDisType() const;
    void setDisType(int value);

    QString getParameters() const;
    void setParameters(const QString &value);

    void setParameter(const cmdParameters &c);
    void setParameter(const numberParamters &n);
    void setParameter(enumParameters &e);
    void setParameter(const alarmParameters &a);
    void setParameter(const dateParameters &d);
    void setParameter(const timeParameters &t);

    cmdParameters getCmdParameters();
    numberParamters getNumberParameters();
    enumParameters getenumParameters();
    alarmParameters getAlarmParameters();
    timeParameters getTimeParameters();
    dateParameters getDateParameters();





    int getId() const;
    void setId(int value);

    int getLength() const;
    void setLength(int value);

    int getDataType() const;
    void setDataType(int value);

    int getClass2() const;
    void setClass2(int value);

    int getClass1() const;
    void setClass1(int value);

    int getDataPos() const;
    void setDataPos(int value);

    int getBitPos() const;
    void setBitPos(int value);

private:
    int id;
    int varIndex;
    QString functionName;
    int functionNumber;
    int direction;
    int varType;
    int disType;
    QString parameters;
    int length;
    int dataType;
    int class1;
    int class2;

    int dataPos;
    int bitPos;

};

#endif // FUNCTIONSTRUCT_H
