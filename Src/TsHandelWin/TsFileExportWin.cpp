# pragma execution_character_set("utf-8")
#include "TsFileExportWin.h"
#include "ui_TsFileExportWin.h"

#include <QDebug>
#include "Src/DataManage/DataOperate.h"
#include <QFileInfo>
#include <QDir>
#include <QMenu>
#include "Src/FileHandel/DirOperate.h"
#include <QMessageBox>
#include <QDomDocument>
#include "Src/FileHandel/ExcelHandel.h"
#include <thread>

TsFileExportWin::TsFileExportWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TsFileExportWin)
{
    ui->setupUi(this);
}

TsFileExportWin::~TsFileExportWin()
{
    delete ui;
}

void TsFileExportWin::InitClass()
{
    InitGroupBox();
    InitFrame();
    InitTextEdit();
    InitComboBox();
    InitToolButton();
    InitLabel();
    InitCheckBox();
    InitRadioButton();
    InitTreeWidget();

    GetCreateTSFileName();
    LoadCreateTSFile();
    LoadWinStatus();

    connect(this,static_cast<void (TsFileExportWin::*)(const QString &,const QString &,bool)>(&TsFileExportWin::CtrlSetEnabled), this,&TsFileExportWin::CtrlSetEnabledSlots);
    connect(this,static_cast<void (TsFileExportWin::*)(const QString &)>(&TsFileExportWin::AppendPossessLog), this,&TsFileExportWin::AppendPossessLogSlots);
}

void TsFileExportWin::InitGroupBox()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->groupBox->setFont(font);
    ui->groupBox_2->setFont(font);
    ui->groupBox_3->setFont(font);
    ui->groupBox_4->setFont(font);

    ui->groupBox->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_2->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_3->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_4->layout()->setContentsMargins(10, 0, 10, 5);

    //ui->groupBox_2->setEnabled(false);
}

void TsFileExportWin::InitFrame()
{
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_2->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_3->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_4->layout()->setContentsMargins(0, 0, 0, 0);

    ui->frame->setEnabled(false);
}

void TsFileExportWin::InitTextEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->textEdit->setFont(font);
}

void TsFileExportWin::InitComboBox()
{
    QStringList fileTypeList;fileTypeList.clear();
    fileTypeList<<tr("Excel(.xlsx)");
    ui->comboBox->addItems(fileTypeList);
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    //    connect(ui->comboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,&TsFileCreateWin::ComboBoxActivated);
    connect(ui->comboBox_2,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,&TsFileExportWin::ComboBoxActivated);
}

void TsFileExportWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->ExportTSFileTBtn<<ui->RefreshTSFileListTBtn;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】导出TS文件至选定格式的文件")<<tr("【点击】刷新TS文件列表");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&TsFileExportWin::ToolButtonClicked);
    }
}

void TsFileExportWin::InitLabel()
{
    QList<QLabel*> LableList = this->findChildren<QLabel*>();
    foreach (QLabel *Lable, LableList) {
        if (Lable != nullptr){
            //Lable->setStyleSheet("border: 2px solid blue");
            Lable->setFont(QFont("Microsoft YaHei", 8, QFont::Bold));        //设置字体
            //Lable->setAlignment(Qt::AlignCenter);
        }
    }
}

void TsFileExportWin::InitCheckBox()
{
    ui->checkBox->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));
    ui->checkBox_2->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));
    ui->checkBox_3->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));

    ui->checkBox->setCheckState(Qt::Checked);
    ui->checkBox->setEnabled(false);

    connect(ui->checkBox_2,static_cast<void (QCheckBox::*)(bool)>(&QCheckBox::clicked), this,&TsFileExportWin::CheckBoxClicked);
    connect(ui->checkBox_3,static_cast<void (QCheckBox::*)(bool)>(&QCheckBox::clicked), this,&TsFileExportWin::CheckBoxClicked);
}

void TsFileExportWin::InitRadioButton()
{
    ui->radioButton->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));
    ui->radioButton_2->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));

    //ui->radioButton->setChecked(true);

    connect(ui->radioButton,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked), this,&TsFileExportWin::RadioButtonClicked);
    connect(ui->radioButton_2,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked), this,&TsFileExportWin::RadioButtonClicked);
}

void TsFileExportWin::InitTreeWidget()
{
    // 设置QTreeWidget的列数
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setHeaderLabels({tr("文件名"),tr("类型"),tr("全路径")});
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //自动展开
    ui->treeWidget->setItemsExpandable(true);
    ui->treeWidget->expandAll();
    //ui->treeWidget->hideColumn(2);

    connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&TsFileExportWin::TreeWidgetItemChanged);

    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemPressed), this,&TsFileExportWin::TreeWidgetItemPressed);
}

