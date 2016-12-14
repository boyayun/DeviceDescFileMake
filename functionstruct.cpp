#include "functionstruct.h"

FunctionStruct::FunctionStruct()
{
    this->id = 0;
}
int FunctionStruct::getVarIndex() const
{
    return varIndex;
}

void FunctionStruct::setVarIndex(int value)
{
    varIndex = value;
}
QString FunctionStruct::getFunctionName() const
{
    return functionName;
}

void FunctionStruct::setFunctionName(const QString &value)
{
    functionName = value;
}
int FunctionStruct::getFunctionNumber() const
{
    return functionNumber;
}

void FunctionStruct::setFunctionNumber(int value)
{
    functionNumber = value;
}
int FunctionStruct::getDirection() const
{
    return direction;
}

void FunctionStruct::setDirection(int value)
{
    if(this->varType == VARNUMBER)
    {
        if(direction == 3)
        {
            this->setDisType(DISSCROL);
        }
        else
        {
            this->setDisType(DISNUMBER);
        }
    }
    direction = value;
}
int FunctionStruct::getVarType() const
{
    return varType;
}

void FunctionStruct::setVarType(int value)
{
    if(value == VARCMD)
    {
        setDataType(UINT8_T);
        setLength(1);
        setDisType(DISCMD);
    }
    else if(value == VARNUMBER)
    {
        setLength(2);
        setDataType(INT16_T);
        if(this->direction == 3)
        {
            setDisType(DISSCROL);
        }
        else
        {
            setDisType(DISNUMBER);
        }

    }
    else if(value == VARENUMBERATE)
    {
        setLength(1);
        setDataType(UINT8_T);
        setDisType(DISENUMBERATE);
    }
    else if(value == VARDATE)
    {
        setLength(4);
        setDataType(UINT8_T);
        setDisType(DISDATE);
    }
    else if(value == VARDATETIME)
    {
        setLength(7);
    }
    else if(value == VARALARM)
    {
        setLength(0);
        setDataType(UINT8_T);
        setDisType(DISSTRING);
    }
    else if(value == VARTIME)
    {
        setLength(3);
        setDataType(UINT8_T);
        setDisType(DISTIME);
    }
    else
    {

    }

    varType = value;
}
int FunctionStruct::getDisType() const
{
    return disType;
}

void FunctionStruct::setDisType(int value)
{
    disType = value;
}
QString FunctionStruct::getParameters() const
{
    return parameters;
}

void FunctionStruct::setParameters(const QString &value)
{
    parameters = value;
}

void FunctionStruct::setParameter(const cmdParameters &c)
{
    setLength(1);
    QJsonObject jsonObj;
    jsonObj.insert("on",c.parameter1);
    jsonObj.insert("off",c.parameter2);
    jsonObj.insert("defaultValue",c.defaultValue);
    jsonObj.insert("disoff",c.dis2);
    jsonObj.insert("dison",c.dis1);
    QJsonDocument d;
    d.setObject(jsonObj);
    QString str(d.toJson());
    setParameters(str);
    qDebug()<<str;


}

void FunctionStruct::setParameter(const numberParamters &n)
{
    setLength(2);
    QJsonObject jsonObj;
    jsonObj.insert("Max",n.max);
    jsonObj.insert("Min",n.min);
    jsonObj.insert("DefaultValue",n.defaultValue);
    jsonObj.insert("disScaling",n.scalling);
    jsonObj.insert("unit",n.uint);
    jsonObj.insert("Step",n.step);
    QJsonDocument d;
    d.setObject(jsonObj);
    QString str(d.toJson());
    setParameters(str);
    qDebug()<<str;

}

void FunctionStruct::setParameter(enumParameters &e)
{
    setLength(1);
    QJsonObject jsonObj;
    jsonObj.insert("ParameterNum",e.itemNum);
    jsonObj.insert("DefaultValues",e.defaultValue);
    QJsonArray valueArray,disArray;

    QMap<int,QString>::iterator it ;
    for(it = e.items.begin();it!=e.items.end();it++)
    {
        valueArray.append(it.key());
        disArray.append(it.value());
    }
//    for(int i=0;i<e.itemNum;i++)
//    {
//       valueArray.append(e.values.at(i));
//       disArray.append(e.item.at(i));
//    }
    jsonObj.insert("ParameterItems",disArray);
    jsonObj.insert("ParameterValues",valueArray);
    QJsonDocument d;
    d.setObject(jsonObj);
    QString str(d.toJson());
    setParameters(str);
    qDebug()<<str;
}

