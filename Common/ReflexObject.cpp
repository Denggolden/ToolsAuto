#include "ReflexObject.h"
#include <QDebug>

#include "StyleCtrl/TitleBarWin.h"

#include "TsHandelWin/TsFileCreateWin.h"
#include "TsHandelWin/TsFileExportWin.h"
#include "TsHandelWin/TsFileTranslateWin.h"

#include "MainWin.h"

ReflexObject ReflexObject::Ins;

void ReflexObject::AddObjectIns(QWidget *pObject)
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

QWidget *ReflexObject::GetObjectIns(QString objectName)
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
    CreateWin(TitleBarWin,TitleBarWin);

    CreateWin(TsFileCreateWin,TsFileCreateWin);
    CreateWin(TsFileExportWin,TsFileExportWin);
    CreateWin(TsFileTranslateWin,TsFileTranslateWin);

    CreateWin(MainWin,MainWin);
}
