#include "filegenerater.h"

FileGenerater::FileGenerater()
{
    FunctionPos = 0;

}

void FileGenerater::CreateXml(QString fileName, HeadInformation *h, QList<FunctionStruct *> *l)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"OPEN SUCESS";

        QXmlStreamWriter writer(&file);

        writer.writeStartDocument();
        writer.writeStartElement(ROOT);
        writeHeadXml(&writer,h); // 写头部
        writeFunctionXml(&writer,l); //写Function
        writer.writeEndElement();//根目录
        writer.writeEndDocument();
    }

}

void FileGenerater::writeHeadXml(QXmlStreamWriter *w, HeadInformation *head)
{
    w->writeStartElement(HEADINFORMATION);
    w->writeTextElement(DDVERSION,VERSION_DEVFILE_STD);    //v0.2
    w->writeTextElement(DDFILEVER,VERSION_FILE);
    w->writeTextElement(DDDEVICETYPE,"01"); //使用者
    w->writeTextElement(DDURLLENGTH,TOSTRING(head->getUrl().length()));
    w->writeTextElement(DDURL,head->getUrl());
    w->writeTextElement(MANUFACTURERCODE,head->getManufactureCode());
    w->writeTextElement(DEVICENAME,head->getDeviceName());
    //    w->writeTextElement(DEVICETYPEID,QString("0")+QString::number(head->getDeviceClass1())+"0"+QString::number(head->getDeviceClass2()));

    int type = head->getDeviceClass1()*100+head->getDeviceClass2();

    w->writeTextElement(DEVICETYPEID,QString::number(type).length()<4?"0"+QString::number(type):QString::number(type));

    w->writeTextElement(MODELID,head->getDevcieModeID());
    w->writeTextElement(MODEL,head->getDevcieMode());
    w->writeTextElement(UPDATE,QString::number(head->getUpdate()?1:0));

    w->writeTextElement(SYNCHRONOUS,QString::number(head->getSync()?1:0));
    w->writeEndElement();

}

void FileGenerater::writeFunctionXml(QXmlStreamWriter *w, QList<FunctionStruct *> *l)
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
        //        w->writeTextElement(FUNCTIONNUMBER,QString::number(FunctionNumber));
        PRINTLOG(l->at(i)->getFunctionNumber());
        w->writeTextElement(FUNCTIONNUMBER,QString::number(l->at(i)->getFunctionNumber()));
        //        l->at(i)->setFunctionNumber(FunctionNumber);
        w->writeTextElement(FUNCTIONNAME,l->at(i)->getFunctionName());
        w->writeTextElement(DISPLAY,QString::number(1));
        w->writeTextElement(DIRECTION,QString::number(l->at(i)->getDirection()));
        if(l->at(i)->getVarType()!=VARALARM && l->at(i)->getVarType()!=7)   //警告型没有pos
        {
            w->writeTextElement(DATAPOS,TOSTRING(this->FunctionPos));
            BytePos.append(FunctionPos);
            BitPos.append(0);

        }
        writeVarTypeDesc(w,l->at(i));
        //        if(l->at(i)->getVarType()!=VARALARM)  //警告信无显示描述
        //        {
        //            this->writeDisTypeDesc(w,l->at(i));
        //        }
        writeDisTypeDesc(w,l->at(i));
        FunctionNumber++;
        w->writeEndElement();

    }
    w->writeEndElement();

}