void TsFileExportWin::LoadCreateTSFile()
{
    QString proFilePath= DataOperate::Instance()->GetProFilePath();
    QFileInfo fileInfo(proFilePath);
    QString tsFileDir=fileInfo.path()+"/language/TsFileCreate";
    qDebug()<<"tsFileDir: "<<tsFileDir;

    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0,tr("TsFileCreate"));
    root->setText(1,tr("dir"));
    root->setText(2,tsFileDir);

    root->setCheckState(0, Qt::Unchecked);

    FoEachDir(tsFileDir,root);

    ui->treeWidget->expandAll();
}

void TsFileExportWin::FoEachDir(QString dirPath, QTreeWidgetItem *root)
{
    // 判断路径是否存在
    QDir dir(dirPath);
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Files |QDir::Dirs | QDir::NoDotAndDotDot );
    QFileInfoList list = dir.entryInfoList();

    int dirCount = list.count();
    for(int index=0; index<dirCount; index++){
        QFileInfo fileInfo = list.at(index);

        QTreeWidgetItem* childEml = new QTreeWidgetItem(root);
        childEml->setText(0,fileInfo.fileName());
        //childEml->setText(1,tr("file"));
        childEml->setText(2,fileInfo.absoluteFilePath());
        childEml->setCheckState(0, Qt::Unchecked);

        if(fileInfo.isFile()){
            childEml->setText(1,tr("file"));
        }
        if (fileInfo.isDir()) {
            childEml->setText(1,tr("dir"));
            FoEachDir(fileInfo.absoluteFilePath(),childEml);
        }
    }
}

void TsFileExportWin::GetCreateTSFileName()
{
    QString proFilePath= DataOperate::Instance()->GetProFilePath();
    QFileInfo fileInfo(proFilePath);
    QString tsFileDir=fileInfo.path()+"/language/TsFileCreate";
    qDebug()<<"tsFileDir: "<<tsFileDir;

    QStringList tsFileNameList;tsFileNameList.clear();
    GetCreateTSFileName(tsFileDir,tsFileNameList);
    ui->comboBox_2->addItems(tsFileNameList);
}

void TsFileExportWin::GetCreateTSFileName(QString dirPath, QStringList &tsFileNameList)
{
    // 判断路径是否存在
    QDir dir(dirPath);
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot );
    QFileInfoList list = dir.entryInfoList();

    int dirCount = list.count();
    if(dirCount<=0)
        return;

    QFileInfo fileInfo = list.at(0);
    dir.setPath(fileInfo.absoluteFilePath());
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot );

    QFileInfoList tsFileList = dir.entryInfoList();
    int tsFileListCount = tsFileList.count();
    for(int index=0; index<tsFileListCount; index++){
        fileInfo=tsFileList.at(index);
        tsFileNameList.append(fileInfo.fileName());
    }
}

void TsFileExportWin::LoadWinStatus()
{
    WorkMode=DataOperate::Instance()->GetIniFileNode(tr("TsFileExportWinGroup"),tr("WorkMode"), tr("0")).toInt();
    ExportItem[0]=DataOperate::Instance()->GetIniFileNode(tr("TsFileExportWinGroup"),tr("ExportItem0"), tr("1")).toInt();
    ExportItem[1]=DataOperate::Instance()->GetIniFileNode(tr("TsFileExportWinGroup"),tr("ExportItem1"), tr("0")).toInt();
    ExportItem[2]=DataOperate::Instance()->GetIniFileNode(tr("TsFileExportWinGroup"),tr("ExportItem2"), tr("0")).toInt();
    SourceFileNameIndex=DataOperate::Instance()->GetIniFileNode(tr("TsFileExportWinGroup"),tr("SourceFileNameIndex"), tr("0")).toInt();

    if(ExportItem[0])
        ui->checkBox->setCheckState(Qt::Checked);
    if(ExportItem[1])
        ui->checkBox_2->setCheckState(Qt::Checked);
    if(ExportItem[2])
        ui->checkBox_3->setCheckState(Qt::Checked);

    if(ui->comboBox_2->count()>SourceFileNameIndex)
        ui->comboBox_2->setCurrentIndex(SourceFileNameIndex);

    if(WorkMode==0){
        ui->frame->setEnabled(false);
        ui->groupBox_2->setEnabled(true);
        ui->radioButton->setChecked(true);

        SourceFileName=ui->comboBox_2->currentText();
        disconnect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged),0,0);
        int topNum = ui->treeWidget->topLevelItemCount();  //返回根节点数量
        for (int index=0;index<topNum ;index++ ) {
            QTreeWidgetItem *rootItem = ui->treeWidget->topLevelItem(index);//获取最高节点
            SetChileItemCheckState(rootItem,SourceFileName);
        }
        connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&TsFileExportWin::TreeWidgetItemChanged);
    }
    else if (WorkMode==1){
        ui->frame->setEnabled(true);
        ui->groupBox_2->setEnabled(false);
        ui->radioButton_2->setChecked(true);
    }
}

bool TsFileExportWin::JudgeChildAllCheck(QTreeWidgetItem *parentItem)
{
    int childEmlCount=parentItem->childCount();
    for (int index=0;index<childEmlCount ;index++ ) {
        QTreeWidgetItem *childEml=parentItem->child(index);
        if(childEml->checkState(0)==Qt::Unchecked)
            return false;
    }
    return true;
}

