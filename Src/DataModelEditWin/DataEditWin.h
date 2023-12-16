#ifndef DATAEDITWIN_H
#define DATAEDITWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"
#include "Src/DataManage/DataModel.h"

class QToolButton;
class QTableWidget;

namespace Ui {
class DataEditWin;
}

class DataEditWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit DataEditWin(QWidget *parent = nullptr);
    ~DataEditWin();

private:
    Ui::DataEditWin *ui;
    QList<QToolButton*>ToolButtonList;
    QList<ModListInfo> ModListInfoList;
    QList<ModDataInfo> ModDataInfoList;

    int CurModListInfoListIndex=-1;

    int CurRowIndex=-1;
    int CurColIndex=-1;

protected:
    void InitClass() override;

public:
    void InitGroupBox();
    void InitFrame();
    void InitComboBox();
    void InitToolButton();
    void InitLabel();
    void InitTableWidget();
    void InitTableWidget(QTableWidget *tableWidget,int flag);

    void LoadModTemplate();
    void LoadWinData();

    void SetTableWidgetHader(QTableWidget *tableWidget,const QStringList &header);
    void ResetTableWidget1Data(QTableWidget *tableWidget,const QStringList &header);
    void SetTableWidget2Data(QTableWidget *tableWidget,const QStringList &header,const QString &modName);
    void GetModData(const QString &modName,const int &modFieldCount,QList<ModDataInfo> &modDataInfoList);

    void AddDataToMod(const QString &modName,const int &iDCount,QList<QString> &fieldList);
    void AddDataToTableWidget2(QTableWidget *tableWidget,const int &iDCount,QList<QString> &fieldList);
    void DeleteDataToMod(const QString &modName,const QString &dataId);
    void ModifyDataToMod(const QString &modName,const QString &dataId,const QList<QString> &modDataList);


    void PoptableWidget1Menu();
    void PoptableWidget2Menu();

    void AddDataFun();
    void PasteDataFun();

    void DeleteDataFun(int curRowIndex);
    void ModifyDataFun(int curRowIndex);


public slots:
    void ComboBoxActivated(int index);
    void ToolButtonClicked(bool checked);
    void ShowTableWidgetMenuSlots(const QPoint &pos);
    void OnClickedPopMenu();
};

#endif // DATAEDITWIN_H
