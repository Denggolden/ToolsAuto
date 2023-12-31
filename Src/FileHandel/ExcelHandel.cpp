﻿# pragma execution_character_set("utf-8")
#include "ExcelHandel.h"
#include <QDebug>

#include "ThirdLib/QXlsx/header/xlsxdocument.h"
QXLSX_USE_NAMESPACE

ExcelHandel::ExcelHandel()
{

}

ExcelHandel::~ExcelHandel()
{

}

void ExcelHandel::TSFileExportToExcelSeparate(QString exportFileName, bool exportItem[],const QList<TSFileInfo> &tSFileInfoList)
{
    //QXlsx::Document pXlsx(filePath);
    QXlsx::Document pXlsx;
    int row=0;
    QString tableName="TSExportData";
    pXlsx.addSheet(tableName);
    pXlsx.selectSheet(tableName);

    //生成表头名称
    QStringList headList;headList.clear();
    headList<<QObject::tr("源文");
    if(exportItem[1]==true)
        headList<<QObject::tr("译文");
    if(exportItem[2]==true)
        headList<<QObject::tr("注释");

    int headListSize=headList.size();
    row++;
    for(int tIndex=0;tIndex<headListSize;tIndex++){
        pXlsx.write(row,tIndex+1,headList.at(tIndex));
    }

    int tSFileInfoListSize=tSFileInfoList.size();
    for(int index=0;index<tSFileInfoListSize;index++){
        row++;
        pXlsx.write(row,1,tSFileInfoList.at(index).Source);
        if(exportItem[1]==true)
            pXlsx.write(row,2,tSFileInfoList.at(index).Translation);
        if(exportItem[2]==true)
            pXlsx.write(row,3,tSFileInfoList.at(index).Comment);
    }
    pXlsx.saveAs(exportFileName);
}

void ExcelHandel::TSFileExportToExcelSummary(QString exportFileName, QStringList header, const QList<TSFileSummaryInfo> &tSFileSummaryInfoList)
{
    QXlsx::Document pXlsx;
    int row=0;
    QString tableName="TSExportData";
    pXlsx.addSheet(tableName);
    pXlsx.selectSheet(tableName);

    int headListSize=header.size();
    row++;
    for(int tIndex=0;tIndex<headListSize;tIndex++){
        pXlsx.write(row,tIndex+1,header.at(tIndex));
    }

    int tSFileSummaryInfoListSize=tSFileSummaryInfoList.size();
    for(int index=0;index<tSFileSummaryInfoListSize;index++){
        row++;
        pXlsx.write(row,1,tSFileSummaryInfoList.at(index).Source);
        QList<QString> translationList= tSFileSummaryInfoList.at(index).TranslationList;
        int translationListSize=translationList.size();
        for(int tIndex=0;tIndex<translationListSize;tIndex++){
            //pXlsx.write(row,2+tIndex,translationList.at(tIndex));//实际的译文
            pXlsx.write(row,2+tIndex,header[tIndex+1].replace(".ts","",Qt::CaseInsensitive)+"@"+tSFileSummaryInfoList.at(index).Source);
        }
        pXlsx.write(row,2+translationListSize,tSFileSummaryInfoList.at(index).Comment);
    }
    pXlsx.saveAs(exportFileName);
}

void ExcelHandel::LoadTRListFile(QString tRListFilePath, QString languageType, QList<TSFileInfo> &TRFileInfoList)
{
    //读取翻译文件
    QXlsx::Document* pXlsx = nullptr;
    if(!pXlsx){
        pXlsx = new QXlsx::Document(tRListFilePath);
    }
    if(!pXlsx->load()){// 判断文件是否打开成功（也可以使用isLoadPackage）
        qInfo() << "excel打开失败!";
        return;
    }

    int tRColIndex=0;
    int rowCount = pXlsx->dimension().rowCount();           // 获取最大行数
    int colCount = pXlsx->dimension().columnCount();     // 获取最大列数
    qDebug()<<"colCount: "<<colCount;
    for(int col = 2; col <= colCount-1; col++){//找到翻译目标语言所在的列
        QString langType=pXlsx->read(1,col).toString().trimmed();
        if(langType==languageType){
            tRColIndex=col;
            break;
        }
    }

    for(int row = 2; row <= rowCount; row++){// 遍历每一行
        QString source=pXlsx->read(row,1).toString().trimmed();
        QString translation=pXlsx->read(row,tRColIndex).toString().trimmed();
        QString comment=pXlsx->read(row,4).toString().trimmed();
        //qDebug()<<"comment: "<<comment;
        TRFileInfoList.push_back(TSFileInfo(source,translation,comment));
    }
    delete pXlsx;
    pXlsx=nullptr;
}

