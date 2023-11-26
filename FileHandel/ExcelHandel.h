#ifndef EXCELHANDEL_H
#define EXCELHANDEL_H

#include <QObject>
#include "DataManage/DataModel.h"

class ExcelHandel
{
    //Q_OBJECT
public:
    ExcelHandel();
    ~ExcelHandel();

public:
    void TSFileExportToExcelSeparate(QString exportFileName,bool exportItem[],const QList<TSFileInfo> &tSFileInfoList);
    void TSFileExportToExcelSummary(QString exportFileName,QStringList header,const QList<TSFileSummaryInfo> &tSFileSummaryInfoList);

    void LoadTRListFile(QString tRListFilePath,QString languageType,QList<TSFileInfo> &TRFileInfoList);
};

#endif // EXCELHANDEL_H
