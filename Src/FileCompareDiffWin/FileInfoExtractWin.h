#ifndef FILEINFOEXTRACTWIN_H
#define FILEINFOEXTRACTWIN_H

#include <QWidget>

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

#include "Src/Common/GlobalCus.h"

#include <QCryptographicHash>

class QTreeWidgetItem;

namespace Ui {
class FileInfoExtractWin;
}

typedef struct FileInfoExtractBase
{
    FileInfoExtractBase(){};
    FileInfoExtractBase(QString result,QString duration):Result(result),Duration(duration){};
    QString Result="";
    QString Duration="";
}FileInfoExtractBase;

typedef struct FileInfoExtract
{
    FileInfoExtract() {}
    FileInfoExtract(FileInfoExtractBase fileName,FileInfoExtractBase suffix,FileInfoExtractBase size,FileInfoExtractBase lastModified,FileInfoExtractBase lastRead,FileInfoExtractBase birthTime,FileInfoExtractBase mD5,FileInfoExtractBase sHA1,FileInfoExtractBase sHA512):
        FileName(fileName),Suffix(suffix),Size(size),LastModified(lastModified),LastRead(lastRead),BirthTime(birthTime),MD5(mD5),SHA1(sHA1),SHA512(sHA512){}
    FileInfoExtractBase FileName;
    FileInfoExtractBase Suffix;
    FileInfoExtractBase Size;
    FileInfoExtractBase LastModified;
    FileInfoExtractBase LastRead;
    FileInfoExtractBase BirthTime;
    FileInfoExtractBase MD5;
    FileInfoExtractBase SHA1;
    FileInfoExtractBase SHA512;
}FileInfoExtract;

class FileInfoExtractWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit FileInfoExtractWin(QWidget *parent = nullptr);
    ~FileInfoExtractWin();

private:
    Ui::FileInfoExtractWin *ui;
    QList<QToolButton*>ToolButtonList;

    QString FileDirPath="";

    int NCount=0;
    QTimer* pTimer=nullptr;//

    QTreeWidgetItem *PrechildEml=nullptr;//

    QString CurCalFile="";//当前计算的文件名 Key
    QString CurSelectFile="";//当前计算的文件名 Key
    FileInfoExtract CurCalFileInfoExtract;//当前的计算文件信息
    QMap<QString,FileInfoExtract> FileInfoMapList;


protected:
    void InitClass() override;

public:
    void InitGroupBox();
    void InitLineEdit();
    void InitToolButton();
    void InitTreeWidget();
    void InitLabel();
    void InitTextEdit();
    void InitTableWidget();
    void InitTimer();

    void LoadWinStatus();

    void SetTBtnIsEnable(bool enable);


protected:
    void SelectFileOrDirPathFun(QString flag);

    void CalculateFun();

    void FoEachDirAsyn();
    void FoEachDirAsyn(QString dirPath,QTreeWidgetItem* root,int sleepCount);


    //更新过程
    void UpDatePossess();
    void UpTableWidgetPossess(const QString& possessStr);


    void FileCalExtract(const QString& filePath);
    //获取文件基本信息
    QString GetFileBaseInfo(const QString& filePath,const QString& flag);
    //获取文件高级信息
    QString GetFileHashStr(const QString &fileName,QCryptographicHash::Algorithm algorithm);

    QString CovDuration(long long duration);

    bool FindFileInfoMapList(const QString &key,FileInfoExtract& fileInfoExtract);

    void RefshTableWidget(const FileInfoExtract &fileInfoExtract);

signals:
    void SetTBtnIsEnableSignal(bool enable);
    void AppendPossessLog(const QString &logStr);
    void StartCreateTree(QTreeWidgetItem* root,QString name,QString type,QString path);
    void StartCreateTreeChild(QTreeWidgetItem* root,QTreeWidgetItem *childEml,QString name,QString type,QString path);
    void CreateTreeCmopete();
    void ClearTableWidget(const QString& upDateTableWidget);
    void UpDateTableWidget(const QString& upDateTableWidget,int rowIndex,bool isNewAdd,const QString& itemName,const QString& itemResult,const QString& duration);
    void AppendFileInfoMapList();
    void TimerIsEnable(bool enable);

    void SetCurCalFile(const QString& curCalFile);


public slots:
    void ToolButtonClicked(bool checked);
    void SetTBtnIsEnableSlots(bool enable);
    void AppendPossessLogSlots(const QString &logStr);
    void StartCreateTreeSlots(QTreeWidgetItem* root,QString name,QString type,QString path);
    void StartCreateTreeChildSlots(QTreeWidgetItem* root,QTreeWidgetItem *childEml,QString name,QString type,QString path);
    void CreateTreeCmopeteSlots();
    void ClearTableWidgetSlots(const QString& upDateTableWidget);
    void UpDateTableWidgetSlots(const QString& upDateTableWidget,int rowIndex,bool isNewAdd,const QString& itemName,const QString& itemResult,const QString& duration);
    void AppendFileInfoMapListSlots();
    void TimerIsEnableSlots(bool enable);
    void OnTimeOut();

    void SetCurCalFileSlots(const QString& curCalFile);


    void TreeWidgetItemPressed(QTreeWidgetItem *item, int column);
};

#endif // FILEINFOEXTRACTWIN_H
