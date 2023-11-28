#ifndef TSFILECREATEWIN_H
#define TSFILECREATEWIN_H

//#include <QWidget>
#include "Common/WidgetBase.h"

typedef struct CommandInfo{
    CommandInfo(){};
    CommandInfo(QString command,QString tSFilePath){Command=command;TSFilePath=tSFilePath;};
    QString Command;
    QString TSFilePath;
}CommandInfo;

class QToolButton;

namespace Ui {
class TsFileCreateWin;
}

class TsFileCreateWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit TsFileCreateWin(QWidget *parent = nullptr);
    ~TsFileCreateWin();

private:
    Ui::TsFileCreateWin *ui;
    QList<QToolButton*>ToolButtonList;

    QList<CommandInfo> CommandInfoList;

    int KitSelectIndex=0;
    int TSFileSelectIndex=0;
    int TSFileNameCount=0;

protected:
    void InitClass() override;

public:
    void InitGroupBox();
    void InitFrame();
    void InitLineEdit();
    void InitTextEdit();
    void InitComboBox();
    void InitToolButton();
    void InitLabel();

    void LoadWinData();

public:
    void SelectKitDirFun();
    void SelectProFileFun();
    void GenerateTSFileFun();
    void AppendItemFun();
    void DeleteCurItemFun();

    void ForEachDirKit(QString kitDir,QStringList &kitList);

    void HandelCommandList();

signals:
    void CtrlSetEnabled(const QString &ctrlName,const QString &txtStr,bool isEnabled);
    void AppendPossessLog(const QString &logStr);

public slots:
    void ToolButtonClicked(bool checked);
    void ComboBoxActivated(int index);
    void CtrlSetEnabledSlots(const QString &ctrlName,const QString &txtStr,bool isEnabled);
    void AppendPossessLogSlots(const QString &logStr);
};

#endif // TSFILECREATEWIN_H
