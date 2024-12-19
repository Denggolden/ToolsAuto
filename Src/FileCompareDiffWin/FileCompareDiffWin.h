#ifndef FILECOMPAREDIFFWIN_H
#define FILECOMPAREDIFFWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

#include "Src/Common/GlobalCus.h"

class QTreeWidgetItem;

//还差临时文件索引路径选择
//还差整理结果文件输出路径选择

namespace Ui {
class FileCompareDiffWin;
}

class FileCompareDiffWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit FileCompareDiffWin(QWidget *parent = nullptr);
    ~FileCompareDiffWin();

private:
    Ui::FileCompareDiffWin *ui;
    QList<QToolButton*>ToolButtonList;

    QString FileOrDirPath1="";
    QString FileOrDirPath2="";

    QString TargetTempDir="";
    QString FilesArranger="";

    //已经废弃
    // bool IsAddNew=false;//是否新增
    // QTreeWidgetItem* ParNode=nullptr;

protected:
    void InitClass() override;

public:
    void InitGroupBox();
    void InitLineEdit();
    void InitToolButton();
    void InitTreeWidget();
    void InitLabel();
    void InitTextEdit();

    void LoadWinStatus();

protected:
    void OpenCompareFun();
    void OpenCompareSynchronousFun();//同步的方法
    void OpenCompareAsynchronousFun();//异步的方法
    void SelectFileOrDirPathFun(QString flag);
    void FilesArrangerFun();
    //异步遍历的方法   遍历构建好的文件树
    void FoEachTree(QTreeWidgetItem* root);

    //同步的方法
    //开始比较第一步（先遍历 FileOrDirPath1 及 FileOrDirPath2）
    void OpenCompareSynchronousStep1New();
    void FoEachDirAndCreateIndex(QString dirPath,QString flag);

    //开始比较第二步（先遍历临时文件夹 建立文件树,并同时开始比较）
    void OpenCompareSynchronousStep2New();
    void FoEachDirTemp();
    void FoEachDirTemp(QString dirPath,QTreeWidgetItem* root);

    //开始比较第三步（遍历临时文件夹 删除临时文件）
    void OpenCompareSynchronousStep3New();
    void FoEachDirAndDeleteIndex(QString dirPath);

    //异步的方法
    //开始比较第一步（先遍历 FileOrDirPath1 及 FileOrDirPath2）
    void OpenCompareAsynchronousStep1New();
    //void FoEachDirAndCreateIndex(QString dirPath,QString flag);

    //开始比较第二步（先遍历临时文件夹 建立文件树,并同时开始比较）
    void OpenCompareAsynchronousStep2New();
    void FoEachDirTempAsyn();
    void FoEachDirTempAsyn(QString dirPath,QTreeWidgetItem* root,int sleepCount);
    bool JudgeTowFileIsConsistent(const QString &filePath1,const QString & filePath2);//判断两个文件是否一样
    QByteArray GetFileMd5(const QString &fileName);

    //开始比较第三步（遍历临时文件夹 删除临时文件）
    void OpenCompareAsynchronousStep3New();
    //void FoEachDirAndDeleteIndex(QString dirPath);

    void SetTBtnIsEnable(bool enable);

signals:
    void SetTBtnIsEnableSignal(bool enable);
    void AppendPossessLog(const QString &logStr);
    void StartCreateTree(QTreeWidgetItem* root,QString name,QString type,QString path);
    void StartCreateTreeChild(QTreeWidgetItem* root,QTreeWidgetItem *childEml,QString name,QString type,QString path,QString Or,QString dir1IsExist,QString dir2IsExist,QString result);
    void CreateTreeCmopete();
    void AssestTreeItem(QTreeWidgetItem *itemEml);

public slots:
    void ToolButtonClicked(bool checked);
    void SetTBtnIsEnableSlots(bool enable);
    void AppendPossessLogSlots(const QString &logStr);
    void StartCreateTreeSlots(QTreeWidgetItem* root,QString name,QString type,QString path);
    void StartCreateTreeChildSlots(QTreeWidgetItem* root,QTreeWidgetItem *childEml,QString name,QString type,QString path,QString Or,QString dir1IsExist,QString dir2IsExist,QString result);
    void CreateTreeCmopeteSlots();
    void AssestTreeItemSlots(QTreeWidgetItem *itemEml);
    void TreeWidgetItemChanged(QTreeWidgetItem *item, int column);
    void TreeWidgetItemClicked(QTreeWidgetItem *item, int column);
    void TreeWidgetItemPressed(QTreeWidgetItem *item, int column);
};

#endif // FILECOMPAREDIFFWIN_H