void TsFileExportWin::SetChileItemCheckState(QTreeWidgetItem *item,bool checked)
{
    int childEmlCount=item->childCount();
    for (int index=0;index<childEmlCount ;index++ ) {
        QTreeWidgetItem *childEml=item->child(index);
        if(WorkMode==0){//汇总
            if(checked==true)
                childEml->setCheckState(0, Qt::Checked);
            else{
                if(childEml->text(0)==SourceFileName)
                    ;//childEml->setCheckState(0, Qt::Unchecked);
                else
                    childEml->setCheckState(0, Qt::Unchecked);
            }
        }else if (WorkMode==1) {
            if(checked==true)
                childEml->setCheckState(0, Qt::Checked);
            else
                childEml->setCheckState(0, Qt::Unchecked);
        }
        SetChileItemCheckState(childEml,checked);
    }
}

void TsFileExportWin::SetChileItemCheckState(QTreeWidgetItem *item, QString itemName)
{
    int childEmlCount=item->childCount();
    for (int index=0;index<childEmlCount ;index++ ) {
        QTreeWidgetItem *pChildEml=item->child(index);
        int pChildEmlCount=pChildEml->childCount();
        for (int tIndex=0;tIndex<pChildEmlCount ;tIndex++ ) {
            QTreeWidgetItem *childEml=pChildEml->child(tIndex);
            if(childEml->text(0)==itemName){
                //childEml->setCheckState(0, Qt::PartiallyChecked);
                childEml->setCheckState(0, Qt::Checked);
                childEml->setFlags(childEml->flags() & ~Qt::ItemIsUserCheckable);
                //childEml->setCheckState(0, Qt::Checked);
            }else {
                childEml->setCheckState(0, Qt::Unchecked);
                childEml->setFlags(childEml->flags() | Qt::ItemIsUserCheckable);
                //childEml->setCheckState(0, Qt::Unchecked);
            }
        }
    }
}

void TsFileExportWin::RestTreeWidgetItemStatus(QTreeWidgetItem *item)
{
    int childEmlCount=item->childCount();
    for (int index=0;index<childEmlCount ;index++ ) {
        QTreeWidgetItem *pChildEml=item->child(index);
        int pChildEmlCount=pChildEml->childCount();
        for (int tIndex=0;tIndex<pChildEmlCount ;tIndex++ ) {
            QTreeWidgetItem *childEml=pChildEml->child(tIndex);
            //childEml->setCheckState(0, Qt::Unchecked);
            childEml->setFlags(childEml->flags() | Qt::ItemIsUserCheckable);
        }
    }
}

void TsFileExportWin::ExportTSFileFun()
{
    //对于汇总的情况不存在选择导出字段这个说法  只有分离才有这种说法
    //由于汇总是导出源文及所有翻译文件译文及源文注解（评论）
    if(ui->radioButton->isChecked()==true){//汇总
        SummaryExportTSFile();
    }
    if(ui->radioButton_2->isChecked()==true){//分离
        SeparateExportTSFile();
    }
}

void TsFileExportWin::SummaryExportTSFile()
{
    QString proFilePath= DataOperate::Instance()->GetProFilePath();
    QFileInfo fileInfo(proFilePath);
    QString tsFileDir=fileInfo.path()+"/language/TsFileExport/";

    QList<SummaryExportInfo> summaryExportInfoList;summaryExportInfoList.clear();

    QTreeWidgetItem *rootItem = ui->treeWidget->topLevelItem(0);//获取最高节点
    int pchildEmelCount=rootItem->childCount();
    for (int index=0;index<pchildEmelCount ;index++ ) {
        QTreeWidgetItem *pchildEmel=rootItem->child(index);
        int childEmleCount=pchildEmel->childCount();
        QList<QString> tSFilePathList;tSFilePathList.clear();
        for (int tIndex=0;tIndex<childEmleCount ;tIndex++ ) {
            QTreeWidgetItem *childEmel=pchildEmel->child(tIndex);
            if(childEmel->checkState(0)==Qt::Checked){
                //exportFileInfoList.append(ExportFileInfo(childEmel->text(2),exportFilePath));
                tSFilePathList.append(childEmel->text(2));
            }
        }
        if(!tSFilePathList.isEmpty()){
            QString exportFilePath=tsFileDir+pchildEmel->text(0)+"/"+pchildEmel->text(0)+"All.xlsx";
            summaryExportInfoList.append(SummaryExportInfo(exportFilePath,tSFilePathList));
        }
    }

    int summaryExportInfoListSize=summaryExportInfoList.size();
    for (int index=0;index<summaryExportInfoListSize ;index++ ) {
        QFileInfo dirFileInfo(summaryExportInfoList.at(index).ExportFilePath);
        //qDebug()<<"dirFileInfo.absolutePath(): "<<dirFileInfo.absolutePath();
        QString fileDir=dirFileInfo.absolutePath();
        DirOperate dirOperate;
        dirOperate.CreateDir(fileDir);
    }

    //处理Ts文件生成EXcel文件

    //HandelSummaryExportInfoList(summaryExportInfoList);

    std::thread t(&TsFileExportWin::HandelSummaryExportInfoList,this,summaryExportInfoList); //使用类成员函数，并传入类指针
    t.detach();
}

