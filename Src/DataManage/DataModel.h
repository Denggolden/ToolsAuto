#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>

typedef struct ExportFileInfo{
    ExportFileInfo(){};
    ExportFileInfo(QString tSFilePath,QString exportFilePath){TSFilePath=tSFilePath;ExportFilePath=exportFilePath;};
    QString TSFilePath;
    QString ExportFilePath;
}ExportFileInfo;

typedef struct TSFileInfo{
    TSFileInfo(){};
    TSFileInfo(QString source,QString translation,QString comment){Source=source;Translation=translation;Comment=comment;};
    bool operator == (const TSFileInfo &b) const
    {
        return (Source==b.Source);
        //return (Source==b.Source && Comment==b.Comment);
    }
//    bool operator()(const TSFileInfo& a){
//        return a.Source.size() < Source.size();
//        return a.Source.size() >=Source.size();
//    }
//    TSFileInfo &operator=(const TSFileInfo& a) {
//        this->Source=a.Source;
//        this->Translation=a.Translation;
//        this->Comment=a.Comment;
//        return *this;
//    }

    QString Source;
    QString Translation;
    QString Comment;
}TSFileInfo;

typedef struct TSFileSummaryInfo{
    TSFileSummaryInfo(){};
    TSFileSummaryInfo(QString source,QList<QString> translationList,QString comment,bool isFindTranslation){Source=source;TranslationList=translationList;Comment=comment;IsFindTranslation=isFindTranslation;};
    bool operator == (const TSFileSummaryInfo &a) const
    {
        return (Source==a.Source);
        //return (Source==b.Source && Comment==b.Comment);
    }

    QString Source;
    QList<QString> TranslationList;
    QString Comment;
    bool IsFindTranslation;
}TSFileSummaryInfo;

typedef struct SummaryExportInfo{
    SummaryExportInfo(){};
    SummaryExportInfo(QString exportFilePath,QList<QString> tSFilePathList){ExportFilePath=exportFilePath;TSFilePathList=tSFilePathList;};
    QString ExportFilePath;
    QList<QString> TSFilePathList;
} SummaryExportInfo;

typedef struct TRFileInfo{
    TRFileInfo(){};
    TRFileInfo(QString srcTSFilePath,QString outTSFilePath){SrcTSFilePath=srcTSFilePath;OutTSFilePath=outTSFilePath;};
    QString SrcTSFilePath;
    QString OutTSFilePath;
}TRFileInfo;

typedef struct TRListRetInfo{
    TRListRetInfo(){};
    TRListRetInfo(QString translation,QString comment){Translation=translation;Comment=comment;};
    QString Translation="";
    QString Comment="";
}TRListRetInfo;

typedef struct TRTSFileInfo{
    TRTSFileInfo(){};
    TRTSFileInfo(QString srcTRTSFilePath,QString outQMFilePath,QString command){SrcTRTSFilePath=srcTRTSFilePath;OutQMFilePath=outQMFilePath;Command=command;};
    QString SrcTRTSFilePath;
    QString OutQMFilePath;
    QString Command;
}TRTSFileInfo;

typedef  struct TRListExportInfo{
    TRListExportInfo(){};
    TRListExportInfo(QString source,QString translation,bool isFindSource){Source=source;Translation=translation;IsFindSource=isFindSource;};
    QString Source;
    QString Translation;
    bool IsFindSource;
}TRListExportInfo ;


typedef struct ModListInfo{
    ModListInfo(){};
    ModListInfo(QString modName,QString modFieldCount,int dataIDCount,QList<QString> fieldList){ModName=modName;ModFieldCount=modFieldCount;DataIDCount=dataIDCount;FieldList=fieldList;};
    QString ModName;
    QString ModFieldCount;
    int DataIDCount;
    QList<QString> FieldList;
}ModListInfo;

typedef struct ModDataInfo{
    ModDataInfo(){};
    ModDataInfo(QString modName,int iDCount,QList<QString> modData){ModName=modName;IDCount=iDCount;ModData=modData;};
    QString ModName;
    int IDCount;
    QList<QString> ModData;
}ModDataInfo;

typedef struct FieldKeyReplaceInfo{
    FieldKeyReplaceInfo(){};
    FieldKeyReplaceInfo(QString oldFieldKey,QString newFieldKey){OldFieldKey=oldFieldKey;NewFieldKey=newFieldKey;};
    QString OldFieldKey;
    QString NewFieldKey;
}FieldKeyReplaceInfo ;

enum LogGrade{
    Normal=0,Warning,Critical,Fatal
};

class DataModel
{
    //Q_OBJECT
public:
    DataModel();
};

#endif // DATAMODEL_H
