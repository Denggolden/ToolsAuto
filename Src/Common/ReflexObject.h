#ifndef REFLEXOBJECT_H
#define REFLEXOBJECT_H

#include <QWidget>
#include <QMap>

#include "Src/Common/GlobalCus.h"
//pMainWin=new MainWin();
//pMainWin->setObjectName("MainWin");
//Ins.AddObjectIns(pMainWin);

#define CreateWin(ClassName,ObjName)\
    p##ObjName=new ClassName();\
    p##ObjName->setObjectName(#ObjName);\
    Ins.AddObjectIns(p##ObjName);

class TitleBarWin;
class StatusBarWin;
class OperateRecordWin;

class TsHandelMainWin;
class TsFileCreateWin;
class TsFileExportWin;
class TsFileTranslateWin;
class QMFileGenerateWin;

class FileConvertMainWin;
class JsonHandelWin;

class DataModelEditMainWin;
class ModelEditWin;
class DataEditWin;

class FileCompareDiffMainWin;
class FileCompareDiffWin;

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
    OperateRecordWin *pOperateRecordWin=nullptr;

    TsHandelMainWin *pTsHandelMainWin=nullptr;
    TsFileCreateWin *pTsFileCreateWin=nullptr;
    TsFileExportWin *pTsFileExportWin=nullptr;
    TsFileTranslateWin *pTsFileTranslateWin=nullptr;
    QMFileGenerateWin *pQMFileGenerateWin=nullptr;

    FileConvertMainWin *pFileConvertMainWin=nullptr;
    JsonHandelWin *pJsonHandelWin=nullptr;

    DataModelEditMainWin *pDataModelEditMainWin=nullptr;
    ModelEditWin *pModelEditWin=nullptr;
    DataEditWin *pDataEditWin=nullptr;

    FileCompareDiffMainWin *pFileCompareDiffMainWin=nullptr;
    FileCompareDiffWin *pFileCompareDiffWin=nullptr;

    MainWin *pMainWin=nullptr;

private:
    ReflexObject();
    static ReflexObject Ins;
    QMap<QString, QObject*> ObjectMap;
};

#endif // REFLEXOBJECT_H
