#ifndef FILECOMPAREDIFFDETAILSWIN_H
#define FILECOMPAREDIFFDETAILSWIN_H

//#include <QWidget>
//#include "Src/Common/WidgetBase.h"
#include "Src/Common/DialogBase.h"

#include "Src/Common/GlobalCus.h"

#include <QCryptographicHash>
#include <QMutex>

namespace Ui {
class FileCompareDiffDetailsWin;
}

class FileCompareDiffDetailsWin : public DialogBase
{
    Q_OBJECT

public:
    explicit FileCompareDiffDetailsWin(QString openFlagSrc,QWidget *parent = nullptr);
    ~FileCompareDiffDetailsWin();

private:
    Ui::FileCompareDiffDetailsWin *ui;
    QList<QToolButton*>ToolButtonList;

protected:
    void InitClass() override;

    QString File1Path="";
    QString File2Path="";

    int NCount=0;
    QTimer* pTimer=nullptr;//

    QTimer* pTimerWait=nullptr;//

    bool IsOpenDiffCompete=true;
    QMutex IsOpenDiffCompeteMutex;

    bool IsCancel=false;
    QMutex IsCancelMutex;

    QString OpenFlagSrc="";

public:
    void InitGroupBox();
    void InitFrame();
    void InitLineEdit();
    void InitToolButton();
    void InitLabel();
    void InitTableWidget();
    void InitTimer();

    void LoadWinStatus();

    void SetTBtnIsEnable(bool enable);

    //打开时的flag
    void OpenFlag(QString flag);

    //开启对比
    void OpenDiff();
    //选择文件对话框
    void SelectFilePathFun(QString flag);

    //获取文件基本信息
    QString GetFileBaseInfo(const QString& filePath,const QString& flag);
    //获取文件高级信息
    QString GetFileHashStr(const QString &fileName,QCryptographicHash::Algorithm algorithm);

    //更新过程
    void UpDatePossess();
    void UpTableWidgetPossess(const QString& possessStr);
    void UpTableResultPossess(const QString& possessStr);

    void SetFile1And2Path(const QString& file1Path,const QString& file2Path);

protected:
    void SetIsOpenDiffCompete(bool isOpenDiffCompete);
    bool GetIsOpenDiffCompete();

    void SetIsCancel(bool isCancel);
    bool GetIsCancel();

signals:
    void SetTBtnIsEnableSignal(bool enable);
    void UpDateDiffResult(const QString& result);
    void ClearTableWidget(const QString& upDateTableWidget);
    void UpDateTableWidget(const QString& upDateTableWidget,int rowIndex,bool isNewAdd,bool isEqual,const QString& itemName,const QString& itemResult,const QString& itemStatus);
    void TimerIsEnable(bool enable);

public slots:
    void ToolButtonClicked(bool checked);
    void SetTBtnIsEnableSlots(bool enable);
    void UpDateDiffResultSlots(const QString& result);
    void ClearTableWidgetSlots(const QString& upDateTableWidget);
    void UpDateTableWidgetSlots(const QString& upDateTableWidget,int rowIndex,bool isNewAdd,bool isEqual,const QString& itemName,const QString& itemResult,const QString& itemStatus);
    void TimerIsEnableSlots(bool enable);
    void OnTimeOut();
    void OnTimeOutWait();
};

#endif // FILECOMPAREDIFFDETAILSWIN_H
