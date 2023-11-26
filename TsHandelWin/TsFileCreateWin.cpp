# pragma execution_character_set("utf-8")
#include "TsFileCreateWin.h"
#include "ui_TsFileCreateWin.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include "FileHandel/DirOperate.h"
#include "DataManage/DataOperate.h"

#include <thread>

TsFileCreateWin::TsFileCreateWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TsFileCreateWin)
{
    ui->setupUi(this);

    InitGroupBox();
    InitFrame();
    InitLineEdit();
    InitTextEdit();
    InitComboBox();
    InitToolButton();
    InitLabel();

    LoadWinData();

    connect(this,static_cast<void (TsFileCreateWin::*)(const QString &,const QString &,bool)>(&TsFileCreateWin::CtrlSetEnabled), this,&TsFileCreateWin::CtrlSetEnabledSlots);
    connect(this,static_cast<void (TsFileCreateWin::*)(const QString &)>(&TsFileCreateWin::AppendPossessLog), this,&TsFileCreateWin::AppendPossessLogSlots);
}

TsFileCreateWin::~TsFileCreateWin()
{
    delete ui;
}

void TsFileCreateWin::InitGroupBox()
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
}

void TsFileCreateWin::InitFrame()
{
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_2->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_3->layout()->setContentsMargins(0, 0, 0, 0);
}

void TsFileCreateWin::InitLineEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);
    ui->lineEdit_3->setFont(font);

    ui->lineEdit->setPlaceholderText(tr("Qt-Kit安装路径"));
    ui->lineEdit_2->setPlaceholderText(tr("目标工程文件路径（pro文件）"));
    //ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_3->setPlaceholderText(tr("TS文件名编辑"));
}

void TsFileCreateWin::InitTextEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->textEdit->setFont(font);
    //    ui->textEdit->clear();
    //    ui->textEdit->setStyleSheet("QTextEdit{border:3px solid blue;border-radius: 5px;}");
}

void TsFileCreateWin::InitComboBox()
{
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    connect(ui->comboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,&TsFileCreateWin::ComboBoxActivated);
    connect(ui->comboBox_2,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,&TsFileCreateWin::ComboBoxActivated);
}

void TsFileCreateWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->GenerateTSFileTBtn<<ui->SelectKitDirTBtn<<ui->SelectProFileTBtn<<ui->AppendItemTBtn<<ui->DeleteCurItemTBtn;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】生成TS文件")<<tr("【点击】选择Qt-Kit安装的目录")<<tr("【点击】选择需要生成TS文件的工程（.pro）文件")
              <<tr("【点击】添加新的TS文件名")<<tr("【点击】删除当前选中的TS文件名");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&TsFileCreateWin::ToolButtonClicked);
    }
}

void TsFileCreateWin::InitLabel()
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

void TsFileCreateWin::LoadWinData()
{
    QString kitDir= DataOperate::Instance()->GetKitDir();
    QString proFilePath= DataOperate::Instance()->GetProFilePath();

    KitSelectIndex=DataOperate::Instance()->GetIniFileNode(tr("TsFileCreateWinGroup"),tr("KitSelectIndex"), tr("0")).toInt();
    TSFileSelectIndex=DataOperate::Instance()->GetIniFileNode(tr("TsFileCreateWinGroup"),tr("TSFileSelectIndex"), tr("0")).toInt();
    TSFileNameCount=DataOperate::Instance()->GetIniFileNode(tr("TsFileCreateWinGroup"),tr("TSFileNameCount"), tr("0")).toInt();

    ui->lineEdit->setText(kitDir);
    ui->lineEdit_2->setText(proFilePath);

    if(kitDir!=tr("")){
        QStringList kitList;kitList.clear();
        ui->comboBox->clear();
        kitList.append(tr("全部"));

        ForEachDirKit(kitDir,kitList);

        int kitListSize=kitList.size();
        if(kitListSize>1)
            ui->comboBox->addItems(kitList);

        ui->comboBox->setCurrentIndex(KitSelectIndex);
    }

    if(TSFileNameCount>0){
        QStringList tsFileNameList;tsFileNameList.clear();
        tsFileNameList<<tr("全部");
        QString tsFileName=DataOperate::Instance()->GetIniFileNode(tr("TsFileCreateWinGroup"),QString("TSFileNameList"), tr(""));
        QStringList LIST=tsFileName.split("-");
        tsFileNameList<<LIST;
        ui->comboBox_2->addItems(tsFileNameList);
        ui->comboBox_2->setCurrentIndex(TSFileSelectIndex);
    }
}

void TsFileCreateWin::SelectKitDirFun()
{
    QString selectDir = QFileDialog::getExistingDirectory();
    if(selectDir.isEmpty())
        return;

    ui->lineEdit->setText(selectDir);
    qDebug() << "KitDir:" << selectDir;
    DataOperate::Instance()->SetKitDir(selectDir);

    QStringList kitList;kitList.clear();
    ui->comboBox->clear();
    kitList.append(tr("全部"));

    ForEachDirKit(selectDir,kitList);

    int kitListSize=kitList.size();
    if(kitListSize>1)
        ui->comboBox->addItems(kitList);
}

