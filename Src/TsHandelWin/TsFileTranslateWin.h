#ifndef TSFILETRANSLATEWIN_H
#define TSFILETRANSLATEWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"
#include "Src/DataManage/DataModel.h"

class QToolButton;
class QTreeWidgetItem;
class QDomElement;

namespace Ui {
class TsFileTranslateWin;
}

class TsFileTranslateWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit TsFileTranslateWin(QWidget *parent = nullptr);
    ~TsFileTranslateWin();

private:
    Ui::TsFileTranslateWin *ui;
    QList<QToolButton*>ToolButtonList;

    bool TranslateItem[2]={true,false};

    QString TRListFilePath="";
    //为了资源不至于吃太多可以这样
    QList<TSFileInfo> TRFileInfoList;

    QString NewTRListFilePath="";
    QString OldTRListFilePath="";

protected:
    void InitClass() override;

public:
    void InitGroupBox();
    void InitFrame();
    void InitLineEdit();
    void InitTextEdit();
    void InitToolButton();
    void InitTreeWidget();
    void InitLabel();
    void InitCheckBox();
    void InitTabWidget();

    void LoadWinStatus();

    void LoadCreateTSFile();
    void FoEachDir(QString dirPath,QTreeWidgetItem* root);

    bool JudgeChildAllCheck(QTreeWidgetItem* parentItem);
    void SetChileItemCheckState(QTreeWidgetItem* item,bool checked);

    void SelectTRListFun();
    void TRTSFileFun();
    void SelectNewFileFun();
    void SelectOldFileFun();
    void MergeTRListFileFun();

    void HandelTRFileInfoList(QList<TRFileInfo> tRFileInfoList);
    void CreateTranslateFile(QDomElement *root);
    TRListRetInfo FindTRFileInfoList(QString source);

signals:
    void CtrlSetEnabled(const QString &ctrlName,const QString &txtStr,bool isEnabled);
    void AppendPossessLog(const QString &logStr,QString txtEditName);

public slots:
    void ToolButtonClicked(bool checked);
    void TreeWidgetItemChanged(QTreeWidgetItem *item, int column);
    void CheckBoxClicked(bool checked);
    void CtrlSetEnabledSlots(const QString &ctrlName,const QString &txtStr,bool isEnabled);
    void AppendPossessLogSlots(const QString &logStr,QString txtEditName);
    void TextEditCursorPositionChangedSlots();
};

#endif // TSFILETRANSLATEWIN_H