void FunctionStruct::setParameter(const alarmParameters &a)
{
    setLength(1);
    QJsonObject jsonObj;
    QJsonArray jsonArray;
    for(int i=0;i<a.alarmClass.size();i++)
    {
        QJsonObject o;
        o.insert("alarmclass",a.alarmClass.at(i));
        o.insert("alarmcode",a.alarmCode.at(i));
        o.insert("alarmstring",a.alarmName.at(i));
        jsonArray.append(o);
    }
    jsonObj.insert("alarmItem",jsonArray);
    QJsonDocument d;
    d.setObject(jsonObj);
    QString str(d.toJson());
    setParameters(str);
    qDebug()<<str;

}

void FunctionStruct::setParameter(const dateParameters &d)
{
    setLength(4);
    qDebug()<<d.date.toString("yyyy-MM-dd");

    QJsonObject jsonObj;
    jsonObj.insert("disYear",d.yearDis);
    jsonObj.insert("disMonth",d.monthDis);
    jsonObj.insert("disDay",d.dayDis);
    jsonObj.insert("maxDate",d.maxDate.toString("yyyy-MM-dd"));
    jsonObj.insert("minDate",d.minDate.toString("yyyy-MM-dd"));
    jsonObj.insert("date",d.date.toString("yyyy-MM-dd"));
    jsonObj.insert("yearStep",d.yearStep);
    jsonObj.insert("monthStep",d.monthStep);
    jsonObj.insert("dayStep",d.dayStep);

    QJsonDocument doc;
    doc.setObject(jsonObj);
    QString str(doc.toJson());
    setParameters(str);


}

void FunctionStruct::setParameter(const timeParameters &t)
{
    setLength(3);
    qDebug()<<t.maxTime.toString("h:m:s");
    QJsonObject jsonObj;
    jsonObj.insert("disHour",t.hourDis);
    jsonObj.insert("disMinute",t.minuteDis);
    jsonObj.insert("disSecond",t.secondDis);
    jsonObj.insert("minTime",t.minTime.toString("h:m:s"));
    jsonObj.insert("maxTime",t.maxTime.toString("h:m:s"));
    jsonObj.insert("time",t.time.toString("h:m:s"));
    jsonObj.insert("hourStep",t.hourStep);
    jsonObj.insert("minuteStep",t.minuteStep);
    jsonObj.insert("secondStep",t.secondStep);
    QJsonDocument doc;
    doc.setObject(jsonObj);
    QString str(doc.toJson());
    setParameters(str);

}

cmdParameters FunctionStruct::getCmdParameters()
{
    QString jsonStr = parameters;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if(jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        PRINTLOG(jsonObj.value("disoff").toString());
        PRINTLOG(jsonObj.value("dison").toString());
        PRINTLOG(jsonObj.value("off").toInt());
        PRINTLOG(jsonObj.value("on").toInt());
        PRINTLOG(jsonObj.value("defaultValue").toInt());
        cmdParameters cmdparameter;
        cmdparameter.defaultValue = jsonObj.value("defaultValue").toInt();
        cmdparameter.dis1 = jsonObj.value("dison").toString();
        cmdparameter.dis2 = jsonObj.value("disoff").toString();
        cmdparameter.parameter1 = jsonObj.value("on").toInt();
        cmdparameter.parameter2 = jsonObj.value("off").toInt();
        return cmdparameter;
    }
    else
    {
        PRINTLOG("ERROR!");
        cmdParameters c;
        c.dis1="";
        c.dis2="";
        c.parameter1=1;
        c.parameter2=0;
        c.defaultValue = 0;
        return c;
    }
}

numberParamters FunctionStruct::getNumberParameters()
{
    QString jsonStr = parameters;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
    numberParamters np;
    if(jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        PRINTLOG(jsonObj.value("Max").toInt());
        PRINTLOG(jsonObj.value("Min").toInt());
        PRINTLOG(jsonObj.value("DefaultValue").toInt());
        PRINTLOG(jsonObj.value("Step").toInt());
        PRINTLOG(jsonObj.value("disScaling").toInt());
        PRINTLOG(jsonObj.value("unit").toInt());

        np.max = jsonObj.value("Max").toInt();
        np.min = jsonObj.value("Min").toInt();
        np.defaultValue = jsonObj.value("DefaultValue").toInt();
        np.scalling = jsonObj.value("disScaling").toInt();
        np.step = jsonObj.value("Step").toInt();
        np.uint = jsonObj.value("unit").toInt();
    }
    else
    {
        PRINTLOG("ERROR!");
    }
    return np;
}

