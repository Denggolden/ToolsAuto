#ifndef REFLEXOBJECT_H
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

class TsFileCreateWin;
class TsFileExportWin;
class TsFileTranslateWin;

class MainWin;

class ReflexObject
{
public:
    void AddObjectIns(QWidget* pObject);
    void DeleteObjectIns(QString objectName);
    QWidget *GetObjectIns(QString objectName);
    void InitObjectIns();

public:
    ReflexObject(ReflexObject&) = delete;
    ReflexObject& operator=(const ReflexObject&) = delete;
    static ReflexObject* Instance();

private:
    TitleBarWin* pTitleBarWin=nullptr;

    TsFileCreateWin *pTsFileCreateWin=nullptr;
    TsFileExportWin *pTsFileExportWin=nullptr;
    TsFileTranslateWin *pTsFileTranslateWin=nullptr;

    MainWin *pMainWin=nullptr;

private:
    ReflexObject();
    static ReflexObject Ins;
    QMap<QString, QWidget*> ObjectMap;
};

#endif // REFLEXOBJECT_H
