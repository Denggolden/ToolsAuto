#ifndef EXCELHANDEL_H
#define EXCELHANDEL_H

#include <QObject>
#include "Src/DataManage/DataModel.h"

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

    void MergeTRListFile(const QString &newTRListFilePath,const QString &oldTRListFilePath);
    void CreateTargetTranslationList(const QString &oldTRListFilePath,const QString &targetLang,QList<TRListExportInfo> &tRListExportInfoList);
    QString FindTargetTranslation(const QString &source,const QList<TRListExportInfo> &tRListExportInfoList);
};

#endif // EXCELHANDEL_H
