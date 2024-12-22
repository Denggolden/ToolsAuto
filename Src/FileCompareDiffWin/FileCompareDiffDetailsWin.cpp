#include "FileCompareDiffDetailsWin.h"
#include "ui_FileCompareDiffDetailsWin.h"

#include <QtConcurrent>
#include <QFileDialog>
#include <QMessageBox>
#include <chrono>

#include "Src/DataManage/DataOperate.h"

FileCompareDiffDetailsWin::FileCompareDiffDetailsWin(QString openFlagSrc,QWidget *parent)
    : DialogBase(parent)
    , ui(new Ui::FileCompareDiffDetailsWin)
{
    ui->setupUi(this);

    OpenFlag(openFlagSrc);
}

FileCompareDiffDetailsWin::~FileCompareDiffDetailsWin()
{
    delete ui;
}

void FileCompareDiffDetailsWin::InitClass()
{
    InitGroupBox();
    InitFrame();
    InitLineEdit();
    InitToolButton();
    InitLabel();
    InitTableWidget();
    InitTimer();

    if(OpenFlagSrc=="defeat"){
        LoadWinStatus();
    }
}

void FileCompareDiffDetailsWin::InitGroupBox()
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

void FileCompareDiffDetailsWin::InitFrame()
{
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_2->layout()->setContentsMargins(0, 0, 0, 0);
}

void FileCompareDiffDetailsWin::InitLineEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);

    ui->lineEdit->setPlaceholderText(tr("来源1路径"));
    ui->lineEdit_2->setPlaceholderText(tr("来源2路径"));

    ui->lineEdit->setEnabled(false);ui->lineEdit_2->setEnabled(false);
}

void FileCompareDiffDetailsWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->BackTBtn<<ui->OpenDiffTBtn<<ui->SelectFile1TBtn<<ui->SelectFile2TBtn;
    QStringList toolTipList;toolTipList.clear();
    //待定
    toolTipList<<tr("【点击】开启文件(夹)对比")<<tr("【点击】选择文件(夹)1路径")<<tr("【点击】选择文件(夹)2路径")<<tr("【点击】进行运行文件整理");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&FileCompareDiffDetailsWin::ToolButtonClicked);
    }

    connect(this,static_cast<void (FileCompareDiffDetailsWin::*)(bool)>(&FileCompareDiffDetailsWin::SetTBtnIsEnableSignal), this,&FileCompareDiffDetailsWin::SetTBtnIsEnableSlots);
}

void FileCompareDiffDetailsWin::InitLabel()
{
    QList<QLabel*> LableList = this->findChildren<QLabel*>();
    foreach (QLabel *Lable, LableList) {
        if (Lable != nullptr){
            //Lable->setStyleSheet("border: 2px solid blue");
            Lable->setFont(QFont("Microsoft YaHei", 8, QFont::Bold));        //设置字体
            //Lable->setAlignment(Qt::AlignCenter);
        }
    }

    ui->label->setText(tr("文件名"));
    ui->label_2->setText(tr("对比结果"));

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);

    connect(this,static_cast<void (FileCompareDiffDetailsWin::*)(const QString&)>(&FileCompareDiffDetailsWin::UpDateDiffResult),
            this,&FileCompareDiffDetailsWin::UpDateDiffResultSlots,Qt::QueuedConnection);
}

