#include "generatedescriptionfile.h"


GenerateDescriptionFile::GenerateDescriptionFile()
{
    FunctionNumber = 1;
    this->FunctionPos = Pos;
    qDebug()<<__FILE__<<Pos;



    // Pos  = 1;
}

GenerateDescriptionFile::GenerateDescriptionFile(int pos)
{
    FunctionNumber =0;
    this->FunctionPos = pos;
    pos = 0;
    qDebug()<<__FILE__<<FunctionPos;
}

void GenerateDescriptionFile::CreateXml(QString fileName, DeviceDescribeHead *h, QList<FunctionItem *> *l)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"OPEN SUCESS";
        qDebug()<<this->FunctionNumber;

        QXmlStreamWriter writer(&file);

        writer.writeStartDocument();
        writer.writeStartElement(ROOT);
        writeHeadXml(&writer,h); // 写头部
        writeFunctionXml(&writer,l); //写Function
//#ifdef WRITE_STATUS_LAST
//        writeReportStateXml(&writer,l);
//#endif
        writer.writeEndElement();//根目录
        writer.writeEndDocument();
    }
    QFile fileFormat(fileName+".txt");
    if(fileFormat.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Open fileformat success";
        QTextStream w(&fileFormat);
        int pos = 0;
        w.setFieldAlignment(QTextStream::AlignLeft);
        w<<qSetFieldWidth(20)<<"FunctionName";
        w<<qSetFieldWidth(8)<<"Data";
        w<<qSetFieldWidth(3)<<"Pos";
        w<<qSetFieldWidth(1)<<endl;
        for(int i=0;i<l->size();i++)
        {

            w.setFieldAlignment(QTextStream::AlignLeft);
            w.setFieldWidth(20-l->at(i)->getDisplayName().length());

            w<<l->at(i)->getDisplayName();
            w<<qSetFieldWidth(8)<<l->at(i)->getLength();
            w<<qSetFieldWidth(8)<<pos;
            w<<qSetFieldWidth(1)<<endl;
            if(l->at(i)->getVarType()!=VARALARM)
            {
                pos += l->at(i)->getLength();
            }
        }
    }

}

void GenerateDescriptionFile::CreateJson(DeviceDescribeHead *h, QList<FunctionItem *> *l)
{
   QJsonObject jsonObject;
    writeHeadJson(h,&jsonObject);
    writeFunctionJson(l,&jsonObject);
    QJsonDocument doc;
    doc.setObject(jsonObject);
    QFile f("test.json");
   f.open(QIODevice::WriteOnly | QIODevice::Text);
    f.write(doc.toJson());
    f.close();

}

void GenerateDescriptionFile::writeHeadXml(QXmlStreamWriter *w, DeviceDescribeHead *head)
{
    w->writeStartElement(HEADINFORMATION);
    w->writeTextElement(DDVERSION,"02");    //v0.2
    w->writeTextElement(DDFILEVER,"01");
    w->writeTextElement(DDDEVICETYPE,"01"); //使用者
    w->writeTextElement(DDURLLENGTH,TOSTRING(head->getURL().length()));
    w->writeTextElement(DDURL,head->getURL());
    w->writeTextElement(MANUFACTURERCODE,head->getManufatureID());
    w->writeTextElement(DEVICENAME,head->getDeviceName());
    w->writeTextElement(DEVICETYPEID,"0"+QString::number(head->getDeviceTypeID1())+"0"+QString::number(head->getDeviceTypeID2()));
    w->writeTextElement(MODELID,QString::number(head->getDeviceModel()));
    w->writeTextElement(UPDATE,QString::number(head->getUpdate()));
    w->writeTextElement(SYNCHRONOUS,QString::number(head->getSynchronous()));
    w->writeEndElement();
}

