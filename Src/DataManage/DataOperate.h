#ifndef DATAOPERATE_H
#define DATAOPERATE_H

#include <QObject>

#include "Src/Common/GlobalCus.h"

class DataOperate
{
    //Q_OBJECT
public:
    DataOperate(DataOperate&)=delete;
    DataOperate& operator=(const DataOperate&)=delete;
    static DataOperate* Instance();
private:
    DataOperate();
    static DataOperate Ins;

public:
    void InitMemVar();

    QString GetIniFileNode(QString groupName,QString key,QString defaultValue);
    void WriteIniFile(QString groupName,QString key,QString value);

    void SetKitDir(const QString &kitDir);
    QString GetKitDir();

    void SetProFilePath(const QString &proFilePath);
    QString GetProFilePath();

private:
    QString KitDir="";
    QString ProFilePath="";
};

#endif // DATAOPERATE_H