void FileCompareDiffDetailsWin::InitTableWidget()
{
    QStringList labels;
    labels<<tr("项目")<<tr("结果")<<tr("状态")<<tr("耗时");

    //设置列数
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget_2->setColumnCount(4);

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget_2->verticalHeader()->hide();

    //创建表头
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget_2->setHorizontalHeaderLabels(labels);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //或
    //tableWidget->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Fixed);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行

    //设置选中行的背景色，必须要显示设置，即代码设置或者在ui文件的控件中设置，用系统默认的是无法代码选中高亮某行
    ui->tableWidget->setStyleSheet("selection-background-color:rgb(255,209,128)");
    ui->tableWidget_2->setStyleSheet("selection-background-color:rgb(255,209,128)");

    connect(ui->tableWidget,static_cast<void (QTableWidget::*)(int,int)>(&QTableWidget::cellClicked),this,[this](int row, int column){
        ui->tableWidget_2->selectRow(row);
    });
    connect(ui->tableWidget_2,static_cast<void (QTableWidget::*)(int,int)>(&QTableWidget::cellClicked),this,[this](int row, int column){
        ui->tableWidget->selectRow(row);
    });


    connect(this,static_cast<void (FileCompareDiffDetailsWin::*)(const QString&)>(&FileCompareDiffDetailsWin::ClearTableWidget),
            this,&FileCompareDiffDetailsWin::ClearTableWidgetSlots,Qt::QueuedConnection);

    connect(this,static_cast<void (FileCompareDiffDetailsWin::*)(const QString&,int,bool,bool,const QString&,const QString&,const QString&,const QString&)>(&FileCompareDiffDetailsWin::UpDateTableWidget),
            this,&FileCompareDiffDetailsWin::UpDateTableWidgetSlots,Qt::QueuedConnection);
}

void FileCompareDiffDetailsWin::InitTimer()
{
    pTimer=new QTimer();
    pTimer->setInterval(100);
    connect(pTimer,&QTimer::timeout,this,&FileCompareDiffDetailsWin::OnTimeOut);

    pTimerWait=new QTimer();
    pTimerWait->setInterval(100);
    connect(pTimerWait,&QTimer::timeout,this,&FileCompareDiffDetailsWin::OnTimeOutWait);

    connect(this,static_cast<void (FileCompareDiffDetailsWin::*)(bool)>(&FileCompareDiffDetailsWin::TimerIsEnable),
            this,&FileCompareDiffDetailsWin::TimerIsEnableSlots,Qt::QueuedConnection);
}

void FileCompareDiffDetailsWin::LoadWinStatus()
{
    File1Path=DataOperate::Instance()->GetIniFileNode(tr("FileCompareDiffDetailsWinGroup"),tr("File1Path"), tr(""));
    File2Path=DataOperate::Instance()->GetIniFileNode(tr("FileCompareDiffDetailsWinGroup"),tr("File2Path"), tr(""));

    ui->lineEdit->setText(File1Path);
    ui->lineEdit_2->setText(File2Path);

    if(File1Path==""||File2Path==""){
        ui->label->setText("");
    }else{
        QString fileName1= GetFileBaseInfo(File1Path,"fileName");
        QString fileName2= GetFileBaseInfo(File2Path,"fileName");

        if(fileName1==fileName2){ui->label->setText(fileName1);}
        else{
            QString con=tr("%1 及 %2").arg(fileName1).arg(fileName2);
            ui->label->setText(con);
        }
    }
}

void FileCompareDiffDetailsWin::SetTBtnIsEnable(bool enable)
{
    int ToolButtonListSize=ToolButtonList.size();
    for (int index = 0; index < ToolButtonListSize; ++index) {
        if(ToolButtonList[index]->objectName()=="BackTBtn"){
            continue;
        }
        ToolButtonList[index]->setEnabled(enable);
    }
}

void FileCompareDiffDetailsWin::OpenFlag(QString flag)
{
    if(flag=="defeat"){
        ui->BackTBtn->setText(tr("终止"));
        ui->BackTBtn->setEnabled(true);
        // ui->BackTBtn->setEnabled(true);
        // ui->OpenDiffTBtn->setEnabled(true);
        // ui->SelectFile1TBtn->setEnabled(true);
        // ui->SelectFile2TBtn->setEnabled(true);

        // ui->BackTBtn->show();
        // ui->OpenDiffTBtn->show();
        // ui->SelectFile1TBtn->show();
        // ui->SelectFile2TBtn->show();
    }else{
        ui->BackTBtn->setText(tr("返回"));
        ui->BackTBtn->setEnabled(true);
        // ui->BackTBtn->setEnabled(true);
        // ui->OpenDiffTBtn->setEnabled(false);
        // ui->SelectFile1TBtn->setEnabled(false);
        // ui->SelectFile2TBtn->setEnabled(false);

        // ui->BackTBtn->show();
        // ui->OpenDiffTBtn->hide();
        ui->SelectFile1TBtn->hide();
        ui->SelectFile2TBtn->hide();
    }

    OpenFlagSrc=flag;
}

