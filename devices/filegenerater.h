#ifndef FILEGENERATER_H
#define FILEGENERATER_H

#include<QList>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QXmlStreamWriter>
#include<QFile>
#include<QDebug>
#include<stdint.h>
#include<iostream>
#include<QTextStream>
#include<QTextBlock>
#include<QTextDocument>
#include<QTextTable>
#include <QtPrintSupport/QPrinter>


#include "functionstruct.h"
#include "headinformation.h"
#include "config.h"

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

#define TOSTRING(x)  (QString::number(x))
#define ROOT "DeviceDescription"    //根节点
#define HEADINFORMATION "DeviceInformation"         //设备头标记
#define DDVERSION "DDVersion"             //描述文件标准本号
#define DDFILEVER "DDFileVer"             //描述文件版本号
#define DDDEVICETYPE "DDDeviceType"       //文件使用者类型
#define DDURLLENGTH   "DDURLLength"       //URL长度
#define DDURL   "DDURL"                   //描述文件地址
#define MANUFACTURERCODE "ManufacturerCode" // 厂商编码
#define DEVICENAME "DeviceName"             //设备名称
#define DEVICETYPEID "DeviceTypeID"         //设备大小类
#define MODELID      "ModelID"              // 设备型号代码
#define MODEL      "Model"              // 设备型号代码
#define UPDATE       "Update"               //具备在线升级能力
#define SYNCHRONOUS  "Synchronous"          //具备时间同步能力

#define FUNCTIONLIST "FunctionList"         //功能列表标识
#define FUNCTION     "Function"             //功能描述标识
#define FUNCTIONNUMBER "FunctionNumber"     //功能编号
#define FUNCTIONNAME   "FunctionName"       //功能名称
#define DISPLAY        "Display"            //是否显示
#define DIRECTION      "Direction"          //权限
#define VARTYPEDESC    "VarTypeDesc"        //类型描述标识
#define VARTYPE        "VarType"            //功能类型
#define VARINDEX       "VarIndex"           //功能控制码
#define VARLENGTH      "VarLength"          //参数长度
#define DATATYPE       "DataType"           //数据类型
#define VARPARAMETERS  "VarParameters"      //类型参数标识

#define ITEM           "Item"               //
#define DEFAULTVALUE   "DefaultValue"       //

#define MAX             "Max"               //最大值
#define MIN             "Min"               //最小值
#define SCALLING        "Scalling"          //分辨率
#define UNIT            "Unit"              //单位
#define ITEMNUM         "ItemNum"           //子项数量
#define ALARMCLASS      "AlarmClass"        //报警级别
#define ALARMCODE       "AlarmCode"
#define ALARMSTRING    "AlarmString"
#define ALARMITEM       "AlarmItem"
#define YEAR            "Year"              //年
#define YEARMAX         "YearMax"           //年最大值
#define YEARMIN         "YearMin"           //年最小值
#define YEARSTEP        "YearStep"          //年步长
#define MONTH           "Month"             //y月
#define MONTHMAX        "MonthMax"          //月最大值
#define MONTHMIN        "MonthMin"          //月最小值
#define MONTHSTEP       "MonthStep"         //月步长
#define DAY             "Day"               //日
#define DAYMAX          "DayMax"            //日最大值
#define DAYMIN          "DayMin"            //日最小值
#define DAYSTEP         "DayStep"           //日步长
#define HOUR            "Hour"              //时
#define HOURMAX         "HourMax"           //时最大值
#define HOURMIN         "HourMin"           //时最小值
#define HOURSTEP        "HourStep"          //时步长
#define MINUTE          "Minute"            //分
#define MINUTEMAX       "MinuteMax"         //分最大值
#define MINUTEMIN       "MinuteMin"         //分最小值
#define MINUTESTEP      "MinuteStep"        //分步长
#define SECOND          "Second"            //分
#define SECONDMAX       "SecondMax"         //分最大值
#define SECONDMIN       "SecondMin"         //分最小值
#define SECONDSTEP      "SecondStep"        //分步长

#define DISTYPEDESC    "DisTypeDesc"       //显示描述标识
#define DISTYPE        "DisType"           //显示类型
#define DISPLAYTEXT    "DisplayText"       //显示字符串
#define DISPLAYICONINDEX "DisplayIconIndex"//显示图标编号
#define DISPARAMETERS "DisParameters"      //显示参数集合

#define DISPLAYSTRING "DisplayString"     //显示字符串
#define DISPLAYPICTUREINDEX "DisplayPictureIndex"//显示图标编号
#define DISPLAYYEAR    "DisplayYear"      //年显示
#define DISPLAYMONTH   "DisplayMonth"     //月显示
#define DISPLAYDAY      "DisplayDay"      //日显示

#define DISPLAYHOUR    "DisplayHour"     // 时显示
#define DISPLAYMINUTE  "DisplayMinute"   //分显示
#define DISPLAYSECOND  "DisplaySecond"   //秒显示
#define DISPLAYITEM    "DisplayItem"     //显示
#define DISPLAYITEMSTRING "DisplayItemString" //显示字符串
#define DISPLAYITEMPICTUREINDEX "DisplayPictureIndex" //显示图片索引

#define DISPLAYMAX     "DisplayMax"      //显示最大值
#define DISPLAYMIN     "DisplayMin"      //显示最小值
#define DISPLAYSTEP     "DisplayStep"    //显示步长
#define DISPLAYUNIT     "DisplayUnit"    //显示单位序号
#define DISPLAYDEFAULT  "DisplayDefault" //默认显示


#define DATAPOS         "DataPos"        //位置
#define BYTEPOS         "BytePos"
#define BITPOS          "BitPos"
#define DATATYPE        "DataType"       //数据类型


#define REPORTSTATE      "ReportState"  //上报状态标识
#define STATESTRUCT      "StateStruct"  //上报结构



class FileGenerater
{
public:
    FileGenerater();
    void CreateXml(QString fileName,HeadInformation *h,QList<FunctionStruct *> *l);
    void writeHeadXml(QXmlStreamWriter *w,HeadInformation *head);
    void writeFunctionXml(QXmlStreamWriter *w,QList<FunctionStruct *> *l);

    void writeVarTypeDesc(QXmlStreamWriter *w, FunctionStruct *f);
    void writeDisTypeDesc(QXmlStreamWriter *w, FunctionStruct *f);

    void CreateDocument(QString fileName,QList<FunctionStruct *> *l);

    void WriteDoc(QTextCursor *cursor, QList<FunctionStruct *> *l);

    void WriteDescribe(FunctionStruct *f);

    QString getDescribe(FunctionStruct *f);



private:
    int FunctionNumber;
    int FunctionPos;
    QList<int>BytePos;
    QList<int>BitPos;
};

#endif // FILEGENERATER_H