void TsFileExportWin::SeparateExportTSFile()
{
    QString proFilePath= DataOperate::Instance()->GetProFilePath();
    QFileInfo fileInfo(proFilePath);
    QString tsFileDir=fileInfo.path()+"/language/TsFileExport/";

    QList<ExportFileInfo> exportFileInfoList;exportFileInfoList.clear();

    QTreeWidgetItem *rootItem = ui->treeWidget->topLevelItem(0);//获取最高节点
    int pchildEmelCount=rootItem->childCount();
    for (int index=0;index<pchildEmelCount ;index++ ) {
        QTreeWidgetItem *pchildEmel=rootItem->child(index);
        int childEmleCount=pchildEmel->childCount();
        for (int tIndex=0;tIndex<childEmleCount ;tIndex++ ) {
            QTreeWidgetItem *childEmel=pchildEmel->child(tIndex);
            if(childEmel->checkState(0)==Qt::Checked){
                QString exportFilePath=tsFileDir+pchildEmel->text(0)+"/"+childEmel->text(0).replace("ts","xlsx",Qt::CaseInsensitive);
                //qDebug()<<"exportFilePath: "<<exportFilePath;
                exportFileInfoList.append(ExportFileInfo(childEmel->text(2),exportFilePath));
            }
        }
    }

    int exportFileListSize=exportFileInfoList.size();
    for (int index=0;index<exportFileListSize ;index++ ) {
        QFileInfo dirFileInfo(exportFileInfoList.at(index).ExportFilePath);
        //qDebug()<<"dirFileInfo.absolutePath(): "<<dirFileInfo.absolutePath();
        QString fileDir=dirFileInfo.absolutePath();
        DirOperate dirOperate;
        dirOperate.CreateDir(fileDir);
    }

    //    HandelExportFileInfoList(exportFileInfoList);
    std::thread t(&TsFileExportWin::HandelExportFileInfoList,this,exportFileInfoList); //使用类成员函数，并传入类指针
    t.detach();
}