void TsFileCreateWin::SelectProFileFun()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open a file."),
                                                    QCoreApplication::applicationDirPath(),
                                                    tr("Pro File(*.Pro)"));
    if (fileName.isEmpty())
        return;

    ui->lineEdit_2->setText(fileName);
    qDebug() << "ProFilePath:" << fileName;
    DataOperate::Instance()->SetProFilePath(fileName);
}

void TsFileCreateWin::GenerateTSFileFun()
{
    CommandInfoList.clear();
    if(ui->comboBox->currentText()==tr("全部")){
        int comboBoxCount=ui->comboBox->count();
        for (int index=1;index<comboBoxCount ;index++ ) {

            QString kitType=ui->comboBox->itemText(index);
            QString kitDir= DataOperate::Instance()->GetKitDir();
            QString proFilePath= DataOperate::Instance()->GetProFilePath();
            QFileInfo fileInfo(proFilePath);
            QString tsFileDir=fileInfo.path()+"/language/TsFileCreate/"+kitType+"/";
            DirOperate dirOperate;
            dirOperate.CreateDir(tsFileDir);

            if(ui->comboBox_2->currentText()==tr("全部")){
                int comboBox2Count=ui->comboBox_2->count();
                for (int tIndex=1;tIndex<comboBox2Count ;tIndex++ ) {
                    QString tsFileName=ui->comboBox_2->itemText(tIndex);

                    QString tsFilePath=tsFileDir+tsFileName+".ts";
                    QString cmd=kitDir+"/"+kitType+"/bin/lupdate "
                            +proFilePath
                            +" -ts "+tsFilePath;
                    //qDebug()<<"tsFilePath: "<<tsFilePath;
                    CommandInfoList.push_back(CommandInfo(cmd,tsFilePath));
                }
            }
            else {
                QString tsFileName=ui->comboBox_2->currentText();

                QString tsFilePath=tsFileDir+tsFileName+".ts";
                QString cmd=kitDir+"/"+kitType+"/bin/lupdate "
                        +proFilePath
                        +" -ts "+tsFilePath;
                //qDebug()<<"tsFilePath: "<<tsFilePath;
                CommandInfoList.push_back(CommandInfo(cmd,tsFilePath));
            }
        }
    }else {

        QString kitDir= DataOperate::Instance()->GetKitDir();
        QString proFilePath= DataOperate::Instance()->GetProFilePath();

        QString kitType=ui->comboBox->currentText();
        QFileInfo fileInfo(proFilePath);
        QString tsFileDir=fileInfo.path()+"/language/TsFileCreate/"+kitType+"/";
        DirOperate dirOperate;
        dirOperate.CreateDir(tsFileDir);

        if(ui->comboBox_2->currentText()==tr("全部")){
            int comboBox2Count=ui->comboBox_2->count();
            for (int tIndex=1;tIndex<comboBox2Count ;tIndex++ ) {
                QString tsFileName=ui->comboBox_2->itemText(tIndex);

                QString tsFilePath=tsFileDir+tsFileName+".ts";
                QString cmd=kitDir+"/"+kitType+"/bin/lupdate "
                        +proFilePath
                        +" -ts "+tsFilePath;
                //qDebug()<<"tsFilePath: "<<tsFilePath;
                CommandInfoList.push_back(CommandInfo(cmd,tsFilePath));
            }
        }
        else {
            QString tsFileName=ui->comboBox_2->currentText();

            QString tsFilePath=tsFileDir+tsFileName+".ts";
            QString cmd=kitDir+"/"+kitType+"/bin/lupdate "
                    +proFilePath
                    +" -ts "+tsFilePath;
            //qDebug()<<"tsFilePath: "<<tsFilePath;
            CommandInfoList.push_back(CommandInfo(cmd,tsFilePath));
        }
    }

    HandelCommandList();
}

void TsFileCreateWin::AppendItemFun()
{
    QString tsFileName=ui->lineEdit_3->text();
    if(tsFileName.trimmed().isEmpty())
        return;

    ui->lineEdit_3->clear();

    int comboBox2Count=ui->comboBox_2->count();

    TSFileNameCount++;
    if(comboBox2Count<=0)
        ui->comboBox_2->addItem(tr("全部"));

    ui->comboBox_2->addItem(tsFileName);
    comboBox2Count=ui->comboBox_2->count();
    QString tSFileNameList="";
    for (int index=1;index<comboBox2Count ;index++ ) {
        QString tsFileName=ui->comboBox_2->itemText(index);
        tSFileNameList.append(tsFileName);
        if(index!=(comboBox2Count-1))
            tSFileNameList.append("-");
    }
    ui->comboBox_2->setCurrentIndex(0);
    qDebug()<<"tSFileNameList: "<<tSFileNameList;
    DataOperate::Instance()->WriteIniFile(tr("TsFileCreateWinGroup"),tr("TSFileNameCount"), QString::number(TSFileNameCount));
    DataOperate::Instance()->WriteIniFile(tr("TsFileCreateWinGroup"),tr("TSFileNameList"), tSFileNameList);
    DataOperate::Instance()->WriteIniFile(tr("TsFileCreateWinGroup"),tr("TSFileSelectIndex"), QString::number(0));
}