void FileCompareDiffDetailsWin::OpenDiff()
{
    SetIsOpenDiffCompete(false);
    SetIsCancel(false);
    auto ret= QtConcurrent::run([this](){

        //SetIsOpenDiffCompete(false);
        emit SetTBtnIsEnableSignal(false);

        QStringList itemList;itemList.clear();
        itemList<<tr("文件名")<<tr("文件大小")<<tr("创建时间")<<tr("修改时间")<<tr("读取时间")<<tr("MD5")<<tr("SHA1")/*<<tr("SHA512")*/;

        int equalCount=0;
        int diffCount=0;
        int allItemCount=0;

        emit ClearTableWidget("tableWidget");
        emit ClearTableWidget("tableWidget_2");

        emit TimerIsEnable(true);
        emit UpDateDiffResult(tr("对比中"));
        int itemListSize=itemList.size();
        for (int index = 0; index < itemListSize; ++index) {

            if(GetIsCancel()==true){//看情况提前结束
                break;
            }

            auto start = std::chrono::high_resolution_clock::now();

            QString itemName=itemList[index];
            QString itemResult="";
            QString itemStatus="进行中";
            //开始计算
            emit UpDateTableWidget("tableWidget",index,true,true,itemName,itemResult,itemStatus,"");
            emit UpDateTableWidget("tableWidget_2",index,true,true,itemName,itemResult,itemStatus,"");

            QThread::msleep(10);//这里模拟计算

            QString itemResult1="";
            QString itemResult2="";

            if(itemList[index]==tr("文件名")){
                itemResult1=GetFileBaseInfo(File1Path,"fileName");
                itemResult2=GetFileBaseInfo(File2Path,"fileName");
            }
            if(itemList[index]==tr("文件大小")){
                itemResult1=GetFileBaseInfo(File1Path,"size")+" Byte";
                itemResult2=GetFileBaseInfo(File2Path,"size")+" Byte";
            }
            if(itemList[index]==tr("创建时间")){
                itemResult1=GetFileBaseInfo(File1Path,"birthTime");
                itemResult2=GetFileBaseInfo(File2Path,"birthTime");
            }
            if(itemList[index]==tr("修改时间")){
                itemResult1=GetFileBaseInfo(File1Path,"lastModified");
                itemResult2=GetFileBaseInfo(File2Path,"lastModified");
            }
            if(itemList[index]==tr("读取时间")){
                itemResult1=GetFileBaseInfo(File1Path,"lastRead");
                itemResult2=GetFileBaseInfo(File2Path,"lastRead");
            }

            if(itemList[index]==tr("MD5")){
                itemResult1=GetFileHashStr(File1Path,QCryptographicHash::Md5);
                itemResult2=GetFileHashStr(File2Path,QCryptographicHash::Md5);
            }
            if(itemList[index]==tr("SHA1")){
                itemResult1=GetFileHashStr(File1Path,QCryptographicHash::Sha1);
                itemResult2=GetFileHashStr(File2Path,QCryptographicHash::Sha1);
            }
            // if(itemList[index]==tr("SHA512")){
            //     itemResult1=GetFileHashStr(File1Path,QCryptographicHash::Sha512);
            //     itemResult2=GetFileHashStr(File2Path,QCryptographicHash::Sha512);
            // }

            bool isEqual=false;
            if(itemResult1==itemResult2){
                isEqual=true;
                equalCount++;
            }else{
                diffCount++;
            }

            allItemCount++;

            itemStatus="完成";

            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
            QString durationStr=CovDuration(duration);

            //计算完成
            emit UpDateTableWidget("tableWidget",index,false,isEqual,itemName,itemResult1,itemStatus,durationStr);
            emit UpDateTableWidget("tableWidget_2",index,false,isEqual,itemName,itemResult2,itemStatus,durationStr);
        }

        emit TimerIsEnable(false);
        emit UpDateDiffResult(tr("对比完成: [相同项目数为：%1] [不同项目数为：%2 -已标红] [总项目数为：%3]").arg(equalCount).arg(diffCount).arg(allItemCount));

        SetIsOpenDiffCompete(true);
        emit SetTBtnIsEnableSignal(true);

        return;
    });
}

