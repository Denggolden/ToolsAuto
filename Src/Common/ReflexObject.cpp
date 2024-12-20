﻿#include "ReflexObject.h"
#include <QDebug>
#include "Src/Common/ObjectBase.h"
#include "Src/Common/WidgetBase.h"

#include "MainWin.h"

#include "Src/StyleCtrl/TitleBarWin.h"
#include "Src/StyleCtrl/StatusBarWin.h"
#include "Src/StyleCtrl/OperateRecordWin.h"

#include "Src/TsHandelWin/TsHandelMainWin.h"
#include "Src/TsHandelWin/TsFileCreateWin.h"
#include "Src/TsHandelWin/TsFileExportWin.h"
#include "Src/TsHandelWin/TsFileTranslateWin.h"
#include "Src/TsHandelWin/QMFileGenerateWin.h"

#include "Src/FileConvertWin/FileConvertMainWin.h"
#include "Src/FileConvertWin/JsonHandelWin.h"

#include "Src/DataModelEditWin/DataModelEditMainWin.h"
#include "Src/DataModelEditWin/ModelEditWin.h"
#include "Src/DataModelEditWin/DataEditWin.h"

#include "Src/FileCompareDiffWin/FileCompareDiffMainWin.h"
#include "Src/FileCompareDiffWin/FileCompareDiffWin.h"

ReflexObject ReflexObject::Ins;

void ReflexObject::AddObjectIns(QObject *pObject)
{
    QString objectName=pObject->objectName();
    auto it = ObjectMap.find(objectName);
    if (it == ObjectMap.end()) {//找不到元素
        ObjectMap.insert(objectName, pObject);
        qDebug()<<"---"<<objectName<<" Create---"<<"  Address:"<<(void*)pObject;
    }
    else {// 找到元素
    }
}

void ReflexObject::DeleteObjectIns(QString objectName)
{
    if(ObjectMap.contains(objectName)){
        qDebug()<<"---"<<objectName<<" Delete---"<<"  Address:"<<(void*)ObjectMap.find(objectName).value();
        int DeleteSuccess= ObjectMap.remove(objectName);
        if(DeleteSuccess==1){
            qDebug()<<"---"<<objectName<<" Delete---";
        }
    }
}

QObject *ReflexObject::GetObjectIns(QString objectName)
{
    if(ObjectMap.contains(objectName)){
        qDebug()<<"---"<<objectName<<" find---"<<"  Address:"<<(void*)ObjectMap.find(objectName).value();
        return ObjectMap.find(objectName).value();
    }
    return nullptr;
}

ReflexObject::ReflexObject()
{
    //InitObjectIns();
}

ReflexObject *ReflexObject::Instance()
{
    return &Ins;
}

void ReflexObject::InitObjectIns()
{
    //主窗口
    CreateWin(MainWin,MainWin);

    //标题栏
    CreateWin(TitleBarWin,TitleBarWin);
    //状态栏
    CreateWin(StatusBarWin,StatusBarWin);
    //操作记录
    CreateWin(OperateRecordWin,OperateRecordWin);

    //软件主体-Qt翻译自动化
    CreateWin(TsHandelMainWin,TsHandelMainWin);
    CreateWin(TsFileCreateWin,TsFileCreateWin);
    CreateWin(TsFileExportWin,TsFileExportWin);
    CreateWin(TsFileTranslateWin,TsFileTranslateWin);
    CreateWin(QMFileGenerateWin,QMFileGenerateWin);
    //软件主体-文件转换
    CreateWin(FileConvertMainWin,FileConvertMainWin);
    CreateWin(JsonHandelWin,JsonHandelWin);
    //软件主体-数据模型编辑
    CreateWin(DataModelEditMainWin,DataModelEditMainWin);
    CreateWin(ModelEditWin,ModelEditWin);
    CreateWin(DataEditWin,DataEditWin);
    //软件主体-文件(夹)对比
    CreateWin(FileCompareDiffMainWin,FileCompareDiffMainWin);
    CreateWin(FileCompareDiffWin,FileCompareDiffWin);

    //InitClassObj();
}

void ReflexObject::InitClassObj()
{
    QMap<QString, QObject*>::iterator it;
    for (it = ObjectMap.begin(); it != ObjectMap.end(); ++it){
        qDebug() << it.key() << ":" << it.value();
        if ((dynamic_cast<ObjectBase*>(it.value())) != nullptr) {
            //qDebug() << "此类为 ObjectBase";
            ObjectBase *pObjectBase = dynamic_cast<ObjectBase*>(it.value());
            pObjectBase->InitClass();
        }
        if ((dynamic_cast<WidgetBase*>(it.value())) != nullptr) {
            //qDebug() << "此类为 WidgetBase";
            WidgetBase *pWidgetBase = dynamic_cast<WidgetBase*>(it.value());
            pWidgetBase->InitClass();
        }
    }
}
