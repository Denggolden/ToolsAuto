#include "ReflexObject.h"
#include <QDebug>
#include "Common/ObjectBase.h"
#include "Common/WidgetBase.h"

#include "MainWin.h"

#include "StyleCtrl/TitleBarWin.h"
#include "StyleCtrl/StatusBarWin.h"

#include "TsHandelWin/TsHandelMainWin.h"
#include "TsHandelWin/TsFileCreateWin.h"
#include "TsHandelWin/TsFileExportWin.h"
#include "TsHandelWin/TsFileTranslateWin.h"
#include "TsHandelWin/QMFileGenerateWin.h"

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

    //软件主体-Qt翻译自动化
    CreateWin(TsHandelMainWin,TsHandelMainWin);
    CreateWin(TsFileCreateWin,TsFileCreateWin);
    CreateWin(TsFileExportWin,TsFileExportWin);
    CreateWin(TsFileTranslateWin,TsFileTranslateWin);
    CreateWin(QMFileGenerateWin,QMFileGenerateWin);

    InitClassObj();
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