void GenerateDescriptionFile::writeFunctionXml(QXmlStreamWriter *w, QList<FunctionItem *> *l)
{
    w->writeStartElement(FUNCTIONLIST);
    for(int i=0;i<l->size();i++)
    {
        if(i==0)
        {
            if(l->at(0)->getVarIndex()==31)
            {
                this->FunctionNumber = 0;
            }
            else
            {
                this->FunctionNumber  = 1;
            }
        }
        w->writeStartElement(FUNCTION);
        w->writeTextElement(FUNCTIONNUMBER,QString::number(FunctionNumber));
        l->at(i)->setFunctionNumber(FunctionNumber);
        w->writeTextElement(FUNCTIONNAME,l->at(i)->getDisplayName());
        w->writeTextElement(DISPLAY,QString::number(l->at(i)->getDisplay()));
        w->writeTextElement(DIRECTION,QString::number(l->at(i)->getDirection()));
        if(l->at(i)->getVarType()!=VARALARM)   //警告型没有pos
        {
          w->writeTextElement(DATAPOS,TOSTRING(this->FunctionPos));
        }


        this->writeVarTypeDesc(w,l->at(i));
        if(l->at(i)->getVarType()!=VARALARM)  //警告信无显示描述
        {

            this->writeDisTypeDesc(w,l->at(i));
        }
            FunctionNumber++;

//#ifndef WRITE_STATUS_LAST
//        writeStateStruct(w,l->at(i));
//#endif
        w->writeEndElement();

    }
    w->writeEndElement();
}