void FileCompareDiffDetailsWin::SelectFilePathFun(QString flag)
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("open a file."),
        "D:/",
        tr("All files(*.*)"));

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
        qDebug()<<"Failed to open the file!";
        return;
    }

    if(flag==QString("SelectFile1TBtn")){
        File1Path=fileName;
        ui->lineEdit->setText(fileName);
        qDebug()<<QString("选择的文件1路径：%1").arg(fileName);
        DataOperate::Instance()->WriteIniFile(tr("FileCompareDiffDetailsWinGroup"),tr("File1Path"),fileName);
    }
    if(flag==QString("SelectFile2TBtn")){
        File2Path=fileName;
        ui->lineEdit_2->setText(fileName);
        qDebug()<<QString("选择的文件1路径：%1").arg(fileName);
        DataOperate::Instance()->WriteIniFile(tr("FileCompareDiffDetailsWinGroup"),tr("File2Path"),fileName);
    }

    if(File1Path==""||File2Path==""){
        ui->label->setText("");
    }else{
        QString fileName1= GetFileBaseInfo(File1Path,"fileName");
        QString fileName2= GetFileBaseInfo(File2Path,"fileName");

        if(fileName1==fileName2){ui->label->setText(fileName1);}
        else{
            QString con=tr("%1 及 %2").arg(fileName1).arg(fileName2);
            ui->label->setText(con);
        }
    }
}

QString FileCompareDiffDetailsWin::GetFileBaseInfo(const QString &filePath, const QString &flag)
{
    QFileInfo fileInfo(filePath);
    QString retValue="";
    if(flag=="fileName")
        retValue=fileInfo.fileName();
    if(flag=="suffix")
        retValue=fileInfo.suffix();
    if(flag=="size")
        retValue=QString::number(fileInfo.size());
    if(flag=="lastModified")
        retValue=fileInfo.lastModified().toString();
    if(flag=="lastRead")
        retValue=fileInfo.lastRead().toString();
    if(flag=="birthTime")
        retValue=fileInfo.birthTime().toString();

    return retValue;
}

QString FileCompareDiffDetailsWin::GetFileHashStr(const QString &fileName, QCryptographicHash::Algorithm algorithm)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){//以只读形式打开文件
        return QByteArray();
    }

    QCryptographicHash hash(algorithm);
    while(!file.atEnd())
    {
        QByteArray data = file.read(64 * 1024 * 1024);// 64m  实际内容若不足只读实际大小
        //QByteArray catalog = file.readAll(); // 小文件可以一直全读在内存中，大文件必须分批处理
        hash.addData(data);
        //qApp->processEvents();//执行事件循环  防止界面卡顿。
    }
    QByteArray hashResult = hash.result();
    file.close();//及时关闭
    return hashResult.toHex().toUpper();
}

void FileCompareDiffDetailsWin::UpDatePossess()
{
    NCount++;
    if(NCount>3){
        NCount=0;
    }

    QString possessStr=tr("进行中");
    for (int index = 0; index < NCount; ++index) {
        possessStr.append(".");
    }

    UpTableWidgetPossess(possessStr);
    UpTableResultPossess(possessStr);
}

void FileCompareDiffDetailsWin::UpTableWidgetPossess(const QString &possessStr)
{
    int rowCount1= ui->tableWidget->rowCount()-1;
    int rowCount2= ui->tableWidget_2->rowCount()-1;

    ui->tableWidget->item(rowCount1,2)->setText(possessStr);
    ui->tableWidget_2->item(rowCount2,2)->setText(possessStr);
}

void FileCompareDiffDetailsWin::UpTableResultPossess(const QString &possessStr)
{
    UpDateDiffResultSlots(possessStr);
}

