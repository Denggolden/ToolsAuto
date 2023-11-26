# pragma execution_character_set("utf-8")
#include "QMFileGenerateWin.h"
#include "ui_QMFileGenerateWin.h"

#include <QDebug>
#include "DataManage/DataOperate.h"
#include <QFileInfo>
#include <QDir>
#include "FileHandel/DirOperate.h"
#include <QProcess>
#include <thread>

QMFileGenerateWin::QMFileGenerateWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QMFileGenerateWin)
{
    ui->setupUi(this);

    InitGroupBox();
    InitFrame();
    InitTextEdit();
    InitToolButton();
    InitTreeWidget();

    LoadTranslateTSFile();

    connect(this,static_cast<void (QMFileGenerateWin::*)(const QString &,const QString &,bool)>(&QMFileGenerateWin::CtrlSetEnabled), this,&QMFileGenerateWin::CtrlSetEnabledSlots);
    connect(this,static_cast<void (QMFileGenerateWin::*)(const QString &)>(&QMFileGenerateWin::AppendPossessLog), this,&QMFileGenerateWin::AppendPossessLogSlots);

}

QMFileGenerateWin::~QMFileGenerateWin()
{
    delete ui;
}

void QMFileGenerateWin::InitGroupBox()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->groupBox->setFont(font);
    ui->groupBox_2->setFont(font);

    ui->groupBox->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_2->layout()->setContentsMargins(10, 0, 10, 5);
}

void QMFileGenerateWin::InitFrame()
{
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);
}

void QMFileGenerateWin::InitTextEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->textEdit->setFont(font);
}

void QMFileGenerateWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->GenerateQMFileTBtn<<ui->RefreshTSFileListTBtn;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】生成指定TS文件的QM文件")<<tr("【点击】刷新翻译的TS文件列表");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&QMFileGenerateWin::ToolButtonClicked);
    }
}

void QMFileGenerateWin::InitTreeWidget()
{
    // 设置QTreeWidget的列数
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setHeaderLabels({tr("文件名"),tr("类型"),tr("全路径")});
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //自动展开
    ui->treeWidget->setItemsExpandable(true);
    ui->treeWidget->expandAll();
    //ui->treeWidget->hideColumn(2);

    connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&QMFileGenerateWin::TreeWidgetItemChanged);

    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemPressed), this,&QMFileGenerateWin::TreeWidgetItemPressed);
}

void QMFileGenerateWin::LoadTranslateTSFile()
{
    QString proFilePath= DataOperate::Instance()->GetProFilePath();
    QFileInfo fileInfo(proFilePath);
    QString tsFileDir=fileInfo.path()+"/language/TsFileTranslate";
    qDebug()<<"tsFileDir: "<<tsFileDir;

    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0,tr("TsFileTranslate"));
    root->setText(1,tr("dir"));
    root->setText(2,tsFileDir);

    root->setCheckState(0, Qt::Unchecked);

    FoEachDir(tsFileDir,root);

    ui->treeWidget->expandAll();
}

void QMFileGenerateWin::FoEachDir(QString dirPath, QTreeWidgetItem *root)
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

bool QMFileGenerateWin::JudgeChildAllCheck(QTreeWidgetItem *parentItem)
{
    int childEmlCount=parentItem->childCount();
    for (int index=0;index<childEmlCount ;index++ ) {
        QTreeWidgetItem *childEml=parentItem->child(index);
        if(childEml->checkState(0)==Qt::Unchecked)
            return false;
    }
    return true;
}

void QMFileGenerateWin::SetChileItemCheckState(QTreeWidgetItem *item, bool checked)
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

