#ifndef QMFILEGENERATEWIN_H
#define QMFILEGENERATEWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"
#include "Src/DataManage/DataModel.h"

#include "Src/Common/GlobalCus.h"

class QToolButton;
class QTreeWidgetItem;

namespace Ui {
class QMFileGenerateWin;
}

class QMFileGenerateWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit QMFileGenerateWin(QWidget *parent = nullptr);
    ~QMFileGenerateWin();

private:
    Ui::QMFileGenerateWin *ui;
    QList<QToolButton*>ToolButtonList;

protected:
    void InitClass() override;

public:
    void InitGroupBox();
    void InitFrame();
    void InitTextEdit();
    void InitToolButton();
    void InitTreeWidget();
    void InitEventFilterObj();

    void LoadTranslateTSFile();
    void FoEachDir(QString dirPath,QTreeWidgetItem* root);

    bool JudgeChildAllCheck(QTreeWidgetItem* parentItem);
    void SetChileItemCheckState(QTreeWidgetItem* item,bool checked);

    void GenerateQMFileFun();

    void HandelTRTSFileInfoList(QList<TRTSFileInfo> tRTSFileInfoList);


signals:
    void CtrlSetEnabled(const QString &ctrlName,const QString &txtStr,bool isEnabled);
    void AppendPossessLog(const QString &logStr);

public slots:
    void ToolButtonClicked(bool checked);
    void TreeWidgetItemChanged(QTreeWidgetItem *item, int column);
    void CtrlSetEnabledSlots(const QString &ctrlName,const QString &txtStr,bool isEnabled);
    void AppendPossessLogSlots(const QString &logStr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // QMFILEGENERATEWIN_H