void FileCompareDiffDetailsWin::SetFile1And2Path(const QString &file1Path, const QString &file2Path)
{
    File1Path=file1Path;
    File2Path=file2Path;

    ui->lineEdit->setText(File1Path);
    ui->lineEdit_2->setText(File2Path);

    if(File1Path==""||File2Path==""){
        ui->label->setText("");
    }else{
        QString fileName1= GetFileBaseInfo(File1Path,"fileName");
        QString fileName2= GetFileBaseInfo(File2Path,"fileName");

        if(fileName1==fileName2){ui->label->setText(fileName1);}
        else{
            QString con=tr("%1 及 %2").arg(fileName1).arg(fileName2);
            ui->label->setText(con);
        }
    }
}

QString FileCompareDiffDetailsWin::CovDuration(long long duration)
{
    QString durationStr="";
    if(duration<=1000&&duration>=0){
        durationStr=QString("%1 us").arg(duration);
    }
    else if(duration<=1000*1000&&duration>1000){
        durationStr=QString("%1 ms").arg(QString::number((float)duration/1000.0,'f',3));
    }
    else if(duration<=1000*1000*1000&&duration>1000*1000){
        durationStr=QString("%1 s").arg(QString::number((float)duration/(1000.0*1000.0),'f',3));
    }

    return durationStr;
}

void FileCompareDiffDetailsWin::SetIsOpenDiffCompete(bool isOpenDiffCompete)
{
    IsOpenDiffCompeteMutex.lock();
    IsOpenDiffCompete=isOpenDiffCompete;
    IsOpenDiffCompeteMutex.unlock();
}

bool FileCompareDiffDetailsWin::GetIsOpenDiffCompete()
{
    bool tempIsOpenDiffCompete=false;
    IsOpenDiffCompeteMutex.lock();
    tempIsOpenDiffCompete=IsOpenDiffCompete;
    IsOpenDiffCompeteMutex.unlock();
    return tempIsOpenDiffCompete;
}

void FileCompareDiffDetailsWin::SetIsCancel(bool isCancel)
{
    IsCancelMutex.lock();
    IsCancel=isCancel;
    IsCancelMutex.unlock();
}

bool FileCompareDiffDetailsWin::GetIsCancel()
{
    bool tempIsCancel=false;
    IsCancelMutex.lock();
    tempIsCancel=IsCancel;
    IsCancelMutex.unlock();
    return tempIsCancel;
}

void FileCompareDiffDetailsWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="SelectFile1TBtn"){
        emit SetTBtnIsEnableSignal(false);
        SelectFilePathFun("SelectFile1TBtn");
        emit SetTBtnIsEnableSignal(true);
    }
    else if(ToolButton->objectName()=="SelectFile2TBtn"){
        emit SetTBtnIsEnableSignal(false);
        SelectFilePathFun("SelectFile2TBtn");
        emit SetTBtnIsEnableSignal(true);
    }
    else if(ToolButton->objectName()=="OpenDiffTBtn"){
        if(File1Path==""||File2Path==""){
            QMessageBox::warning(this, "Warning!", "请选择文件1路径、选择文件2路径不能为空！！！");
            return;
        }

        OpenDiff();

        if(OpenFlagSrc=="defeat"){
            ui->BackTBtn->setEnabled(true);
        }else{}
    }
    else if(ToolButton->objectName()=="BackTBtn"){
        if(GetIsOpenDiffCompete()==true){//已经结束没啥东西可以发生了
            if(OpenFlagSrc=="defeat"){
            }else{
                this->close();
            }
        }else{//未结束
            ui->BackTBtn->setEnabled(false);
            SetIsCancel(true);//之后等待提前终止
            //或许得启动一个定时器查询
            pTimerWait->start();
        }
    }
}

void FileCompareDiffDetailsWin::SetTBtnIsEnableSlots(bool enable)
{
    SetTBtnIsEnable(enable);
}

void FileCompareDiffDetailsWin::UpDateDiffResultSlots(const QString &result)
{
    // if(result==tr("失败")){
    // }else{
    // }
    ui->label_2->setText(result);
}