void QMFileGenerateWin::GenerateQMFileFun()
{
    QString kitDir= DataOperate::Instance()->GetKitDir();
    QString proFilePath= DataOperate::Instance()->GetProFilePath();
    QFileInfo fileInfo(proFilePath);
    QString QmFileDir=fileInfo.path()+"/language/QMFileGenerate/";

    QList<TRTSFileInfo> tRTSFileInfoList;tRTSFileInfoList.clear();

    QTreeWidgetItem *rootItem = ui->treeWidget->topLevelItem(0);//获取最高节点
    int pchildEmelCount=rootItem->childCount();
    for (int index=0;index<pchildEmelCount ;index++ ) {
        QTreeWidgetItem *pchildEmel=rootItem->child(index);
        int childEmleCount=pchildEmel->childCount();
        for (int tIndex=0;tIndex<childEmleCount ;tIndex++ ) {
            QTreeWidgetItem *childEmel=pchildEmel->child(tIndex);
            if(childEmel->checkState(0)==Qt::Checked){
                QString kitType=pchildEmel->text(0);
                QString srcTRTSFilePath=childEmel->text(2);
                QString outQMFilePath=QmFileDir+kitType+"/"+childEmel->text(0).replace("ts","qm",Qt::CaseInsensitive);

                QString cmd=kitDir+"/"+kitType+"/bin/lrelease "
                        +srcTRTSFilePath
                        +" -qm "+outQMFilePath;
                tRTSFileInfoList.append(TRTSFileInfo(srcTRTSFilePath,outQMFilePath,cmd));
            }
        }
    }

    int tRTSFileInfoListSize=tRTSFileInfoList.size();
    for (int index=0;index<tRTSFileInfoListSize ;index++ ) {
        QFileInfo dirFileInfo(tRTSFileInfoList.at(index).OutQMFilePath);
        //qDebug()<<"dirFileInfo.absolutePath(): "<<dirFileInfo.absolutePath();
        QString fileDir=dirFileInfo.absolutePath();
        DirOperate dirOperate;
        dirOperate.CreateDir(fileDir);
        //qDebug()<<"cmd"<< tRTSFileInfoList.at(index).Command;
    }

    //HandelTRTSFileInfoList(tRTSFileInfoList);
    std::thread t(&QMFileGenerateWin::HandelTRTSFileInfoList,this,tRTSFileInfoList); //使用类成员函数，并传入类指针
    t.detach();
}

void QMFileGenerateWin::HandelTRTSFileInfoList(QList<TRTSFileInfo> tRTSFileInfoList)
{
    int handleSuccessCount=0,handleFaileCount=0;
    QString handelResult="";
    int tRTSFileInfoListSize=tRTSFileInfoList.size();
    emit AppendPossessLog(tr("-------开始处理------"));
    for (int index=0;index<tRTSFileInfoListSize ;index++ ) {
        QString srcTRTSFilePath = tRTSFileInfoList.at(index).SrcTRTSFilePath;
        QString outQMFilePath = tRTSFileInfoList.at(index).OutQMFilePath;
        QString command = tRTSFileInfoList.at(index).Command;
        //qDebug()<<"command: "<<command;
        emit AppendPossessLog(QString(tr("-------正在处理 当前第：%1项 共：%2项------\n")).arg(index+1).arg(tRTSFileInfoListSize)+srcTRTSFilePath);
        emit AppendPossessLog(QString(tr("-------处理至------\n%1")).arg(outQMFilePath));
        QProcess p;
        p.start(command);
        p.waitForStarted();
        p.closeWriteChannel();  //关闭写通道 ，解决未响应问题
        p.waitForFinished();
        QString OutMsg = QString::fromLocal8Bit(p.readAllStandardOutput());
        QString ErrMsg = QString::fromLocal8Bit(p.readAllStandardError());
        p.close();
        if(ErrMsg==""){
            handleSuccessCount++;
            handelResult=tr("成功");
        }
        else{
            handleFaileCount++;
            handelResult=tr("失败");
        }
        emit AppendPossessLog(QString(tr("-------处理结果【%1】------\n注意：结果【失败】不一定真失败了仅供参考\n")+OutMsg+"\n"+ErrMsg).arg(handelResult));
    }
    emit AppendPossessLog(tr("-------处理完成------\n"));
    emit AppendPossessLog(QString(tr("-------总计：%1项    成功：%2项    失败：%3项------\n")).arg(tRTSFileInfoListSize).arg(handleSuccessCount).arg(handleFaileCount));
    emit CtrlSetEnabled(tr("GenerateQMFileTBtn"),tr("生成QM文件"),true);
}

void QMFileGenerateWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="GenerateQMFileTBtn"){
        ui->textEdit->clear();
        ui->GenerateQMFileTBtn->setText(tr("处理中..."));
        ui->GenerateQMFileTBtn->setEnabled(false);
        GenerateQMFileFun();
    }
    else if (ToolButton->objectName()=="RefreshTSFileListTBtn") {
        ui->treeWidget->clear();
        LoadTranslateTSFile();
    }
}

void QMFileGenerateWin::TreeWidgetItemChanged(QTreeWidgetItem *item, int column)
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

    connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&QMFileGenerateWin::TreeWidgetItemChanged);
}

void QMFileGenerateWin::CtrlSetEnabledSlots(const QString &ctrlName, const QString &txtStr, bool isEnabled)
{
    if(ctrlName==tr("GenerateQMFileTBtn")){
        ui->GenerateQMFileTBtn->setText(txtStr);
        ui->GenerateQMFileTBtn->setEnabled(isEnabled);
    }
}

void QMFileGenerateWin::AppendPossessLogSlots(const QString &logStr)
{
    ui->textEdit->append(logStr);
}
