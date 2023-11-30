# pragma execution_character_set("utf-8")
#include "TsFileTranslateWin.h"
#include "ui_TsFileTranslateWin.h"

#include <QDebug>
#include "Src/DataManage/DataOperate.h"
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include "Src/FileHandel/DirOperate.h"
#include <QMessageBox>
#include <QDomDocument>
#include "Src/FileHandel/ExcelHandel.h"
#include <thread>

TsFileTranslateWin::TsFileTranslateWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TsFileTranslateWin)
{
    ui->setupUi(this);
}

TsFileTranslateWin::~TsFileTranslateWin()
{
    delete ui;
}

void TsFileTranslateWin::InitClass()
{
    InitGroupBox();
    InitFrame();
    InitLineEdit();
    InitTextEdit();
    InitToolButton();
    InitTreeWidget();
    InitLabel();
    InitCheckBox();
    InitTabWidget();

    LoadCreateTSFile();

    LoadWinStatus();

    connect(this,static_cast<void (TsFileTranslateWin::*)(const QString &,const QString &,bool)>(&TsFileTranslateWin::CtrlSetEnabled), this,&TsFileTranslateWin::CtrlSetEnabledSlots);
    connect(this,static_cast<void (TsFileTranslateWin::*)(const QString &,QString)>(&TsFileTranslateWin::AppendPossessLog), this,&TsFileTranslateWin::AppendPossessLogSlots);
}

void TsFileTranslateWin::InitGroupBox()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->groupBox->setFont(font);
    ui->groupBox_3->setFont(font);
    ui->groupBox_4->setFont(font);

    ui->groupBox_2->setFont(font);
    ui->groupBox_5->setFont(font);

    ui->groupBox->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_3->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_4->layout()->setContentsMargins(10, 0, 10, 5);

    ui->groupBox_2->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_5->layout()->setContentsMargins(10, 0, 10, 5);
}

void TsFileTranslateWin::InitFrame()
{
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_2->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_3->layout()->setContentsMargins(0, 0, 0, 0);
}

void TsFileTranslateWin::InitLineEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);
    ui->lineEdit_3->setFont(font);

    ui->lineEdit->setPlaceholderText(tr("翻译清单文件路径"));
    ui->lineEdit_2->setPlaceholderText(tr("原始翻译清单文件路径"));
    ui->lineEdit_3->setPlaceholderText(tr("新翻译清单文件路径"));
}

void TsFileTranslateWin::InitTextEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->textEdit->setFont(font);
    ui->textEdit_2->setFont(font);
}

void TsFileTranslateWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->SelectTRListTBtn<<ui->TRTSFileTBtn<<ui->RefreshTSFileListTBtn<<ui->SelectNewFileTBtn<<ui->SelectOldFileTBtn
                 <<ui->MergeTRListFileTBtn;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】选择翻译清单文件")<<tr("【点击】生成翻译后的TS文件")<<tr("【点击】刷新TS文件列表")
              <<tr("【点击】选择翻译当前清单文件")<<tr("【点击】选择翻译原始的清单文件")<<tr("【点击】合并新旧翻译清单文件");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&TsFileTranslateWin::ToolButtonClicked);
    }
}

void TsFileTranslateWin::InitTreeWidget()
{
    // 设置QTreeWidget的列数
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setHeaderLabels({tr("文件名"),tr("类型"),tr("全路径")});
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //自动展开
    ui->treeWidget->setItemsExpandable(true);
    ui->treeWidget->expandAll();
    //ui->treeWidget->hideColumn(2);

    connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&TsFileTranslateWin::TreeWidgetItemChanged);

    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemPressed), this,&TsFileExportWin::TreeWidgetItemPressed);
}

void TsFileTranslateWin::InitLabel()
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

void TsFileTranslateWin::InitCheckBox()
{
    ui->checkBox->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));
    ui->checkBox_2->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));

    ui->checkBox->setCheckState(Qt::Checked);
    ui->checkBox->setEnabled(false);

    //connect(ui->checkBox,static_cast<void (QCheckBox::*)(bool)>(&QCheckBox::clicked), this,&TsFileTranslateWin::CheckBoxClicked);
    connect(ui->checkBox_2,static_cast<void (QCheckBox::*)(bool)>(&QCheckBox::clicked), this,&TsFileTranslateWin::CheckBoxClicked);
}

void TsFileTranslateWin::InitTabWidget()
{
    ui->tabWidget->widget(0)->layout()->setContentsMargins(0, 10, 0, 0);
}