void TsFileExportWin::HandelExportFileInfoList(QList<ExportFileInfo> exportFileInfoList)
{
    int handleSuccessCount=0,handleFaileCount=0;
    QString handelResult=tr("成功");
    int exportFileListSize=exportFileInfoList.size();
    emit AppendPossessLog(tr("-------开始处理------"));
    for (int index=0;index<exportFileListSize ;index++ ) {
        QString tSFilePath=exportFileInfoList.at(index).TSFilePath;
        QString exportFileName=exportFileInfoList.at(index).ExportFilePath;
        emit AppendPossessLog(QString(tr("-------正在处理 当前第：%1项 共：%2项------\n")).arg(index+1).arg(exportFileListSize)+tSFilePath+"\n");
        emit AppendPossessLog(QString(tr("-------导出至------\n%1")).arg(exportFileName));
        QFile fileIn(tSFilePath);
        if (!fileIn.open(QFileDevice::ReadOnly)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QDomDocument doc;
        if (!doc.setContent(&fileIn)) {
            QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
            fileIn.close();
            return;
        }

        fileIn.close();

        QList<TSFileInfo> tSFileInfoList;tSFileInfoList.clear();
        QList<TSFileInfo> outTSFileInfoList;outTSFileInfoList.clear();

        QDomElement root = doc.documentElement();//读取根节点
        ForEachTSFileSeparate(&root,tSFileInfoList);
        //去重
        ClearSameData(tSFileInfoList,outTSFileInfoList);
        //tSFileInfoList:原始数据 outTSFileInfoList 去重后数据

        ExcelHandel excelHandel;
        excelHandel.TSFileExportToExcelSeparate(exportFileName,ExportItem,outTSFileInfoList);

        handleSuccessCount++;
        emit AppendPossessLog(QString(tr("-------处理结果【%1】------\n")).arg(handelResult));
    }
    emit AppendPossessLog(tr("-------处理完成------\n"));
    emit AppendPossessLog(QString(tr("-------总计：%1项    成功：%2项    失败：%3项------\n")).arg(exportFileListSize).arg(handleSuccessCount).arg(handleFaileCount));
    emit CtrlSetEnabled(tr("ExportTSFileTBtn"),tr("导出TS文件"),true);
}

void TsFileExportWin::ForEachTSFileSeparate(QDomElement *root, QList<TSFileInfo> &tSFileInfoList)
{
    QDomElement childEle= (*root).firstChildElement();
    while (!childEle.isNull()){
        QString tagName = childEle.toElement().tagName();
        if (tagName.compare("source") == 0){//节点标记查找
            //qDebug()<< childEle.text();//读取节点文本
            QString source="",translation="",comment="";
            source=childEle.text();
            if(!childEle.nextSiblingElement().isNull()&&ExportItem[1]==true)
                translation=childEle.nextSiblingElement().text();
            if(!childEle.nextSiblingElement().nextSiblingElement().isNull()&&ExportItem[2]==true)
                comment=childEle.nextSiblingElement().nextSiblingElement().text();
            tSFileInfoList.append(TSFileInfo(source,translation,comment));
        }
        ForEachTSFileSeparate(&childEle,tSFileInfoList);
        childEle=childEle.nextSiblingElement();
    }
}

void TsFileExportWin::ClearSameData(const QList<TSFileInfo> &srcTSFileInfoList, QList<TSFileInfo> &outTSFileInfoList)
{
    for(auto item : srcTSFileInfoList){
        if (!outTSFileInfoList.contains(item)){
            outTSFileInfoList<<item;
        }
    }
}

void TsFileExportWin::HandelSummaryExportInfoList(QList<SummaryExportInfo> summaryExportInfoList)
{
    int handleSuccessCount=0,handleFaileCount=0;
    QString handelResult=tr("成功");
    int summaryExportInfoListSize=summaryExportInfoList.size();
    emit AppendPossessLog(tr("-------开始处理------"));
    for (int index=0;index<summaryExportInfoListSize ;index++ ) {
        QString exportFileName=summaryExportInfoList.at(index).ExportFilePath;
        QList<QString> tSFilePathList=summaryExportInfoList.at(index).TSFilePathList;
        QList<TSFileSummaryInfo> tSFileSummaryInfoList;tSFileSummaryInfoList.clear();
        QStringList header;header.clear();
        int tSFilePathListSize=tSFilePathList.size();
        emit AppendPossessLog(QString(tr("-------正在处理 当前第：%1大项 共：%2大项------\n")).arg(index+1).arg(summaryExportInfoListSize)+exportFileName+"\n");
        emit AppendPossessLog(QString(tr("-------处理源文注释数为：%1  译文数为：%2---------")).arg(1).arg(tSFilePathListSize-1));
        emit AppendPossessLog(QString(tr("-------处理源文注释开始---------")));
        //先 生成源文注释（评论）
        for (int tIndex=0;tIndex<tSFilePathListSize ;tIndex++ ) {
            QString tSFilePath= tSFilePathList.at(tIndex);
            QFileInfo fileInfo(tSFilePath);
            if(fileInfo.fileName()!=SourceFileName){
                continue;
            }
            header<<QString(tr("源文 %1")).arg(SourceFileName);
            emit AppendPossessLog(QString(tr("-------当前处理源文为：%1---------")).arg(SourceFileName));

            QFile fileIn(tSFilePath);
            if (!fileIn.open(QFileDevice::ReadOnly)) {
                QMessageBox::information(NULL, "提示", "文件打开失败！");
                return;
            }

            QDomDocument doc;
            if (!doc.setContent(&fileIn)) {
                QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
                fileIn.close();
                return;
            }

            fileIn.close();

            QDomElement root = doc.documentElement();//读取根节点
            ForEachTSFileSummarySource(&root,tSFileSummaryInfoList);

        }

        emit AppendPossessLog(QString(tr("-------处理源文注释结束---------")));
        emit AppendPossessLog(QString(tr("-------处理译文开始---------")));

        //如需要去重则这里用 tSFileInfoList去重掉即可
        //不去重则下面直接用一个QList<QString> TranslationList即可 与上面的一一对应遍历追加即可
        //这里去重掉
        //tSFileInfoList=tSFileInfoList.toSet().toList();
        QList<TSFileSummaryInfo> outTSFileInfoList;outTSFileInfoList.clear();
        ClearSameData(tSFileSummaryInfoList,outTSFileInfoList);
        //tSFileSummaryInfoList:原始数据 outTSFileInfoList 去重后数据

        //        int tSFileInfoListSize=tSFileInfoList.size();
        //        int outTSFileInfoListSize=outTSFileInfoList.size();
        //        qDebug()<<"tSFileInfoListSize: "<<tSFileInfoListSize;
        //        qDebug()<<"outTSFileInfoListSize: "<<outTSFileInfoListSize;

        for (int tIndex=0;tIndex<tSFilePathListSize ;tIndex++ ) {//生成译文的
            QString tSFilePath= tSFilePathList.at(tIndex);
            QFileInfo fileInfo(tSFilePath);
            if(fileInfo.fileName()==SourceFileName){
                continue;
            }
            header<<fileInfo.fileName();
            emit AppendPossessLog(QString(tr("-------当前处理译文为：%1---------")).arg(fileInfo.fileName()));

            QFile fileIn(tSFilePath);
            if (!fileIn.open(QFileDevice::ReadOnly)) {
                QMessageBox::information(NULL, "提示", "文件打开失败！");
                return;
            }

            QDomDocument doc;
            if (!doc.setContent(&fileIn)) {
                QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
                fileIn.close();
                return;
            }

            fileIn.close();

            QDomElement root = doc.documentElement();//读取根节点
            ForEachTSFileSummaryTran(&root,outTSFileInfoList);
        }
        header<<QString(tr("注释 %1")).arg(SourceFileName);
        emit AppendPossessLog(QString(tr("-------处理译文结束---------")));

        ExcelHandel excelHandel;
        excelHandel.TSFileExportToExcelSummary(exportFileName,header,outTSFileInfoList);

        handleSuccessCount++;
        emit AppendPossessLog(QString(tr("-------处理结果【%1】------\n")).arg(handelResult));
    }
    emit AppendPossessLog(tr("-------处理完成------\n"));
    emit AppendPossessLog(QString(tr("-------总计：%1项    成功：%2项    失败：%3项------\n")).arg(summaryExportInfoListSize).arg(handleSuccessCount).arg(handleFaileCount));
    emit CtrlSetEnabled(tr("ExportTSFileTBtn"),tr("导出TS文件"),true);
}

void TsFileExportWin::ForEachTSFileSummarySource(QDomElement *root, QList<TSFileSummaryInfo> &tSFileSummaryInfoList)
{
    QDomElement childEle= (*root).firstChildElement();
    while (!childEle.isNull()){
        QString tagName = childEle.toElement().tagName();
        if (tagName.compare("source") == 0){//节点标记查找
            //qDebug()<< childEle.text();//读取节点文本
            QString source="",/*translation="",*/comment="";
            QList<QString> translationList;translationList.clear();
            source=childEle.text();
            //            if(!childEle.nextSiblingElement().isNull())
            //                translation=childEle.nextSiblingElement().text();
            if(!childEle.nextSiblingElement().nextSiblingElement().isNull())
                comment=childEle.nextSiblingElement().nextSiblingElement().text();
            tSFileSummaryInfoList.append(TSFileSummaryInfo(source,translationList,comment));
        }
        ForEachTSFileSummarySource(&childEle,tSFileSummaryInfoList);
        childEle=childEle.nextSiblingElement();
    }
}

void TsFileExportWin::ForEachTSFileSummaryTran(QDomElement *root, QList<TSFileSummaryInfo> &tSFileSummaryInfoList)
{
    QDomElement childEle= (*root).firstChildElement();
    while (!childEle.isNull()){
        QString tagName = childEle.toElement().tagName();
        if (tagName.compare("source") == 0){//节点标记查找
            //qDebug()<< childEle.text();//读取节点文本
            QString source="",translation="";//,comment="";
            source=childEle.text();
            if(!childEle.nextSiblingElement().isNull())
                translation=childEle.nextSiblingElement().text();
            //            if(!childEle.nextSiblingElement().nextSiblingElement().isNull())
            //                comment=childEle.nextSiblingElement().nextSiblingElement().text();
            //tSFileInfoList.append(TSFileInfo(source,translation,comment));
            AppendTranslation(tSFileSummaryInfoList,source,translation);
        }
        ForEachTSFileSummaryTran(&childEle,tSFileSummaryInfoList);
        childEle=childEle.nextSiblingElement();
    }
}
/*
 * function:ClearSameData
 * 功能：清除QList中的相同项
*/
void TsFileExportWin::ClearSameData(const QList<TSFileSummaryInfo> &srcTSFileSummaryInfoList, QList<TSFileSummaryInfo> &outTSFileSummaryInfoList)
{
    for(auto item : srcTSFileSummaryInfoList){
        if (!outTSFileSummaryInfoList.contains(item)){
            outTSFileSummaryInfoList<<item;
        }
    }
}

void TsFileExportWin::AppendTranslation(QList<TSFileSummaryInfo> &tSFileSummaryInfoList, const QString &source, const QString &translation)
{
    int tSFileSummaryInfoListSize=tSFileSummaryInfoList.size();
    for (int index=0;index<tSFileSummaryInfoListSize ;index++ ) {
        if(tSFileSummaryInfoList.at(index).Source==source){
            //tSFileInfoList[index].Translation+=""+translation;
            tSFileSummaryInfoList[index].TranslationList.append(translation);
            break;
        }
    }
}

//void TsFileExportWin::CreateFalgEnableList(QTreeWidgetItem *item)
//{
//    FalgEnableList.clear();
//    //item节点本身
//    FalgEnableList.append(item);

//    //item的 父节点
//    QTreeWidgetItem *parentItem=item->parent();
//    if(parentItem){//遍历 item的 父节点下的子节点 即 item的兄弟节点
//        int parentItemChild=parentItem->childCount();
//        for (int index=0;index<parentItemChild ;index++ ) {
//            QTreeWidgetItem *childEml=parentItem->child(index);
//            if(childEml!=item)
//                FalgEnableList.append(childEml);
//        }
//    }

//    //item 的祖先节点
//    while (parentItem) {//不为空
//        if(JudegIsExistFalgEnableList(parentItem)==false)
//            FalgEnableList.push_back(parentItem);
//        //qDebug()<< QString("%1 %2 %3").arg(parentItem->text(0)).arg(parentItem->text(1)).arg(parentItem->text(2));
//        parentItem=parentItem->parent();//父节点的父节点
//    }

//    //item 的子孙节点
//    FoEachItemOfChileItem(item);
//}

//void TsFileExportWin::FoEachItemOfChileItem(QTreeWidgetItem *item)
//{
//    int childEmlCount=item->childCount();
//    for (int index=0;index<childEmlCount ;index++ ) {
//        QTreeWidgetItem *childEml=item->child(index);
//        FalgEnableList.append(childEml);
//        FoEachItemOfChileItem(childEml);
//    }
//}

//void TsFileExportWin::SetTreeWidgetItemFlag(QTreeWidget *treeWidget,bool flag)
//{
//    int topNum = treeWidget->topLevelItemCount();  //返回根节点数量
//    for (int index=0;index<topNum ;index++ ) {
//        QTreeWidgetItem *rootItem = treeWidget->topLevelItem(index);//获取最高节点
//        if(flag==true){
//            if(JudegIsExistFalgEnableList(rootItem)==true)
//                rootItem->setFlags(Qt::ItemIsEnabled&Qt::ItemIsSelectable);
//            else
//                rootItem->setFlags(rootItem->flags()&(~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable));
//        }else {
//            rootItem->setFlags(Qt::ItemIsEnabled&Qt::ItemIsSelectable);
//        }
//        SetTreeWidgetItemFlag(rootItem,flag);
//    }
//}

//void TsFileExportWin::SetTreeWidgetItemFlag(QTreeWidgetItem *item, bool flag)
//{
//    int childEmlCount=item->childCount();
//    for (int index=0;index<childEmlCount ;index++ ) {
//        QTreeWidgetItem *childEml=item->child(index);
//        if(flag==true){
//            if(JudegIsExistFalgEnableList(childEml)==true)
//                childEml->setFlags(Qt::ItemIsEnabled&Qt::ItemIsSelectable);
//            else
//                childEml->setFlags(childEml->flags()&(~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable));
//        }else {
//            childEml->setFlags(Qt::ItemIsEnabled&Qt::ItemIsSelectable);
//        }
//        SetTreeWidgetItemFlag(childEml,flag);
//    }
//}

//bool TsFileExportWin::JudegIsExistFalgEnableList(QTreeWidgetItem *item)
//{
//    int FalgEnableListSize=FalgEnableList.size();
//    for (int index=0;index<FalgEnableListSize ;index++ ) {
//        if(FalgEnableList.at(index)==item)
//            return true;
//    }
//    return false;
//}

void TsFileExportWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="ExportTSFileTBtn"){
        ui->textEdit->clear();
        ui->ExportTSFileTBtn->setText(tr("处理中..."));
        ui->ExportTSFileTBtn->setEnabled(false);
        ExportTSFileFun();
    }
    else if (ToolButton->objectName()=="RefreshTSFileListTBtn") {
        ui->treeWidget->clear();
        LoadCreateTSFile();
        GetCreateTSFileName();
    }
}