void TsFileCreateWin::DeleteCurItemFun()
{
    if(ui->comboBox_2->currentText()==tr("全部"))
        return;

    TSFileNameCount--;
    int curIndex=ui->comboBox_2->currentIndex();
    ui->comboBox_2->removeItem(curIndex);
    QString tSFileNameList="";
    int comboBox2Count=ui->comboBox_2->count();
    for (int index=1;index<comboBox2Count ;index++ ) {
        QString tsFileName=ui->comboBox_2->itemText(index);
        tSFileNameList.append(tsFileName);
        if(index!=(comboBox2Count-1))
            tSFileNameList.append("-");
    }
    if(comboBox2Count<=1)
        ui->comboBox_2->clear();
    else
        ui->comboBox_2->setCurrentIndex(0);
    DataOperate::Instance()->WriteIniFile(tr("TsFileCreateWinGroup"),tr("TSFileNameCount"), QString::number(TSFileNameCount));
    DataOperate::Instance()->WriteIniFile(tr("TsFileCreateWinGroup"),tr("TSFileNameList"), tSFileNameList);
    DataOperate::Instance()->WriteIniFile(tr("TsFileCreateWinGroup"),tr("TSFileSelectIndex"), QString::number(0));
}

void TsFileCreateWin::ForEachDirKit(QString kitDir,QStringList &kitList)
{
    // 判断路径是否存在
    QDir dir(kitDir);
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot );
    QFileInfoList list = dir.entryInfoList();

    int dirCount = list.count();
    if(dirCount <= 0)
        return;

    for(int index=0; index<dirCount; index++){
        QFileInfo fileInfo = list.at(index);
        if(!fileInfo.fileName().contains("src",Qt::CaseInsensitive))
            kitList.append(fileInfo.fileName());
    }
}

void TsFileCreateWin::HandelCommandList()
{
    std::thread t([=](){
        int handleSuccessCount=0,handleFaileCount=0;
        QString handelResult="";
        int CommandListSize=CommandInfoList.size();
        emit AppendPossessLog(tr("-------开始处理------"));
        for (int index=0;index<CommandListSize ;index++ ) {
            QString command = CommandInfoList.at(index).Command;
            //qDebug()<<"command: "<<command;
            emit AppendPossessLog(QString(tr("-------正在处理 当前第：%1项 共：%2项------\n")).arg(index+1).arg(CommandListSize)+CommandInfoList.at(index).TSFilePath);
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
        emit AppendPossessLog(QString(tr("-------总计：%1项    成功：%2项    失败：%3项------\n")).arg(CommandListSize).arg(handleSuccessCount).arg(handleFaileCount));
        emit CtrlSetEnabled(tr("GenerateTSFileTBtn"),tr("生成TS文件"),true);
        CommandInfoList.clear();
    });
    t.detach();
}

void TsFileCreateWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="GenerateTSFileTBtn"){
        ui->textEdit->clear();
        ui->GenerateTSFileTBtn->setText(tr("处理中..."));
        ui->GenerateTSFileTBtn->setEnabled(false);
        GenerateTSFileFun();
    }
    else if (ToolButton->objectName()=="SelectKitDirTBtn") {
        SelectKitDirFun();
    }
    else if (ToolButton->objectName()=="SelectProFileTBtn") {
        SelectProFileFun();
    }
    else if (ToolButton->objectName()=="AppendItemTBtn") {
        AppendItemFun();
    }
    else if (ToolButton->objectName()=="DeleteCurItemTBtn") {
        DeleteCurItemFun();
    }
}

void TsFileCreateWin::ComboBoxActivated(int index)
{
    QComboBox *ComboBox = qobject_cast<QComboBox*>(sender());
    qDebug()<<ComboBox->objectName()<<"  "<<ComboBox->currentText();
    if(ComboBox->objectName()=="comboBox"){
        KitSelectIndex=index;
        DataOperate::Instance()->WriteIniFile(tr("TsFileCreateWinGroup"),tr("KitSelectIndex"), QString::number(KitSelectIndex));
    }
    else if(ComboBox->objectName()=="comboBox_2"){
        TSFileSelectIndex=index;
        DataOperate::Instance()->WriteIniFile(tr("TsFileCreateWinGroup"),tr("TSFileSelectIndex"), QString::number(TSFileSelectIndex));
        //        if(ComboBox->currentText()==tr("全部")){
        //            ui->lineEdit_3->setEnabled(true);
        //        }else {
        //            ui->lineEdit_3->setEnabled(false);
        //        }
    }
}

void TsFileCreateWin::CtrlSetEnabledSlots(const QString &ctrlName, const QString &txtStr, bool isEnabled)
{
    if(ctrlName==tr("GenerateTSFileTBtn")){
        ui->GenerateTSFileTBtn->setText(txtStr);
        ui->GenerateTSFileTBtn->setEnabled(isEnabled);
    }
}

void TsFileCreateWin::AppendPossessLogSlots(const QString &logStr)
{
    ui->textEdit->append(logStr);
}