void GenerateDescriptionFile::writeParameterXml(QXmlStreamWriter *w, FunctionItem *f)
{
    w->writeStartElement("parameter");
    int vartype = f->getVarType();
    int distype = f->getDisplayType();
    QJsonParseError jsonError;
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;
    jsonDocument = QJsonDocument::fromJson(f->getParameterStrJson().toUtf8(),&jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
      //  qDebug()<<"YES";
    }
    else
    {
        qDebug()<<"参数有误"<<__FUNCTION__<<__LINE__;
    //    return;
    }
    if(jsonDocument.isObject())
    {
        jsonObject = jsonDocument.object();
    }
    else
    {
        qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<"参数有误";
    //    return;
    }

    switch (vartype) {
    case VARCMD:
        if(distype == DISCMD)
        {
        w->writeTextElement("ParameterOnValue",QString::number(jsonObject.value("on").toInt(),16)+"H");
        w->writeTextElement("ParameterOnDisplayname",jsonObject.value("dison").toString());
        w->writeTextElement("ParameterOffValue",TOSTRING(jsonObject.value("off").toInt()));
        w->writeTextElement("ParameterOnDisplayname",jsonObject.value("disoff").toString());
        }
        else if(distype == DISPICTURE)
        {
        w->writeTextElement("ParameterOnValue",QString::number(jsonObject.value("on").toInt(),16)+"H");
        w->writeTextElement("ParameterOffValue",TOSTRING(jsonObject.value("off").toInt()));
        w->writeTextElement("ParameterPicture",TOSTRING(jsonObject.value("dispictureNumber").toInt()));
        }
        break;
    case VARNUMBER:
        w->writeTextElement("Max",TOSTRING(jsonObject.value("Max").toInt()));
        w->writeTextElement("Min",TOSTRING(jsonObject.value("Min").toInt()));
        w->writeTextElement("Step",TOSTRING(jsonObject.value("Step").toInt()));
        if(distype == DISNUMBER)
        {

            w->writeTextElement("Scaling",TOSTRING(jsonObject.value("disScaling").toInt()));
        }
        else if(distype == DISSCROL)
        {
            w->writeTextElement("DisMax",TOSTRING(jsonObject.value("disMax").toInt()));
            w->writeTextElement("DisMin",jsonObject.value("disMin").toString());
            w->writeTextElement("DisScaling",jsonObject.value("disScaling").toString());
            w->writeTextElement("DisDefault",jsonObject.value("disDefault").toString());
            w->writeTextElement("DisUnit",TOSTRING(jsonObject.value("disUnit").toInt()));
        }
        else if(distype == DISSTRING)
        {
            w->writeTextElement("DisString",jsonObject.value("disString").toString());
        }
        break;
    case VARENUMBERATE:
    {
        w->writeTextElement("ParameterNum",TOSTRING(jsonObject.value("ParameterNum").toInt()));
        QJsonArray jsonArray =  jsonObject.value("ParameterValues").toArray();
        for(int i = 0; i<jsonArray.size();i++)
        {
            w->writeTextElement("Parameter"+QString::number(i),TOSTRING(jsonArray.at(i).toInt()));
        }

        //显示类型为枚举型
        QJsonArray jsonArraydis  = jsonObject.value("ParameterItems").toArray();
        for(int i=0;i<jsonArraydis.size();i++)
        {
            w->writeTextElement("Display_item"+QString::number(i),jsonArraydis.at(i).toString());
        }
    }
        break;
    case VARALARM:
    {
       w->writeTextElement("AlarmClass",TOSTRING(jsonObject.value("alarmclass").toInt()));
       if(distype == DISSTRING)
       {
            w->writeTextElement("DisString",jsonObject.value("disString").toString());
       }
       else if(distype == DISPICTURE)
       {
        w->writeTextElement("ParameterPicture",TOSTRING(jsonObject.value("dispictureNumber").toInt()));
       }
    }
        break;
    case VARDATE:
    {
        w->writeTextElement("YearDisplay",jsonObject.value("disYear").toString());
       w->writeTextElement("YearMax",TOSTRING(jsonObject.value("yearmax").toInt()));
       w->writeTextElement("YearMin",TOSTRING(jsonObject.value("yearmin").toInt()));
       w->writeTextElement("Year",TOSTRING(jsonObject.value("year").toInt()));
       w->writeTextElement("YearStep",TOSTRING(jsonObject.value("yearstep").toInt()));

        w->writeTextElement("MonthDisplay",jsonObject.value("disMonth").toString());
       w->writeTextElement("MonthMax",TOSTRING(jsonObject.value("monthmax").toInt()));
       w->writeTextElement("MonthMin",TOSTRING(jsonObject.value("monthmin").toInt()));
       w->writeTextElement("Month",TOSTRING(jsonObject.value("month").toInt()));
       w->writeTextElement("MonthStep",TOSTRING(jsonObject.value("monthstep").toInt()));

        w->writeTextElement("DayDisplay",jsonObject.value("disDay").toString());
       w->writeTextElement("DayMax",TOSTRING(jsonObject.value("daymax").toInt()));
       w->writeTextElement("DayMin",TOSTRING(jsonObject.value("daymin").toInt()));
       w->writeTextElement("Day",TOSTRING(jsonObject.value("day").toInt()));
       w->writeTextElement("DayStep",TOSTRING(jsonObject.value("daystep").toInt()));


    }
     break;
    case VARTIME:
    {
        w->writeTextElement("HourDisplay",jsonObject.value("disHour").toString());
       w->writeTextElement("HourMax",TOSTRING(jsonObject.value("hourmax").toInt()));
       w->writeTextElement("HourMin",TOSTRING(jsonObject.value("hourmin").toInt()));
       w->writeTextElement("Hour",TOSTRING(jsonObject.value("hour").toInt()));
       w->writeTextElement("HourStep",TOSTRING(jsonObject.value("hourstep").toInt()));

        w->writeTextElement("MinuteDisplay",jsonObject.value("disMinute").toString());
       w->writeTextElement("MinuteMax",TOSTRING(jsonObject.value("minutemax").toInt()));
       w->writeTextElement("MinuteMin",TOSTRING(jsonObject.value("minutemin").toInt()));
       w->writeTextElement("Minute",TOSTRING(jsonObject.value("minute").toInt()));
       w->writeTextElement("MinuteStep",TOSTRING(jsonObject.value("minutestep").toInt()));

        w->writeTextElement("SecondDisplay",jsonObject.value("disSecond").toString());
       w->writeTextElement("SecondMax",TOSTRING(jsonObject.value("secondmax").toInt()));
       w->writeTextElement("SecondMin",TOSTRING(jsonObject.value("secondmin").toInt()));
       w->writeTextElement("Second",TOSTRING(jsonObject.value("second").toInt()));
       w->writeTextElement("SecondStep",TOSTRING(jsonObject.value("secondstep").toInt()));

    }
         break;


    default:
        break;
    }

    w->writeEndElement();

}