void FileGenerater::writeVarTypeDesc(QXmlStreamWriter *w, FunctionStruct *f)
{
    w->writeStartElement(VARTYPEDESC);
    PRINTLOG(f->getVarType());
    int vartype = f->getVarType();
    if (vartype == 7)
    {
        w->writeTextElement(VARTYPE,QString::number(VARALARM));
    }
    else
    {
        w->writeTextElement(VARTYPE,QString::number(vartype));
    }
    w->writeTextElement(VARINDEX,TOSTRING(f->getVarIndex()));

    PRINTLOG(f->getLength());

    if(vartype != 7)
    {

        w->writeTextElement(VARLENGTH,QString::number(f->getLength()));
        FunctionPos += f->getLength();
        //        w->writeTextElement(DATATYPE,TOSTRING(f->getDataType())); 数据类型
    }
    w->writeTextElement(DATATYPE,TOSTRING(f->getDataType()));
    w->writeStartElement(VARPARAMETERS);

    if(vartype == VARCMD)
    {
        cmdParameters cp =  f->getCmdParameters();
        w->writeTextElement(ITEM,TOSTRING(cp.parameter1));
        w->writeTextElement(ITEM,TOSTRING(cp.parameter2));
        w->writeTextElement(DEFAULTVALUE,TOSTRING(cp.defaultValue));

    }
    else if(vartype == VARNUMBER)
    {
        numberParamters np = f->getNumberParameters();
        w->writeTextElement(MAX,TOSTRING(np.max));
        w->writeTextElement(MIN,TOSTRING(np.min));
        w->writeTextElement(SCALLING,TOSTRING(np.scalling));
        w->writeTextElement(UNIT,TOSTRING(np.uint));
        w->writeTextElement(DEFAULTVALUE,TOSTRING(np.defaultValue));

    }
    else if(vartype == VARENUMBERATE)
    {
        enumParameters ep = f->getenumParameters();
        w->writeTextElement(ITEMNUM,TOSTRING(ep.itemNum));
        w->writeTextElement(DEFAULTVALUE,TOSTRING(ep.defaultValue));
        for(int i=0;i<ep.items.size();i++)
        {
            w->writeTextElement(ITEM,TOSTRING(ep.items.keys().at(i)));
        }
    }
    else if(vartype == VARDATE)
    {
        dateParameters dp = f->getDateParameters();
        w->writeTextElement(YEARMAX,TOSTRING(dp.maxDate.year()));
        w->writeTextElement(YEARMIN,TOSTRING(dp.minDate.year()));
        w->writeTextElement(YEAR,TOSTRING(dp.date.year()));
        w->writeTextElement(YEARSTEP,TOSTRING(dp.yearStep));

        w->writeTextElement(MONTHMAX,TOSTRING(dp.maxDate.month()));
        w->writeTextElement(MONTHMIN,TOSTRING(dp.minDate.month()));
        w->writeTextElement(MONTH,TOSTRING(dp.date.month()));
        w->writeTextElement(MONTHSTEP,TOSTRING(dp.monthStep));

        w->writeTextElement(DAYMAX,TOSTRING(dp.maxDate.day()));
        w->writeTextElement(DAYMIN,TOSTRING(dp.minDate.day()));
        w->writeTextElement(DAY,TOSTRING(dp.date.day()));
        w->writeTextElement(DAYSTEP,TOSTRING(dp.dayStep));

    }
    else if(vartype == VARTIME)
    {
        timeParameters tp = f->getTimeParameters();
        w->writeTextElement(HOURMAX,TOSTRING(tp.maxTime.hour()));
        w->writeTextElement(HOURMIN,TOSTRING(tp.minTime.hour()));
        w->writeTextElement(HOUR,TOSTRING(tp.time.hour()));
        w->writeTextElement(HOURSTEP,TOSTRING(tp.hourStep));

        w->writeTextElement(MINUTEMAX,TOSTRING(tp.maxTime.minute()));
        w->writeTextElement(MINUTEMIN,TOSTRING(tp.minTime.minute()));
        w->writeTextElement(MINUTE,TOSTRING(tp.time.minute()));
        w->writeTextElement(MINUTESTEP,TOSTRING(tp.minuteStep));

        w->writeTextElement(SECONDMAX,TOSTRING(tp.maxTime.second()));
        w->writeTextElement(SECONDMIN,TOSTRING(tp.minTime.second()));
        w->writeTextElement(SECOND,TOSTRING(tp.time.second()));
        w->writeTextElement(SECONDSTEP,TOSTRING(tp.secondStep));
    }
    else if(vartype == VARALARM || vartype == 7)
    {
        alarmParameters ap;
        if(vartype == VARALARM)
        {
            ap  = f->getAlarmParameters();
        }
        else
        {
            ap = f->getAlarmParameters();
        }

        int bitpos = 0;
        int count = 0;

        for(int i=0;i<ap.alarmCode.size();i++)
        {
            w->writeStartElement(ALARMITEM);
            w->writeTextElement(BYTEPOS,TOSTRING(count+FunctionPos));
            f->setDataPos(count+FunctionPos);
            BytePos.append(count+FunctionPos);
            BitPos.append(7-bitpos);
            w->writeTextElement(BITPOS,TOSTRING(7-bitpos));
            w->writeTextElement(ALARMCODE,TOSTRING(ap.alarmCode.at(i)));
            w->writeTextElement(ALARMCLASS,TOSTRING(ap.alarmClass.at(i)));
            w->writeTextElement(ALARMSTRING,ap.alarmName.at(i));
            w->writeEndElement();
            bitpos++;
            if(bitpos==8)
            {
                count++;
                bitpos = 0;
            }

        }
        if(bitpos % 7 != 0 )
        {
            FunctionPos +=1;
        }

    }
    w->writeEndElement();
    w->writeEndElement();
}