void TsFileTranslateWin::LoadWinStatus()
{
    TRListFilePath=DataOperate::Instance()->GetIniFileNode(tr("TsFileTranslateWinGroup"),tr("TRListFilePath"), tr(""));
    NewTRListFilePath=DataOperate::Instance()->GetIniFileNode(tr("TsFileTranslateWinGroup"),tr("NewTRListFilePath"), tr(""));
    OldTRListFilePath=DataOperate::Instance()->GetIniFileNode(tr("TsFileTranslateWinGroup"),tr("OldTRListFilePath"), tr(""));
    TranslateItem[0]=DataOperate::Instance()->GetIniFileNode(tr("TsFileTranslateWinGroup"),tr("TranslateItem0"), tr("1")).toInt();
    TranslateItem[1]=DataOperate::Instance()->GetIniFileNode(tr("TsFileTranslateWinGroup"),tr("TranslateItem1"), tr("0")).toInt();

    ui->lineEdit->setText(TRListFilePath);
    ui->lineEdit_2->setText(OldTRListFilePath);
    ui->lineEdit_3->setText(NewTRListFilePath);

    if(TranslateItem[0]==true)
        ui->checkBox->setCheckState(Qt::Checked);
    if(TranslateItem[1]==true)
        ui->checkBox_2->setCheckState(Qt::Checked);
}

void TsFileTranslateWin::LoadCreateTSFile()
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

void TsFileTranslateWin::FoEachDir(QString dirPath, QTreeWidgetItem *root)
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

bool TsFileTranslateWin::JudgeChildAllCheck(QTreeWidgetItem *parentItem)
{
    int childEmlCount=parentItem->childCount();
    for (int index=0;index<childEmlCount ;index++ ) {
        QTreeWidgetItem *childEml=parentItem->child(index);
        if(childEml->checkState(0)==Qt::Unchecked)
            return false;
    }
    return true;
}

void TsFileTranslateWin::SetChileItemCheckState(QTreeWidgetItem *item, bool checked)
{
    int childEmlCount=item->childCount();
    for (int index=0;index<childEmlCount ;index++ ) {
        QTreeWidgetItem *childEml=item->child(index);
        if(checked==true)
            childEml->setCheckState(0, Qt::Checked);
        else
            childEml->setCheckState(0, Qt::Unchecked);
        SetChileItemCheckState(childEml,checked);
    }
}

void TsFileTranslateWin::SelectTRListFun()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open a file."),
                                                    QCoreApplication::applicationDirPath(),
                                                    tr("Excel Files(*.xlsx)"));
    if (fileName.isEmpty())
        return;

    TRListFilePath=fileName;
    ui->lineEdit->setText(TRListFilePath);
    DataOperate::Instance()->WriteIniFile(tr("TsFileTranslateWinGroup"),tr("TRListFilePath"),TRListFilePath);
}

void TsFileTranslateWin::TRTSFileFun()
{
    QString proFilePath= DataOperate::Instance()->GetProFilePath();
    QFileInfo fileInfo(proFilePath);
    QString tsFileDir=fileInfo.path()+"/language/TsFileTranslate/";

    QList<TRFileInfo> tRFileInfoList;tRFileInfoList.clear();

    QTreeWidgetItem *rootItem = ui->treeWidget->topLevelItem(0);//获取最高节点
    int pchildEmelCount=rootItem->childCount();
    for (int index=0;index<pchildEmelCount ;index++ ) {
        QTreeWidgetItem *pchildEmel=rootItem->child(index);
        int childEmleCount=pchildEmel->childCount();
        for (int tIndex=0;tIndex<childEmleCount ;tIndex++ ) {
            QTreeWidgetItem *childEmel=pchildEmel->child(tIndex);
            if(childEmel->checkState(0)==Qt::Checked){
                QString outTSFilePath=tsFileDir+pchildEmel->text(0)+"/"+childEmel->text(0);
                //qDebug()<<"exportFilePath: "<<exportFilePath;
                tRFileInfoList.append(TRFileInfo(childEmel->text(2),outTSFilePath));
            }
        }
    }

    int tRFileInfoListSize=tRFileInfoList.size();
    for (int index=0;index<tRFileInfoListSize ;index++ ) {
        QFileInfo dirFileInfo(tRFileInfoList.at(index).OutTSFilePath);
        //qDebug()<<"dirFileInfo.absolutePath(): "<<dirFileInfo.absolutePath();
        QString fileDir=dirFileInfo.absolutePath();
        DirOperate dirOperate;
        dirOperate.CreateDir(fileDir);
    }

    //开始用翻译清单文件+ SrcTSFilePath 来生成 OutTSFilePath
    //HandelTRFileInfoList(tRFileInfoList);
    std::thread t(&TsFileTranslateWin::HandelTRFileInfoList,this,tRFileInfoList); //使用类成员函数，并传入类指针
    t.detach();
}