void GenerateDescriptionFile::writeHeadJson(DeviceDescribeHead *h, QJsonObject *j)
{
   QJsonObject obj;
   obj.insert("DD_Encode","02H");
   obj.insert("DD_Devicetype","01H");
   obj.insert("Manufacturer_Code",h->getManufatureID());
   obj.insert("DD_FileVer","01H");
   obj.insert("Device_TypeID",h->getDeviceTypeID1());
   obj.insert("Device_TypeID1",h->getDeviceTypeID2());
   obj.insert("Model_ID",h->getDeviceModel());
   obj.insert("D_URL_Length",h->getURL().length());
   obj.insert("DD_URL",h->getURL());
   obj.insert("Update",h->getUpdate());
   obj.insert("Synchronous",h->getSynchronous());
   j->insert("dev_info",obj);
}

void GenerateDescriptionFile::writeFunctionJson(QList<FunctionItem *> *l, QJsonObject *j)
{
    QJsonArray jsonArray;
    for(int i=0;i<l->size();i++)
    {
        QJsonObject obj;
        obj.insert("VarIndex",(int)l->at(i)->getVarIndex());
        obj.insert("Displayname",l->at(i)->getDisplayName());
        obj.insert("Direction",(l->at(i)->getDirection()));
        obj.insert("Display",(int)l->at(i)->getDisplay());
        obj.insert("VarType",(int)l->at(i)->getVarType());
        obj.insert("Displaytype",(int)l->at(i)->getDisplayType());
        writeParameterJson(l->at(i),&obj);
        jsonArray.append(obj);
    }

    j->insert("function_list",jsonArray);

}