void FileGenerater::writeDisTypeDesc(QXmlStreamWriter *w, FunctionStruct *f)
{
    if(f->getVarType() == VARNUMBER)
    {
        if(f->getDirection() == 3)
        {
            f->setDisType(DISSCROL);
        }
        else
        {
            f->setDisType(DISNUMBER);
        }
    }
    w->writeStartElement(DISTYPEDESC);
    w->writeTextElement(DISTYPE,TOSTRING(f->getDisType()));
    w->writeTextElement(DISPLAYTEXT,f->getFunctionName());
    w->writeStartElement(DISPARAMETERS);
    int distype = f->getDisType();

    if(distype == DISCMD)
    {
        if(f->getVarType() == VARCMD)
        {
            cmdParameters cp = f->getCmdParameters();
            w->writeStartElement(DISPLAYITEM);
            w->writeTextElement(DISPLAYITEMSTRING,cp.dis1);
            w->writeTextElement(DISPLAYITEMPICTUREINDEX,"");
            w->writeEndElement();
            w->writeStartElement(DISPLAYITEM);
            w->writeTextElement(DISPLAYITEMSTRING,cp.dis2);
            w->writeTextElement(DISPLAYITEMPICTUREINDEX,"");
            w->writeEndElement();
        }
        else
        {

        }

    }
    else if(distype == DISNUMBER)
    {
        if(f->getVarType() == VARNUMBER)
        {

        }

    }
    else if(distype == DISENUMBERATE)
    {
        if(f->getVarType() == VARENUMBERATE)
        {
            enumParameters ep = f->getenumParameters();
            for(int i=0;i<ep.itemNum;i++)
            {
                w->writeStartElement(DISPLAYITEM);
                int t = ep.items.keys().at(i);
                w->writeTextElement(DISPLAYITEMSTRING,ep.items.value(t));
                w->writeTextElement(DISPLAYITEMPICTUREINDEX,"");
                w->writeEndElement();
            }
        }

    }
    else if(distype == DISDATE)
    {

        if(f->getVarType() == VARDATE)
        {
            dateParameters dp = f->getDateParameters();
            w->writeTextElement(DISPLAYYEAR,dp.yearDis);
            w->writeTextElement(DISPLAYMONTH,dp.monthDis);
            w->writeTextElement(DISPLAYDAY,dp.dayDis);
        }
    }
    else if(distype == DISTIME)
    {
        if(f->getVarType() == VARTIME)
        {
            timeParameters tp = f->getTimeParameters();
            w->writeTextElement(DISPLAYHOUR,tp.hourDis);
            w->writeTextElement(DISPLAYMINUTE,tp.minuteDis);
            w->writeTextElement(DISPLAYSECOND,tp.secondDis);
        }
    }
    else if(distype == DISSCROL)
    {
        if(f->getVarType() == VARNUMBER)
        {
            numberParamters np = f->getNumberParameters();
            w->writeTextElement(DISPLAYMAX,TOSTRING(np.max));
            w->writeTextElement(DISPLAYMIN,TOSTRING(np.min));
            w->writeTextElement(DISPLAYSTEP,TOSTRING(np.step));
            w->writeTextElement(DISPLAYUNIT,TOSTRING(np.uint));
            w->writeTextElement(DISPLAYDEFAULT,TOSTRING(np.defaultValue));
        }

    }
    else if(distype == DISSTRING)
    {

    }
    else if(distype == DISPICTURE)
    {

    }
    w->writeEndElement();
    w->writeEndElement();
}

void FileGenerater::CreateDocument(QString fileName, QList<FunctionStruct *> *l)
{
    PRINTLOG("");
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("c:\\a.pdf");
    QFile helpfile(":/doc.html");
    qDebug()<<helpfile.open(QFile::ReadOnly);
    QByteArray helpcontent =  helpfile.readAll();
    qDebug()<<helpcontent.size();
    QTextDocument *doc = new QTextDocument();
    QTextCursor cursor(doc);
    cursor.insertHtml(helpcontent);


    WriteDoc(&cursor,l);

    doc->print(&printer);


}