void FileCompareDiffDetailsWin::ClearTableWidgetSlots(const QString &upDateTableWidget)
{
    if(upDateTableWidget=="tableWidget"){
        while (ui->tableWidget->rowCount()!=0) {
            ui->tableWidget->removeRow(0);
        }
        //ui->tableWidget->clear();
    }
    if(upDateTableWidget=="tableWidget_2"){
        while (ui->tableWidget_2->rowCount()!=0) {
            ui->tableWidget_2->removeRow(0);
        }
        //ui->tableWidget_2->clear();
    }
}

void FileCompareDiffDetailsWin::UpDateTableWidgetSlots(const QString &upDateTableWidget, int rowIndex, bool isNewAdd,bool isEqual, const QString &itemName, const QString &itemResult, const QString &itemStatus,const QString& duration)
{
    QFont font("Microsoft YaHei", 8, QFont::Bold);
    if(upDateTableWidget=="tableWidget"){
        if(isNewAdd==true){
            ui->tableWidget->insertRow(rowIndex);//增加一行
            QStringList LIST;LIST.clear();
            LIST<<itemName<<itemResult<<itemStatus<<duration;
            int LISTSize=LIST.size();
            for (int col = 0; col < LISTSize; ++col) {
                ui->tableWidget->setItem(rowIndex,col,new QTableWidgetItem(LIST[col]));
                ui->tableWidget->item(rowIndex,col)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->tableWidget->item(rowIndex,col)->setFont(font);
            }

        }else{
            QStringList LIST;LIST.clear();
            LIST<<itemName<<itemResult<<itemStatus<<duration;
            int LISTSize=LIST.size();
            for (int col = 0; col < LISTSize; ++col) {
                ui->tableWidget->item(rowIndex,col)->setText(LIST[col]);
            }
        }
    }

    if(upDateTableWidget=="tableWidget_2"){
        if(isNewAdd==true){
            ui->tableWidget_2->insertRow(rowIndex);//增加一行
            QStringList LIST;LIST.clear();
            LIST<<itemName<<itemResult<<itemStatus<<duration;
            int LISTSize=LIST.size();
            for (int col = 0; col < LISTSize; ++col) {
                ui->tableWidget_2->setItem(rowIndex,col,new QTableWidgetItem(LIST[col]));
                ui->tableWidget_2->item(rowIndex,col)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->tableWidget_2->item(rowIndex,col)->setFont(font);
            }

        }else{
            QStringList LIST;LIST.clear();
            LIST<<itemName<<itemResult<<itemStatus<<duration;
            int LISTSize=LIST.size();
            for (int col = 0; col < LISTSize; ++col) {
                ui->tableWidget_2->item(rowIndex,col)->setText(LIST[col]);
            }
        }
    }

    if(isEqual==false){
        for (int col = 0; col < 4; ++col) {
            ui->tableWidget->item(rowIndex,col)->setForeground(QColor(Qt::red));
            ui->tableWidget_2->item(rowIndex,col)->setForeground(QColor(Qt::red));
        }
    }

    // pItem->setBackground(QBrush(Qt::blue));        // 设置背景色
    // pItem->setForeground(QBrush(Qt::red));         // 设置字体颜色
}

void FileCompareDiffDetailsWin::TimerIsEnableSlots(bool enable)
{
    if(enable==true){
        pTimer->start();
    }
    if(enable==false){
        pTimer->stop();
    }
}

void FileCompareDiffDetailsWin::OnTimeOut()
{
    UpDatePossess();
}

void FileCompareDiffDetailsWin::OnTimeOutWait()
{
    if(GetIsOpenDiffCompete()==true){//已经结束
        pTimerWait->stop();
        //发生后续动作
        if(OpenFlagSrc=="defeat"){
            ui->BackTBtn->setText(tr("终止"));
            ui->BackTBtn->setEnabled(true);
        }else{
            this->close();
        }
    }else{
        ui->BackTBtn->setText(tr("等待资源回收完成！！！"));
    }
}