void TsFileExportWin::ComboBoxActivated(int index)
{
    QComboBox *ComboBox = qobject_cast<QComboBox*>(sender());
    qDebug()<<ComboBox->objectName()<<"  "<<ComboBox->currentText();

    if(ComboBox->objectName()=="comboBox_2"){
        SourceFileNameIndex=index;
        SourceFileName=ComboBox->currentText();
        disconnect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged),0,0);
        int topNum = ui->treeWidget->topLevelItemCount();  //返回根节点数量
        for (int index=0;index<topNum ;index++ ) {
            QTreeWidgetItem *rootItem = ui->treeWidget->topLevelItem(index);//获取最高节点
            SetChileItemCheckState(rootItem,SourceFileName);
        }
        connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&TsFileExportWin::TreeWidgetItemChanged);
        DataOperate::Instance()->WriteIniFile(tr("TsFileExportWinGroup"),tr("SourceFileNameIndex"), QString::number(SourceFileNameIndex));
    }
}

void TsFileExportWin::CheckBoxClicked(bool checked)
{
    QCheckBox *CheckBox = qobject_cast<QCheckBox*>(sender());
    qDebug()<<CheckBox->objectName()<<"  "<<CheckBox->text()<<"  checked: "<<checked;
    DataOperate::Instance()->WriteIniFile(tr("TsFileExportWinGroup"),tr("ExportItem0"), QString::number(ExportItem[0]));
    if(CheckBox->objectName()=="checkBox_2"){
        ExportItem[1]=checked;
        DataOperate::Instance()->WriteIniFile(tr("TsFileExportWinGroup"),tr("ExportItem1"), QString::number(ExportItem[1]));
    }
    else if (CheckBox->objectName()=="checkBox_3") {
        ExportItem[2]=checked;
        DataOperate::Instance()->WriteIniFile(tr("TsFileExportWinGroup"),tr("ExportItem2"), QString::number(ExportItem[2]));
    }
}