void FileGenerater::WriteDoc(QTextCursor *cursor, QList<FunctionStruct *> *l)
{

    QTextTableFormat tableFormat;
    QTextCharFormat titleFormat;
    QTextCharFormat titleFormat2;
    QTextCharFormat textFormat;

    QFont textFont;
    textFont.setFamily("Microsoft YaHei");
    textFont.setPointSize(10);
    textFont.setBold(false);

    QFont titleFont;
    titleFont.setFamily("Microsoft YaHei");
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleFormat.setFont(titleFont);
    QFont titleFont2;
    titleFont2.setFamily("Microsoft YaHei");
    titleFont2.setPointSize(14);
    titleFont2.setBold(true);
    titleFormat2.setFont(titleFont2);

    cursor->insertBlock();
    cursor->setPosition(cursor->document()->lastBlock().position());
    cursor->insertText("5 设备",titleFormat);
   cursor->insertBlock();
    cursor->setPosition(cursor->document()->lastBlock().position());

    cursor->insertText("5.1 控制命令\n",titleFormat2);


   QTextCharFormat fragFormat;
    QFont fragfont;
    fragfont.setPointSize(8);
    fragfont.setFamily("Microsoft YaHei");
    fragfont.setItalic(true);
    fragfont.setBold(false);
    fragFormat.setFont(fragfont);
    cursor->insertBlock();

    cursor->setPosition(cursor->document()->lastBlock().position());
    cursor->insertText("    一个控制帧的数据域由功能号（在一个设备中唯一标识一个功能），功能码（命令控制字）以及参数构成，详细控制命令见下表:\n",textFormat);
    cursor->insertText("                               表5-1 控制命令表",fragFormat);


    cursor->insertBlock();

    cursor->setPosition(cursor->document()->lastBlock().position());

    FunctionStruct *alarmf = NULL;
    tableFormat.setAlignment(Qt::AlignCenter);
    tableFormat.setWidth(400);
    cursor->insertBlock();

    cursor->setPosition(cursor->document()->lastBlock().position());
    QTextTable *table = cursor->insertTable(1,5,tableFormat);
    table->cellAt(0,0).firstCursorPosition().insertText("功能名称");
    table->cellAt(0,1).firstCursorPosition().insertText("功能号");
    table->cellAt(0,2).firstCursorPosition().insertText("功能码");
    table->cellAt(0,3).firstCursorPosition().insertText("参数长度");
    table->cellAt(0,4).firstCursorPosition().insertText("参数说明");

    int controlrowcount = 1;
    for(int i=0;i<l->size();i++)
    {
        FunctionStruct *f = l->at(i);
        if(f->getDirection()==3)
        {
        qDebug()<<f->getFunctionName();

            table->insertRows(controlrowcount,1);
            qDebug()<<f->getFunctionNumber();
            table->cellAt(controlrowcount,0).firstCursorPosition().insertText(f->getFunctionName());
            table->cellAt(controlrowcount,1).firstCursorPosition().insertText(QString::number(f->getFunctionNumber(),16));
            table->cellAt(controlrowcount,2).firstCursorPosition().insertText(QString::number(f->getVarIndex(),16));
            table->cellAt(controlrowcount,3).firstCursorPosition().insertText(QString::number(f->getLength())+"Byte");
            table->cellAt(controlrowcount,4).firstCursorPosition().insertText(getDescribe((f)));
        controlrowcount++;
        }
        if(f->getVarType() == 7)
        {
            qDebug()<<f->getAlarmParameters().alarmName;
            alarmf = f;
        }

    }
    qDebug()<<"Rowcount is "<<controlrowcount;




    //上报状态格式部分
    cursor->insertBlock();
     cursor->setPosition(cursor->document()->lastBlock().position());

    cursor->insertText("5.2 状态上报格式\n",titleFormat2);
    cursor->insertText("    状态上报要求将设备所有状态全部以状态帧的形式上报，字节"
                       "偏移表示相对于状态帧的数据的第0个字节的偏移量，位偏移表示相对于字节偏移的"
                       "位偏移(如一个变量字节偏移为10，位偏移为7，表示第10个字节的第7位)。\n",textFormat);
    cursor->insertText("                         表5-2 状态上报格式表",fragFormat);
    QTextTable *tableState = cursor->insertTable(1,4,tableFormat);
    tableState->cellAt(0,0).firstCursorPosition().insertText("功能名称");
    tableState->cellAt(0,1).firstCursorPosition().insertText("字节偏移");
    tableState->cellAt(0,2).firstCursorPosition().insertText("位偏移");
    tableState->cellAt(0,3).firstCursorPosition().insertText("长度");

    int count = 0;
    for(int i=0;i<l->size();i++)
    {
        FunctionStruct *f = l->at(i);
        if(f->getVarType()!=7)
        {
            tableState->insertRows(count+1,1);
            tableState->cellAt(count+1,0).firstCursorPosition().insertText(f->getFunctionName(),textFormat);
            tableState->cellAt(count+1,1).firstCursorPosition().insertText(QString::number(BytePos.at(count)),textFormat);
            tableState->cellAt(count+1,2).firstCursorPosition().insertText(QString::number(BitPos.at(count)),textFormat);
            tableState->cellAt(count+1,3).firstCursorPosition().insertText(QString::number(f->getLength())+"Byte",textFormat);
            count++;
        }
        else
        {
            alarmParameters a = f->getAlarmParameters();
            for(int j=0;j<a.alarmClass.size();j++)
            {
                tableState->insertRows(count+1,1);
                tableState->cellAt(count+1,0).firstCursorPosition().insertText(a.alarmName.at(j));
                tableState->cellAt(count+1,1).firstCursorPosition().insertText(QString::number(BytePos.at(count)));
                tableState->cellAt(count+1,2).firstCursorPosition().insertText(QString::number(BitPos.at(count)));
                tableState->cellAt(count+1,3).firstCursorPosition().insertText("1Bit");

                count++;

            }
        }



    }




//    报警部分

    if(alarmf!=NULL)
    {
        cursor->setPosition(cursor->document()->lastBlock().position());
        cursor->insertText("5.3 报警定义\n",titleFormat2);
        cursor->insertText("    当设备有报警或者故障需要上报，设备以报警帧/异常帧的形式将故障代码发送"
                           "出去，报警码如下表所示：\n",textFormat);
        cursor->insertText("                      表5-3 设备报警表",fragFormat);
        qDebug()<<alarmf->getAlarmParameters().alarmName;
        alarmParameters alarmp= alarmf->getAlarmParameters();
        QTextTable *tableAmarl = cursor->insertTable(1,3,tableFormat);

        tableAmarl->cellAt(0,0).firstCursorPosition().insertText("报警码");
        tableAmarl->cellAt(0,1).firstCursorPosition().insertText("报警名称");
        tableAmarl->cellAt(0,2).firstCursorPosition().insertText("报警级别");
        for(int i=0;i<alarmp.alarmClass.size();i++)
        {
            tableAmarl->insertRows(i+1,1);
            tableAmarl->cellAt(i+1,0).firstCursorPosition().insertText(QString::number(alarmp.alarmCode.at(i)));
            tableAmarl->cellAt(i+1,1).firstCursorPosition().insertText(alarmp.alarmName.at(i));
            tableAmarl->cellAt(i+1,2).firstCursorPosition().insertText(QString::number(alarmp.alarmClass.at(i)));
        }

    }


    qDebug()<<BitPos;
    qDebug()<<BytePos;


}