void ExcelHandel::MergeTRListFile(const QString &newTRListFilePath, const QString &oldTRListFilePath,QString &mergeResult)
{
    //读取翻译文件
    QXlsx::Document* pNewXlsx = nullptr;
    if(!pNewXlsx){
        pNewXlsx = new QXlsx::Document(newTRListFilePath);
    }
    if(!pNewXlsx->load()){// 判断文件是否打开成功（也可以使用isLoadPackage）
        qInfo() << "excel打开失败!";
        return;
    }

    int rowCount = pNewXlsx->dimension().rowCount();           // 获取最大行数
    int colCount = pNewXlsx->dimension().columnCount();     // 获取最大列数

    for(int col=2;col<=colCount;col++){
        QString targetLang=pNewXlsx->read(1,col).toString().trimmed();
        QList<TRListExportInfo> tRListExportInfoList;tRListExportInfoList.clear();
        QList<QString> sourceExcepList;sourceExcepList.clear();
        //拿到源文及目标译文
        //为了时间复杂度空间复杂度综合及开发效率目前这样处理比较稳妥
        CreateTargetTranslationList(oldTRListFilePath,targetLang,tRListExportInfoList);
        for(int row = 2; row <= rowCount; row++){
            //通过 源文 找到目标译文
            QString source=pNewXlsx->read(row,1).toString().trimmed();
            QString translation=FindTargetTranslation(source,tRListExportInfoList);
            if(translation=="")
                sourceExcepList.append(source);
            else
                pNewXlsx->write(row,col,translation);
        }

        mergeResult+=QString(QObject::tr("---合并：%1 结果如下---\n")).arg(targetLang);

        mergeResult+=QString(QObject::tr("【对于新文件】以下源文未找到对应译文\n"));
        int sourceExcepListSize=sourceExcepList.size();
        for(int index=0;index<sourceExcepListSize;index++){
            mergeResult+=QString("%1\n").arg(sourceExcepList.at(index));
        }

        mergeResult+=QString(QObject::tr("【对于旧文件】以下译文未找到对应源文\n"));
        int tRListExportInfoListSize=tRListExportInfoList.size();
        for(int index=0;index<tRListExportInfoListSize;index++){
            if(tRListExportInfoList.at(index).IsFindSource==false){
                mergeResult+=QString(QObject::tr("源文：%1  译文：%2\n")).arg(tRListExportInfoList.at(index).Source).arg(tRListExportInfoList.at(index).Translation);
            }
        }

        mergeResult+=QString(QObject::tr("---合并：%1 结果END---\n")).arg(targetLang);
    }

    pNewXlsx->save();
    delete pNewXlsx;
    pNewXlsx=nullptr;
}

void ExcelHandel::CreateTargetTranslationList(const QString &oldTRListFilePath, const QString &targetLang, QList<TRListExportInfo> &tRListExportInfoList)
{
    QXlsx::Document* pOldXlsx = nullptr;
    if(!pOldXlsx){
        pOldXlsx = new QXlsx::Document(oldTRListFilePath);
    }
    if(!pOldXlsx->load()){// 判断文件是否打开成功（也可以使用isLoadPackage）
        qInfo() << "excel打开失败!";
        return;
    }

    int tRColIndex=0;
    int rowCount = pOldXlsx->dimension().rowCount();           // 获取最大行数
    int colCount = pOldXlsx->dimension().columnCount();     // 获取最大列数
    //qDebug()<<"colCount: "<<colCount;
    for(int col = 2; col <= colCount; col++){//找到翻译目标语言所在的列
        QString langType=pOldXlsx->read(1,col).toString().trimmed();
        if(langType==targetLang){
            tRColIndex=col;
            break;
        }
    }

    for(int row = 2; row <= rowCount; row++){// 遍历每一行
        QString source=pOldXlsx->read(row,1).toString().trimmed();
        QString translation=pOldXlsx->read(row,tRColIndex).toString().trimmed();
        tRListExportInfoList.push_back(TRListExportInfo(source,translation,false));
    }

    delete pOldXlsx;
    pOldXlsx=nullptr;
}

QString ExcelHandel::FindTargetTranslation(const QString &source,QList<TRListExportInfo> &tRListExportInfoList)
{
    QString translation="";
    int tRListExportInfoListSize=tRListExportInfoList.size();
    for(int index=0;index<tRListExportInfoListSize;index++){
        if(tRListExportInfoList.at(index).Source==source){
            translation=tRListExportInfoList.at(index).Translation;
            tRListExportInfoList[index].IsFindSource=true;
            break;
        }
    }
    return translation;
}