void TsFileExportWin::RadioButtonClicked(bool checked)
{
    QRadioButton *RadioButton = qobject_cast<QRadioButton*>(sender());
    qDebug()<<RadioButton->objectName()<<"  "<<RadioButton->text();
    if(RadioButton->objectName()=="radioButton"){
        ui->frame->setEnabled(false);
        ui->groupBox_2->setEnabled(true);
        WorkMode=0;
        qDebug()<<"WorkMode=0;";
        SourceFileName=ui->comboBox_2->currentText();
        disconnect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged),0,0);
        int topNum = ui->treeWidget->topLevelItemCount();  //返回根节点数量
        for (int index=0;index<topNum ;index++ ) {
            QTreeWidgetItem *rootItem = ui->treeWidget->topLevelItem(index);//获取最高节点
            SetChileItemCheckState(rootItem,SourceFileName);
        }
        connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&TsFileExportWin::TreeWidgetItemChanged);

    }
    else if (RadioButton->objectName()=="radioButton_2"){
        ui->frame->setEnabled(true);
        ui->groupBox_2->setEnabled(false);
        WorkMode=1;
        qDebug()<<"WorkMode=1;";
        disconnect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged),0,0);
        int topNum = ui->treeWidget->topLevelItemCount();  //返回根节点数量
        for (int index=0;index<topNum ;index++ ) {
            QTreeWidgetItem *rootItem = ui->treeWidget->topLevelItem(index);//获取最高节点
            RestTreeWidgetItemStatus(rootItem);//看情况是否还需要重置其他状态
        }
        connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&TsFileExportWin::TreeWidgetItemChanged);
    }

    DataOperate::Instance()->WriteIniFile(tr("TsFileExportWinGroup"),tr("WorkMode"), QString::number(WorkMode));
}