void GenerateDescriptionFile::writeParameterJson(FunctionItem *f, QJsonObject *j)
{
   QJsonObject obj;
   int vartype = f->getVarType();
   int distype = f->getDisplayType();
   QJsonParseError jsonError;
   QJsonDocument jsonDocument;
   QJsonObject jsonObject;
   jsonDocument = QJsonDocument::fromJson(f->getParameterStrJson().toUtf8(),&jsonError);
   if(jsonError.error == QJsonParseError::NoError)
   {
     //  qDebug()<<"YES";
   }
   else
   {
       qDebug()<<"参数有误"<<__FUNCTION__<<__LINE__;
       return;
   }
   if(jsonDocument.isObject())
   {
       jsonObject = jsonDocument.object();
   }
   else
   {
       qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<"参数有误";
   //    return;
   }
   switch (vartype) {
   case VARCMD:
   {
       if(distype == DISCMD)
       {
           obj.insert("ParameterOnValue",jsonObject.value("on").toInt());
           obj.insert("ParameterOnDisplayname",jsonObject.value("dison").toString());
           obj.insert("ParameterOffValue",jsonObject.value("off").toInt());
           obj.insert("ParameterOffDisplayname",jsonObject.value("disoff").toString());
       }
       else if(distype == DISPICTURE)
       {
       obj.insert("ParameterOnValue",jsonObject.value("on").toInt());
       obj.insert("ParameterOffValue",jsonObject.value("off").toInt());
       obj.insert("ParameterPicture",jsonObject.value("dispictureNumber").toInt());
       }
   }
       break;
   case VARNUMBER:
       obj.insert("Max",(jsonObject.value("Max").toInt()));
       obj.insert("Min",(jsonObject.value("Min").toInt()));
       obj.insert("Step",(jsonObject.value("Step").toInt()));
       if(distype == DISNUMBER)
       {

            obj.insert("Scaling",(jsonObject.value("disScaling").toInt()));
       }
       else if(distype == DISSCROL)
       {
           obj.insert("DisMax",jsonObject.value("disMax").toString());
           obj.insert("DisMin",jsonObject.value("disMin").toString());
           obj.insert("DisScaling",jsonObject.value("disScaling").toString());
           obj.insert("DisDefault",jsonObject.value("disDefault").toString());
           obj.insert("DisUnit",(jsonObject.value("disUnit").toInt()));
       }
       else if(distype == DISSTRING)
       {
            obj.insert("DisString",jsonObject.value("disString").toString());
       }
       break;
   case VARENUMBERATE:
   {
       obj.insert("ParameterNum",jsonObject.value("ParameterNum").toInt());
       QJsonArray jsonArray =  jsonObject.value("ParameterValues").toArray();

       obj.insert("ParameterValues",jsonArray);

       obj.insert("ParameterItems",jsonObject.value("ParameterItems").toArray());

   }
       break;
   case VARALARM:
   {
      obj.insert("AlarmClass",(jsonObject.value("alarmclass").toInt()));
      if(distype == DISSTRING)
      {
           obj.insert("DisString",jsonObject.value("disString").toString());
      }
      else if(distype == DISPICTURE)
      {
       obj.insert("ParameterPicture",(jsonObject.value("dispictureNumber").toInt()));
      }
   }
       break;
   case VARDATE:
   {
        obj.insert("YearDisplay",jsonObject.value("disYear").toString());
      obj.insert("YearMax",(jsonObject.value("yearmax").toInt()));
      obj.insert("YearMin",(jsonObject.value("yearmin").toInt()));
       obj.insert("Year",(jsonObject.value("year").toInt()));
      obj.insert("YearStep",(jsonObject.value("yearstep").toInt()));

       obj.insert("MonthDisplay",jsonObject.value("disMonth").toString());
      obj.insert("MonthMax",(jsonObject.value("monthmax").toInt()));
      obj.insert("MonthMin",(jsonObject.value("monthmin").toInt()));
      obj.insert("Month",(jsonObject.value("month").toInt()));
      obj.insert("MonthStep",(jsonObject.value("monthstep").toInt()));

       obj.insert("DayDisplay",jsonObject.value("disDay").toString());
      obj.insert("DayMax",(jsonObject.value("daymax").toInt()));
      obj.insert("DayMin",(jsonObject.value("daymin").toInt()));
      obj.insert("Day",(jsonObject.value("day").toInt()));
      obj.insert("DayStep",(jsonObject.value("daystep").toInt()));


   }
    break;
   case VARTIME:
   {
       obj.insert("HourDisplay",jsonObject.value("disHour").toString());
      obj.insert("HourMax",(jsonObject.value("hourmax").toInt()));
      obj.insert("HourMin",(jsonObject.value("hourmin").toInt()));
      obj.insert("Hour",(jsonObject.value("hour").toInt()));
      obj.insert("HourStep",(jsonObject.value("hourstep").toInt()));

       obj.insert("MinuteDisplay",jsonObject.value("disMinute").toString());
      obj.insert("MinuteMax",(jsonObject.value("minutemax").toInt()));
      obj.insert("MinuteMin",(jsonObject.value("minutemin").toInt()));
      obj.insert("Minute",(jsonObject.value("minute").toInt()));
      obj.insert("MinuteStep",(jsonObject.value("minutestep").toInt()));

       obj.insert("SecondDisplay",jsonObject.value("disSecond").toString());
      obj.insert("SecondMax",(jsonObject.value("secondmax").toInt()));
      obj.insert("SecondMin",(jsonObject.value("secondmin").toInt()));
      obj.insert("Second",(jsonObject.value("second").toInt()));
      obj.insert("SecondStep",(jsonObject.value("secondstep").toInt()));

   }
        break;
   default:
       break;
   }
   j->insert(QString("parameter"),obj);
}

void GenerateDescriptionFile::writeReportStateXml(QXmlStreamWriter *w, QList<FunctionItem *> *l)
{
    w->writeStartElement(REPORTSTATE);
    for(int i=0;i<l->size();i++)
    {
        w->writeStartElement(STATESTRUCT);
        w->writeTextElement(FUNCTIONNUMBER,TOSTRING(l->at(i)->getFunctionNumber()));
        w->writeTextElement(DATAPOS,TOSTRING(0));
        w->writeTextElement(DATATYPE,TOSTRING(l->at(i)->getDataType()));
        w->writeEndElement();
    }

    w->writeEndElement();

}

