#ifndef MODELEDITWIN_H
#define MODELEDITWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"
#include "Src/DataManage/DataModel.h"

#include "Src/Common/GlobalCus.h"

class QTableWidget;
class QToolButton;
class QDomElement;

namespace Ui {
class ModelEditWin;
}

class ModelEditWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit ModelEditWin(QWidget *parent = nullptr);
    ~ModelEditWin();

private:
    Ui::ModelEditWin *ui;
    QList<QToolButton*>ToolButtonList;
    QList<ModListInfo> ModListInfoList;
    int CurRowIndex=-1;
    int CurColIndex=-1;

protected:
    void InitClass() override;

public:
    void InitGroupBox();
    void InitFrame();
    void InitLineEdit();
    void InitToolButton();
    void InitLabel();
    void InitTableWidget();
    void InitTableWidget(QTableWidget *tableWidget,int flag);

    void CreateModFieldFun();
    void SaveModFun();

    void LoadModTemplate();
    void UpDateModListTableWidget(const QString &modName,const int &modFieldCount,const QList<QString> &fieldList);

    void AddDataTemplate(const QString &modName, const int &modFieldCount, const QList<QString> &fieldList);//添加模板
    void DeleteDataTemplate(const QString &modName);
    void AddFieldDataTemplate(const QString &modName, const int &modFieldCount,const QString &attrValue);
    void EditFieldDataTemplate(const ModListInfo &preModListInfo,const ModListInfo &curModListInfo);
    void DeleteDataFieldTemplate(const QString &modName,const QString &fieldKey);

    void ClearModFieldTableWidget();
    void RefreshModFieldTableWidget(const QString &modName,const int &modFieldCount,const QList<QString> &fieldList);

    void PoptableWidget1Menu();
    void PoptableWidget2Menu();

    void PasteFieldFun();
    void AddFieldFun(int curFieldIndex);
    void DeleteFieldFun(int curFieldIndex);
    void RedesignFun();

    void DeleteModFun(int curRowIndex);
    void ModifyAddFieldFun(int curRowIndex);
    void ModifyEditFieldFun(int curRowIndex);
    void ModifyDeleteFieldFun(int curRowIndex,int curColIndex);

    void ModifyXML();
    void ModifyXML(QDomElement *root);

public slots:
    void ToolButtonClicked(bool checked);
    void ShowTableWidgetMenuSlots(const QPoint &pos);
    void OnClickedPopMenu();
};

#endif // MODELEDITWIN_H