enumParameters FunctionStruct::getenumParameters()
{
    QString jsonStr = parameters;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
    enumParameters ep;
    if(jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        PRINTLOG(jsonObj.value("ParameterNum").toInt());
        ep.defaultValue  = jsonObj.value("DefaultValues").toInt();
        PRINTLOG(jsonObj.value("DefaultValue").toInt());
        ep.itemNum = jsonObj.value("ParameterNum").toInt();
        QJsonArray valueArray = jsonObj.value("ParameterValues").toArray();
        QJsonArray itemArray = jsonObj.value("ParameterItems").toArray();
        for(int i=0;i<valueArray.size();i++)
        {

            ep.items.insert(valueArray.at(i).toInt(),itemArray.at(i).toString());
//            ep.values.append(valueArray.at(i).toInt());
//            ep.item.append(itemArray.at(i).toString());
        }
    }
    else
    {
        PRINTLOG("ERROR!");
    }
    return ep;


}

alarmParameters FunctionStruct::getAlarmParameters()
{
    QString jsonStr = parameters;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
    alarmParameters ap;
    if(jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray itemArray = jsonObj.value("alarmItem").toArray();
        for(int i=0;i<itemArray.size();i++)
        {
            QJsonObject itemObj = itemArray.at(i).toObject();
            ap.alarmClass.append(itemObj.value("alarmclass").toInt());
            ap.alarmName.append(itemObj.value("alarmstring").toString());
            ap.alarmCode.append(itemObj.value("alarmcode").toInt());
        }
    }
    else
    {
        PRINTLOG("ERROR!");
    }
    return ap;
}

timeParameters FunctionStruct::getTimeParameters()
{
    timeParameters tp;
    QString jsonStr = parameters;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if(jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        tp.maxTime = QTime::fromString(jsonObj.value("maxTime").toString(),"h:m:s");
        tp.minTime = QTime::fromString(jsonObj.value("minTime").toString(),"h:m:s");
        tp.time = QTime::fromString(jsonObj.value("time").toString(),"h:m:s");
        tp.hourDis = jsonObj.value("disHour").toString();
        tp.minuteDis=jsonObj.value("disMinute").toString();
        tp.secondDis =jsonObj.value("disSecond").toString();
        tp.hourStep = jsonObj.value("hourStep").toInt();
        tp.minuteStep= jsonObj.value("minuteStep").toInt();
        tp.secondStep = jsonObj.value("secondStep").toInt();
    }
    else
    {
        PRINTLOG("ERROR!");
    }

    return tp;


}

dateParameters FunctionStruct::getDateParameters()
{
    dateParameters dp;
    QString jsonStr = parameters;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if(jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        dp.maxDate = QDate::fromString(jsonObj.value("maxDate").toString(),"yyyy-MM-dd");
        dp.minDate = QDate::fromString(jsonObj.value("minDate").toString(),"yyyy-MM-dd");
        dp.date = QDate::fromString(jsonObj.value("date").toString(),"yyyy-MM-dd");
        dp.yearStep = jsonObj.value("yearStep").toInt();
        dp.monthStep = jsonObj.value("monthStep").toInt();
        dp.dayStep = jsonObj.value("dayStep").toInt();
        dp.yearDis = jsonObj.value("disYear").toString();
        dp.monthDis = jsonObj.value("disMonth").toString();
        dp.dayDis = jsonObj.value("disDay").toString();
    }
    else
    {
        PRINTLOG("ERROR!");
    }
    QDate d = QDate::fromString("1989-12-12","yyyy-MM-dd");
    qDebug()<<d;
    qDebug()<<d.toString("yyyyMMdd");

    return dp;

}
int FunctionStruct::getId() const
{
    return id;
}

void FunctionStruct::setId(int value)
{
    id = value;
}
int FunctionStruct::getLength() const
{
    return length;
}

void FunctionStruct::setLength(int value)
{
    length = value;
}
int FunctionStruct::getDataType() const
{
    return dataType;
}

void FunctionStruct::setDataType(int value)
{
    dataType = value;
}
int FunctionStruct::getClass2() const
{
    return class2;
}

void FunctionStruct::setClass2(int value)
{
    class2 = value;
}
int FunctionStruct::getClass1() const
{
    return class1;
}

void FunctionStruct::setClass1(int value)
{
    class1 = value;
}

int FunctionStruct::getDataPos() const
{
    return dataPos;
}

void FunctionStruct::setDataPos(int value)
{
    dataPos = value;
}

int FunctionStruct::getBitPos() const
{
    return bitPos;
}

void FunctionStruct::setBitPos(int value)
{
    bitPos = value;
}













