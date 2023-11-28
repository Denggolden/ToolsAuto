﻿#ifndef REFLEXOBJECT_H
#define REFLEXOBJECT_H

#include <QWidget>
#include <QMap>

//pMainWin=new MainWin();
//pMainWin->setObjectName("MainWin");
//Ins.AddObjectIns(pMainWin);

#define CreateWin(ClassName,ObjName)\
    p##ObjName=new ClassName();\
    p##ObjName->setObjectName(#ObjName);\
    Ins.AddObjectIns(p##ObjName);

class TitleBarWin;
class StatusBarWin;

class TsHandelMainWin;
class TsFileCreateWin;
class TsFileExportWin;
class TsFileTranslateWin;
class QMFileGenerateWin;

class MainWin;

class ReflexObject
{
public:
    void AddObjectIns(QObject* pObject);
    void DeleteObjectIns(QString objectName);
    QObject *GetObjectIns(QString objectName);
    void InitObjectIns();
    void InitClassObj();

public:
    ReflexObject(ReflexObject&) = delete;
    ReflexObject& operator=(const ReflexObject&) = delete;
    static ReflexObject* Instance();

private:
    TitleBarWin* pTitleBarWin=nullptr;
    StatusBarWin *pStatusBarWin=nullptr;

    TsHandelMainWin *pTsHandelMainWin=nullptr;
    TsFileCreateWin *pTsFileCreateWin=nullptr;
    TsFileExportWin *pTsFileExportWin=nullptr;
    TsFileTranslateWin *pTsFileTranslateWin=nullptr;
    QMFileGenerateWin *pQMFileGenerateWin=nullptr;

    MainWin *pMainWin=nullptr;

private:
    ReflexObject();
    static ReflexObject Ins;
    QMap<QString, QObject*> ObjectMap;
};

#endif // REFLEXOBJECT_H