void TsFileTranslateWin::SelectNewFileFun()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open a file."),
                                                    QCoreApplication::applicationDirPath(),
                                                    tr("Excel Files(*.xlsx)"));
    if (fileName.isEmpty())
        return;

    NewTRListFilePath=fileName;
    ui->lineEdit_3->setText(NewTRListFilePath);
    DataOperate::Instance()->WriteIniFile(tr("TsFileTranslateWinGroup"),tr("NewTRListFilePath"),NewTRListFilePath);
}

void TsFileTranslateWin::SelectOldFileFun()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open a file."),
                                                    QCoreApplication::applicationDirPath(),
                                                    tr("Excel Files(*.xlsx)"));
    if (fileName.isEmpty())
        return;

    OldTRListFilePath=fileName;
    ui->lineEdit_2->setText(OldTRListFilePath);
    DataOperate::Instance()->WriteIniFile(tr("TsFileTranslateWinGroup"),tr("OldTRListFilePath"),OldTRListFilePath);
}

void TsFileTranslateWin::MergeTRListFileFun()
{ 
    emit AppendPossessLog(tr("-------开始处理------"),"textEdit_2");
    emit AppendPossessLog(QString(tr("-------正在处理 当前第：%1项 共：%2项------\n")).arg(1).arg(1)+NewTRListFilePath+"\n","textEdit_2");
    emit AppendPossessLog(QString(tr("-------从：%1\n处理至------\n%2")).arg(OldTRListFilePath).arg(NewTRListFilePath),"textEdit_2");
    ExcelHandel excelHandel;
    excelHandel.MergeTRListFile(NewTRListFilePath,OldTRListFilePath);
    emit AppendPossessLog(QString(tr("-------处理结果【%1】------\n")).arg(tr("成功")),"textEdit_2");
    emit AppendPossessLog(tr("-------处理完成------\n"),"textEdit_2");
    emit AppendPossessLog(QString(tr("-------总计：%1项    成功：%2项    失败：%3项------\n")).arg(1).arg(1).arg(0),"textEdit_2");
    emit CtrlSetEnabled(tr("MergeTRListFileTBtn"),tr("合并翻译清单文件"),true);
}