void FileGenerater::WriteDescribe(FunctionStruct *f)
{
    int vartype = f->getVarType();
    if(vartype == VARCMD)
    {

    }
    else if(vartype == VARTIME)
    {

    }


}

QString FileGenerater::getDescribe(FunctionStruct *f)
{
    int vartype = f->getVarType();
    QString s;
    if(vartype == VARCMD)
    {
        s+=QString::number(f->getCmdParameters().parameter1)+":"
                +f->getCmdParameters().dis1+"\n"+
                QString::number(f->getCmdParameters().parameter2)+":"
                +f->getCmdParameters().dis2;

    }
    else if(vartype == VARNUMBER)
    {
        numberParamters n = f->getNumberParameters();
        s+="最大值:"+QString::number(n.max)+"\n"
                +"最小值："+QString::number(n.min)+"\n"
                +"分辨率："+QString::number(n.scalling)+"\n"+
                QString::number(n.min)+"和"+QString::number(n.max)+
                "表示实际值："+QString::number(n.min/n.scalling)+"到"
                +QString::number(n.max/n.scalling);

    }
    else if(vartype == VARENUMBERATE)
    {
        enumParameters e = f->getenumParameters();
        for(int i=0;i<e.itemNum;i++)
        {
            s+=QString::number(e.items.keys().at(i))+":"+
                    e.items.values().at(i)+"\n";
        }

    }
    else if(vartype == VARTIME)
    {
        s+="|时(1Byte)|分（1Byte）|秒（1Byte）";
    }
    else if(vartype == VARDATE)
    {
        s+="|年（2Byte，大端）|月（1Byte）|日（1Byte）|";

    }

    return s;
}