void GenerateDescriptionFile::writeHeadXmlStand(QXmlStreamWriter *w, DeviceDescribeHead *head)
{
    w->writeStartElement("dev_info");
    w->writeAttribute("DD_Encode","02H");
    w->writeAttribute("DD_Devicetype","01H");
    w->writeTextElement("Manufacturer_Code",head->getManufatureID());
    w->writeTextElement("DD_FileVer","01H");
    w->writeTextElement("Device_TypeID","0"+QString::number(head->getDeviceTypeID1())+"0"+QString::number(head->getDeviceTypeID2())+"H");
    w->writeTextElement("Model_ID",QString::number(head->getDeviceModel()));
    w->writeTextElement("D_URL_Length",QString::number(head->getURL().length(),16)+"H");
    w->writeTextElement("DD_URL",head->getURL());
    w->writeTextElement("Update",QString::number(head->getUpdate(),16)+"H");
    w->writeTextElement("Synchronous",QString::number(head->getSynchronous(),16)+"H");
    w->writeEndElement();

}

void GenerateDescriptionFile::writeVarTypeDesc(QXmlStreamWriter *w, FunctionItem *f)
{
    w->writeStartElement(VARTYPEDESC);
    int vartype = f->getVarType();
    w->writeTextElement(VARTYPE,QString::number(vartype));
    w->writeTextElement(VARINDEX,TOSTRING(f->getVarIndex()));
   // w->writeTextElement("VarPos",QString::number(FunctionPos));


    if(vartype != VARALARM)
    {
    w->writeTextElement(VARLENGTH,QString::number(f->getLength()));
    w->writeTextElement(DATATYPE,TOSTRING(f->getDataType()));
    FunctionPos += f->getLength();
    }

    //w->writeTextElement("DefaultValue",QString::number(f->getDefaultValue()));
    w->writeStartElement(VARPARAMETERS);
    QJsonParseError jsonError;
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;
    jsonDocument = QJsonDocument::fromJson(f->getParameterStrJson().toUtf8(),&jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
      //  qDebug()<<"YES";
    }
    else
    {
        qDebug()<<"参数有误"<<__FUNCTION__<<__LINE__;
        w->writeEndElement();
        w->writeEndElement();
        return;
    }
    if(jsonDocument.isObject())
    {
        jsonObject = jsonDocument.object();
    }
    else
    {
        qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<"参数有误";
        w->writeEndElement();
        return;
    }

    switch (vartype) {
    case VARCMD:
        w->writeTextElement(ITEM,TOSTRING(jsonObject.value("on").toInt()));
        w->writeTextElement(ITEM,TOSTRING(jsonObject.value("off").toInt()));
        w->writeTextElement(DEFAULTVALUE,TOSTRING(jsonObject.value("defaultValue").toInt()));
        break;
    case VARNUMBER:
        w->writeTextElement(MAX,TOSTRING(jsonObject.value("Max").toInt()));
        w->writeTextElement(MIN,TOSTRING(jsonObject.value("Min").toInt()));
        w->writeTextElement(SCALLING,TOSTRING(jsonObject.value("Step").toInt()));
        w->writeTextElement(UNIT,TOSTRING(jsonObject.value("unit").toInt()));
        w->writeTextElement(DEFAULTVALUE,TOSTRING(jsonObject.value("defaultValue").toInt()));
        break;
    case VARENUMBERATE:
    {
        w->writeTextElement(ITEMNUM,TOSTRING(jsonObject.value("ParameterNum").toInt()));
        QJsonArray jsonArray =  jsonObject.value("ParameterValues").toArray();
        for(int i = 0; i<jsonArray.size();i++)
        {
            w->writeTextElement(ITEM,TOSTRING(jsonArray.at(i).toInt()));
        }
       // w->writeTextElement(DEFAULTVALUE,TOSTRING(jsonObject.value("defaultValue").toInt()));
        w->writeTextElement(DEFAULTVALUE,TOSTRING(jsonArray.at(0).toInt()));
        //显示类型为枚举型
    }
        break;
    case VARALARM:
    {
        QJsonArray jsonArray = jsonObject.value("alarmItem").toArray();
        for(int i=0;i<jsonArray.size();i++)
        {
            w->writeStartElement(ALARMITEM);
           QJsonObject o = jsonArray.at(i).toObject();
           w->writeTextElement(ALARMCODE,TOSTRING(o.value("alarmcode").toInt()));
           w->writeTextElement(ALARMCLASS,TOSTRING(o.value("alarmclass").toInt()));
           w->writeTextElement(ALARMSTRING,o.value("alarmstring").toString());
            w->writeEndElement();


//           s.alarmClass.append(o.value("alarmclass").toInt());
//           s.alarmCode.append(o.value("alarmcode").toInt());
//           s.alarmString.append(o.value("alarmstring").toString());
        }
      // w->writeTextElement(ALARMCLASS,TOSTRING(jsonObject.value("alarmclass").toInt()));
    }

        break;
    case VARDATE:
    {
       w->writeTextElement(YEARMAX,TOSTRING(jsonObject.value("yearmax").toInt()));
       w->writeTextElement(YEARMIN,TOSTRING(jsonObject.value("yearmin").toInt()));
       w->writeTextElement(YEAR,TOSTRING(jsonObject.value("year").toInt()));
       w->writeTextElement(YEARSTEP,TOSTRING(jsonObject.value("yearstep").toInt()));

       w->writeTextElement(MONTHMAX,TOSTRING(jsonObject.value("monthmax").toInt()));
       w->writeTextElement(MONTHMIN,TOSTRING(jsonObject.value("monthmin").toInt()));
       w->writeTextElement(MONTH,TOSTRING(jsonObject.value("month").toInt()));
       w->writeTextElement(MONTHSTEP,TOSTRING(jsonObject.value("monthstep").toInt()));

       w->writeTextElement(DAYMAX,TOSTRING(jsonObject.value("daymax").toInt()));
       w->writeTextElement(DAYMIN,TOSTRING(jsonObject.value("daymin").toInt()));
       w->writeTextElement(DAY,TOSTRING(jsonObject.value("day").toInt()));
       w->writeTextElement(DAYSTEP,TOSTRING(jsonObject.value("daystep").toInt()));


    }
     break;
    case VARTIME:
    {
       w->writeTextElement(HOURMAX,TOSTRING(jsonObject.value("hourmax").toInt()));
       w->writeTextElement(HOURMIN,TOSTRING(jsonObject.value("hourmin").toInt()));
       w->writeTextElement(HOUR,TOSTRING(jsonObject.value("hour").toInt()));
       w->writeTextElement(HOURSTEP,TOSTRING(jsonObject.value("hourstep").toInt()));

       w->writeTextElement(MINUTEMAX,TOSTRING(jsonObject.value("minutemax").toInt()));
       w->writeTextElement(MINUTEMIN,TOSTRING(jsonObject.value("minutemin").toInt()));
       w->writeTextElement(MINUTE,TOSTRING(jsonObject.value("minute").toInt()));
       w->writeTextElement(MINUTESTEP,TOSTRING(jsonObject.value("minutestep").toInt()));

       w->writeTextElement(SECONDMAX,TOSTRING(jsonObject.value("secondmax").toInt()));
       w->writeTextElement(SECONDMIN,TOSTRING(jsonObject.value("secondmin").toInt()));
       w->writeTextElement(SECOND,TOSTRING(jsonObject.value("second").toInt()));
       w->writeTextElement(SECONDSTEP,TOSTRING(jsonObject.value("secondstep").toInt()));
    }
         break;
    default:
        break;
    }

    w->writeEndElement();
    w->writeEndElement();

}