void TsFileTranslateWin::HandelTRFileInfoList(QList<TRFileInfo> tRFileInfoList)
{
    int handleSuccessCount=0,handleFaileCount=0;
    QString handelResult=tr("成功");
    int tRFileInfoListSize=tRFileInfoList.size();
    emit AppendPossessLog(tr("-------开始处理------"),"textEdit");
    for (int index=0;index<tRFileInfoListSize ;index++ ) {
        QString srcTSFilePath=tRFileInfoList.at(index).SrcTSFilePath;
        QString outTSFilePath=tRFileInfoList.at(index).OutTSFilePath;
        QFileInfo fileInfo(srcTSFilePath);
        QString languageType=fileInfo.fileName();
        emit AppendPossessLog(QString(tr("-------正在处理 当前第：%1项 共：%2项------\n")).arg(index+1).arg(tRFileInfoListSize)+srcTSFilePath+"\n","textEdit");
        emit AppendPossessLog(QString(tr("-------翻译至------\n%1")).arg(outTSFilePath),"textEdit");

        // 打开文件
        QFile fileIn(srcTSFilePath);
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

        TRFileInfoList.clear();
        ExcelHandel excelHandel;
        excelHandel.LoadTRListFile(TRListFilePath,languageType,TRFileInfoList);

        CreateTranslateFile(&root);

        QFile fileOut(outTSFilePath);
        if (!fileOut.open(QFile::WriteOnly | QFile::Truncate)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QTextStream ts(&fileOut);
        //ts.reset();
        ts.setCodec("utf-8");
        //doc.save(ts,2,QDomNode::EncodingFromTextStream);
        doc.save(ts,2);
        fileOut.close();

        handleSuccessCount++;
        emit AppendPossessLog(QString(tr("-------处理结果【%1】------\n")).arg(handelResult),"textEdit");
    }

    emit AppendPossessLog(tr("-------处理完成------\n"),"textEdit");
    emit AppendPossessLog(QString(tr("-------总计：%1项    成功：%2项    失败：%3项------\n")).arg(tRFileInfoListSize).arg(handleSuccessCount).arg(handleFaileCount),"textEdit");
    emit CtrlSetEnabled(tr("TRTSFileTBtn"),tr("翻译TS文件"),true);
}

void TsFileTranslateWin::CreateTranslateFile(QDomElement *root)
{
    QDomElement childEle= (*root).firstChildElement();
    while (!childEle.isNull()){
        QString tagName = childEle.toElement().tagName();
        if (tagName.compare("translation") == 0){
            //qDebug()<< childEle.text();
            QString source=childEle.previousSiblingElement().text();
            //与翻译文件源文匹配   返回需要替换的翻译文本即可
            TRListRetInfo tRListRetInfo=FindTRFileInfoList(source);
            QDomDocument doc;

            //<translation>
            QDomText newText = doc.createTextNode(tRListRetInfo.Translation);
            childEle.appendChild(newText);
            if(tRListRetInfo.Translation!=""){
                //修改完成后把 type="unfinished" 属性给去掉
                childEle.removeAttribute("type");
            }

            //<translatorcomment>
            //看情况是否需要添加评论（注释）
            if(TranslateItem[1]){//tRListRetInfo.Comment 看情况是否需要判空
                QDomElement newEle=doc.createElement("translatorcomment");
                QDomText newEleText = doc.createTextNode(tRListRetInfo.Comment);
                newEle.appendChild(newEleText);
                (*root).appendChild(newEle);
            }
        }
        CreateTranslateFile(&childEle);
        childEle=childEle.nextSiblingElement();
    }
}

TRListRetInfo TsFileTranslateWin::FindTRFileInfoList(QString source)
{
    TRListRetInfo tRListRetInfo;
    int TRFileInfoListSize=TRFileInfoList.size();
    for (int index=0;index<TRFileInfoListSize ;index++ ) {
        if(TRFileInfoList.at(index).Source==source){
            tRListRetInfo=TRListRetInfo(TRFileInfoList.at(index).Translation,TRFileInfoList.at(index).Comment);
            break;
        }
    }
    return tRListRetInfo;
}

void TsFileTranslateWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="SelectTRListTBtn"){
        SelectTRListFun();
    }
    else if (ToolButton->objectName()=="TRTSFileTBtn") {
        ui->textEdit->clear();
        ui->TRTSFileTBtn->setText(tr("处理中..."));
        ui->TRTSFileTBtn->setEnabled(false);
        TRTSFileFun();
    }
    else if (ToolButton->objectName()=="RefreshTSFileListTBtn") {
        ui->treeWidget->clear();
        LoadCreateTSFile();
    }

    else if (ToolButton->objectName()=="SelectNewFileTBtn") {
        SelectNewFileFun();
    }
    else if (ToolButton->objectName()=="SelectOldFileTBtn") {
        SelectOldFileFun();
    }
    else if (ToolButton->objectName()=="MergeTRListFileTBtn") {
        ui->textEdit_2->clear();
        ui->MergeTRListFileTBtn->setText(tr("处理中..."));
        ui->MergeTRListFileTBtn->setEnabled(false);
        MergeTRListFileFun();
    }
}

void TsFileTranslateWin::TreeWidgetItemChanged(QTreeWidgetItem *item, int column)
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

    connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&TsFileTranslateWin::TreeWidgetItemChanged);
}

void TsFileTranslateWin::CheckBoxClicked(bool checked)
{
    QCheckBox *CheckBox = qobject_cast<QCheckBox*>(sender());
    qDebug()<<CheckBox->objectName()<<"  "<<CheckBox->text()<<"  checked: "<<checked;
    DataOperate::Instance()->WriteIniFile(tr("TsFileTranslateWinGroup"),tr("TranslateItem0"), QString::number(TranslateItem[0]));
    if(CheckBox->objectName()=="checkBox_2"){
        TranslateItem[1]=checked;
        DataOperate::Instance()->WriteIniFile(tr("TsFileTranslateWinGroup"),tr("TranslateItem1"), QString::number(TranslateItem[1]));
    }
}

void TsFileTranslateWin::CtrlSetEnabledSlots(const QString &ctrlName, const QString &txtStr, bool isEnabled)
{
    if(ctrlName==tr("TRTSFileTBtn")){
        ui->TRTSFileTBtn->setText(txtStr);
        ui->TRTSFileTBtn->setEnabled(isEnabled);
    }
    else if (ctrlName==tr("MergeTRListFileTBtn")) {
        ui->MergeTRListFileTBtn->setText(txtStr);
        ui->MergeTRListFileTBtn->setEnabled(isEnabled);
    }
}

void TsFileTranslateWin::AppendPossessLogSlots(const QString &logStr,QString txtEditName)
{
    if(txtEditName=="textEdit")
        ui->textEdit->append(logStr);
    else if (txtEditName=="textEdit_2")
        ui->textEdit_2->append(logStr);
}