void TsFileExportWin::TreeWidgetItemChanged(QTreeWidgetItem *item, int column)
{
    //先断掉信号槽连接为了防止在这里面设置设置状态的时候再次多次触发造成判断失效
    disconnect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged),0,0);
    //sender->disconnect(SIGNAL(mySignal()));

    qDebug()<<QString("%1  %2").arg(item->text(0)).arg(item->text(1));
    QTreeWidgetItem *parentItem=item->parent();
    if(parentItem){
        if(JudgeChildAllCheck(parentItem)==true)
            parentItem->setCheckState(0, Qt::Checked);
        else
            parentItem->setCheckState(0, Qt::Unchecked);
    }

    if(item->checkState(0)==Qt::Checked)
        SetChileItemCheckState(item,true);
    else if(item->checkState(0)==Qt::Unchecked)
        SetChileItemCheckState(item,false);

    connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&TsFileExportWin::TreeWidgetItemChanged);
}

void TsFileExportWin::CtrlSetEnabledSlots(const QString &ctrlName, const QString &txtStr, bool isEnabled)
{
    if(ctrlName==tr("ExportTSFileTBtn")){
        ui->ExportTSFileTBtn->setText(txtStr);
        ui->ExportTSFileTBtn->setEnabled(isEnabled);
    }
}

void TsFileExportWin::AppendPossessLogSlots(const QString &logStr)
{
    ui->textEdit->append(logStr);
}

//void TsFileExportWin::TreeWidgetItemPressed(QTreeWidgetItem *item, int column)
//{
//    qDebug()<<QString("%1  %2").arg(item->text(0)).arg(item->text(1));
//    if(qApp->mouseButtons() == Qt::RightButton){// 只针对鼠标右键
//        if(item->text(1)=="dir")
//            return;

//        QTreeWidget* tree = item->treeWidget(); // 获取当前item所在的QTreeWidget
//        // [option] 此处可以添加条件来只针对指定的QTreeWidgetItem来添加右键菜单功能。
//        QMenu* pMenu = new QMenu(tree);
//        QAction* action1 = new QAction(tr("绑定"));
//        QAction* action2 = new QAction(tr("解除"));
//        pMenu->addAction(action1);
//        pMenu->addAction(action2);

//        // 为右键菜单上的QAction创建信号槽，添加对应的功能
//        connect(action1, &QAction::triggered, this,[=]
//        {
//            CreateFalgEnableList(item);
//            SetTreeWidgetItemFlag(tree,true);
//        });
//        connect(action2, &QAction::triggered, this,[=]
//        {
//            SetTreeWidgetItemFlag(tree,false);
//        });
//        //在鼠标右键点击的地方显示菜单
//        pMenu->exec(cursor().pos());

//        //释放内存,若此处不手动释放，则必须等到程序结束时才都能释放
//        QList<QAction*> list = pMenu->actions();
//        foreach (QAction* pAction, list)
//            delete pAction;
//        delete pMenu;
//    }
//}
