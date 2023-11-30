#ifndef TSFILEEXPORTWIN_H
#define TSFILEEXPORTWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"
#include "Src/DataManage/DataModel.h"

class QToolButton;
class QTreeWidgetItem;
class QTreeWidget;
class QDomElement;


namespace Ui {
class TsFileExportWin;
}

class TsFileExportWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit TsFileExportWin(QWidget *parent = nullptr);
    ~TsFileExportWin();

private:
    Ui::TsFileExportWin *ui;
    QList<QToolButton*>ToolButtonList;

    //QList<QTreeWidgetItem*>FalgEnableList;

    bool ExportItem[3]={true,false,false};
    QString SourceFileName="";
    int WorkMode=0;
    int SourceFileNameIndex=-1;

protected:
    void InitClass() override;

public:
    void InitGroupBox();
    void InitFrame();
    void InitTextEdit();
    void InitComboBox();
    void InitToolButton();
    void InitLabel();
    void InitCheckBox();
    void InitRadioButton();
    void InitTreeWidget();

    void LoadCreateTSFile();
    void FoEachDir(QString dirPath,QTreeWidgetItem* root);

    void GetCreateTSFileName();
    void GetCreateTSFileName(QString dirPath,QStringList &tsFileNameList);

    void LoadWinStatus();

    bool JudgeChildAllCheck(QTreeWidgetItem* parentItem);
    void SetChileItemCheckState(QTreeWidgetItem* item,bool checked);

    void SetChileItemCheckState(QTreeWidgetItem* item,QString itemName);

    void RestTreeWidgetItemStatus(QTreeWidgetItem* item);

    void ExportTSFileFun();
    void SummaryExportTSFile();
    void SeparateExportTSFile();

    void HandelExportFileInfoList(QList<ExportFileInfo> exportFileInfoList);
    void ForEachTSFileSeparate(QDomElement *root,QList<TSFileInfo> &tSFileInfoList);
    void ClearSameData(const QList<TSFileInfo> &srcTSFileInfoList,QList<TSFileInfo> &outTSFileInfoList);

    void HandelSummaryExportInfoList(QList<SummaryExportInfo> summaryExportInfoList);
    void ForEachTSFileSummarySource(QDomElement *root,QList<TSFileSummaryInfo> &tSFileSummaryInfoList);
    void ForEachTSFileSummaryTran(QDomElement *root,QList<TSFileSummaryInfo> &tSFileSummaryInfoList);
    void ClearSameData(const QList<TSFileSummaryInfo> &srcTSFileSummaryInfoList,QList<TSFileSummaryInfo> &outTSFileSummaryInfoList);
    void AppendTranslation(QList<TSFileSummaryInfo> &tSFileSummaryInfoList,const QString &source,const QString &translation);

//    void CreateFalgEnableList(QTreeWidgetItem *item);
//    void FoEachItemOfChileItem(QTreeWidgetItem *item);
//    void SetTreeWidgetItemFlag(QTreeWidget *treeWidget,bool flag);
//    void SetTreeWidgetItemFlag(QTreeWidgetItem *item,bool flag);
//    bool JudegIsExistFalgEnableList(QTreeWidgetItem* item);

signals:
    void CtrlSetEnabled(const QString &ctrlName,const QString &txtStr,bool isEnabled);
    void AppendPossessLog(const QString &logStr);

public slots:
    void ToolButtonClicked(bool checked);
    void ComboBoxActivated(int index);
    void CheckBoxClicked(bool checked);
    void RadioButtonClicked(bool checked);
    void TreeWidgetItemChanged(QTreeWidgetItem *item, int column);
//    void TreeWidgetItemPressed(QTreeWidgetItem *item, int column);

    void CtrlSetEnabledSlots(const QString &ctrlName,const QString &txtStr,bool isEnabled);
    void AppendPossessLogSlots(const QString &logStr);
};

#endif // TSFILEEXPORTWIN_H