void GenerateDescriptionFile::writeDisTypeDesc(QXmlStreamWriter *w, FunctionItem *f)
{
    w->writeStartElement(DISTYPEDESC);
    w->writeTextElement(DISTYPE,TOSTRING(f->getDisplayType()));
    w->writeTextElement(DISPLAYTEXT,f->getDisplayName());
    w->writeStartElement(DISPARAMETERS);
    int distype = f->getDisplayType();
    QJsonParseError jsonError;
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;
    QJsonArray jsonArraydis;
    QJsonArray jsonArraypic;
    jsonDocument = QJsonDocument::fromJson(f->getParameterStrJson().toUtf8(),&jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
      //  qDebug()<<"YES";
    }
    else
    {
        qDebug()<<"参数有误"<<__FUNCTION__<<__LINE__;
        w->writeEndElement();
        return;
    }
    if(jsonDocument.isObject())
    {
        jsonObject = jsonDocument.object();
    }
    else
    {
        qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<"参数有误";
        w->writeEndElement();
        w->writeEndElement();
        return;
    }

  switch (distype) {
    case DISCMD:
        w->writeStartElement(DISPLAYITEM);
        w->writeTextElement(DISPLAYITEMSTRING,jsonObject.value("dison").toString());
        w->writeTextElement(DISPLAYITEMPICTUREINDEX,jsonObject.value("disonPic").toString());
        w->writeEndElement();
        w->writeStartElement(DISPLAYITEM);
        w->writeTextElement(DISPLAYITEMSTRING,jsonObject.value("disoff").toString());
        w->writeTextElement(DISPLAYITEMPICTUREINDEX,jsonObject.value("disoffPic").toString());
        w->writeEndElement();
        break;
    case  DISNUMBER:
            //w->writeTextElement("Scaling",TOSTRING(jsonObject.value("disScaling").toInt()));
        break;
    case DISSCROL:
            w->writeTextElement(DISPLAYMAX,TOSTRING(jsonObject.value("disMax").toInt()));
            w->writeTextElement(DISPLAYMIN,TOSTRING(jsonObject.value("disMin").toInt()));
            w->writeTextElement(DISPLAYSTEP,TOSTRING(jsonObject.value("disScaling").toInt()));
            w->writeTextElement(DISPLAYUNIT,TOSTRING(jsonObject.value("disUnit").toInt()));
            w->writeTextElement(DISPLAYDEFAULT,TOSTRING(jsonObject.value("disDefault").toInt()));

        break;
    case DISSTRING:
            w->writeTextElement(DISPLAYSTRING,jsonObject.value("disString").toString());
        break;
    case DISENUMBERATE:

        //显示类型为枚举型
         jsonArraydis = jsonObject.value("ParameterItems").toArray();
         jsonArraypic = jsonObject.value("ParameterItemPics").toArray();

        for(int i=0;i<jsonArraydis.size();i++)
        {
         w->writeStartElement(DISPLAYITEM);
            w->writeTextElement(DISPLAYITEMSTRING,jsonArraydis.at(i).toString());
            w->writeTextElement(DISPLAYITEMPICTUREINDEX,jsonArraypic.at(i).toString());
            w->writeEndElement();
        }
        break;
    case DISPICTURE:

        w->writeTextElement(DISPLAYPICTUREINDEX,TOSTRING(jsonObject.value("dispictureNumber").toInt()));
        break;
    case DISDATE:
        w->writeTextElement(DISPLAYYEAR,jsonObject.value("disYear").toString());
        w->writeTextElement(DISPLAYMONTH,jsonObject.value("disMonth").toString());
        w->writeTextElement(DISPLAYDAY,jsonObject.value("disDay").toString());
     break;
    case DISTIME:
        w->writeTextElement(DISPLAYHOUR,jsonObject.value("disHour").toString());
        w->writeTextElement(DISPLAYMINUTE,jsonObject.value("disMinute").toString());
        w->writeTextElement(DISPLAYSECOND,jsonObject.value("disSecond").toString());

         break;
    }
    w->writeEndElement();
    w->writeEndElement();


}

void GenerateDescriptionFile::writeStateStruct(QXmlStreamWriter *w, FunctionItem *f)
{
    w->writeStartElement(STATESTRUCT);
    w->writeTextElement(FUNCTIONNUMBER,TOSTRING(f->getFunctionNumber()));
    w->writeTextElement(DATAPOS,TOSTRING(0));
    w->writeTextElement(DATATYPE,TOSTRING(f->getDataType()));
    w->writeEndElement();
}


