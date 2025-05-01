#include "FileInfoExtractWin.h"
#include "ui_FileInfoExtractWin.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QtConcurrent>

#include "Src/DataManage/DataOperate.h"

FileInfoExtractWin::FileInfoExtractWin(QWidget *parent)
    : WidgetBase(parent)
    , ui(new Ui::FileInfoExtractWin)
{
    ui->setupUi(this);
}

FileInfoExtractWin::~FileInfoExtractWin()
{
    delete ui;
}

void FileInfoExtractWin::InitClass()
{
    InitGroupBox();
    InitLineEdit();
    InitToolButton();
    InitTreeWidget();
    InitLabel();
    InitTextEdit();
    InitTableWidget();
    InitTimer();

    LoadWinStatus();

    connect(this,static_cast<void (FileInfoExtractWin::*)(const QString&)>(&FileInfoExtractWin::SetCurCalFile),
            this,&FileInfoExtractWin::SetCurCalFileSlots,Qt::QueuedConnection);
}

void FileInfoExtractWin::InitGroupBox()
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

void FileInfoExtractWin::InitLineEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->lineEdit->setFont(font);

    ui->lineEdit->setPlaceholderText(tr("文件夹路径"));

    ui->lineEdit->setEnabled(false);
}

void FileInfoExtractWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->SelectDirPathTBtn<<ui->CalculateTBtn;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】选择文件夹") <<tr("XXX");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&FileInfoExtractWin::ToolButtonClicked);
    }

    connect(this,static_cast<void (FileInfoExtractWin::*)(bool)>(&FileInfoExtractWin::SetTBtnIsEnableSignal), this,&FileInfoExtractWin::SetTBtnIsEnableSlots);
}

void FileInfoExtractWin::InitTreeWidget()
{
    // 设置QTreeWidget的列数
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setHeaderLabels({tr("文件名"),tr("类型"),tr("相对路径")});
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //自动展开
    ui->treeWidget->setItemsExpandable(true);
    ui->treeWidget->expandAll();
    //ui->treeWidget->hideColumn(2);

    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&FileCompareDiffWin::TreeWidgetItemChanged);
    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemClicked), this,&FileCompareDiffWin::TreeWidgetItemClicked);
    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemPressed), this,&FileCompareDiffWin::TreeWidgetItemPressed);

    connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemPressed), this,&FileInfoExtractWin::TreeWidgetItemPressed);

    connect(this,static_cast<void (FileInfoExtractWin::*)(QTreeWidgetItem *,QString,QString,QString)>(&FileInfoExtractWin::StartCreateTree), this,&FileInfoExtractWin::StartCreateTreeSlots
            ,Qt::QueuedConnection);
    connect(this,static_cast<void (FileInfoExtractWin::*)(QTreeWidgetItem *,QTreeWidgetItem*,QString,QString,QString)>(&FileInfoExtractWin::StartCreateTreeChild),
            this,&FileInfoExtractWin::StartCreateTreeChildSlots,Qt::QueuedConnection);
    connect(this,static_cast<void (FileInfoExtractWin::*)()>(&FileInfoExtractWin::CreateTreeCmopete), this,&FileInfoExtractWin::CreateTreeCmopeteSlots
            ,Qt::QueuedConnection);
    // connect(this,static_cast<void (FileCompareDiffWin::*)(QTreeWidgetItem *)>(&FileCompareDiffWin::AssestTreeItem), this,&FileCompareDiffWin::AssestTreeItemSlots
    //         ,Qt::QueuedConnection);
}

void FileInfoExtractWin::InitLabel()
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

void FileInfoExtractWin::InitTextEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->textEdit->setFont(font);

    connect(this,static_cast<void (FileInfoExtractWin::*)(const QString &)>(&FileInfoExtractWin::AppendPossessLog), this,&FileInfoExtractWin::AppendPossessLogSlots
            ,Qt::QueuedConnection);
}

void FileInfoExtractWin::InitTableWidget()
{
    QStringList labels;
    labels<<tr("项目")<<tr("结果")<<tr("耗时");

    //设置列数
    ui->tableWidget->setColumnCount(3);
    //ui->tableWidget_2->setColumnCount(4);

    ui->tableWidget->verticalHeader()->hide();
    // ui->tableWidget_2->verticalHeader()->hide();

    //创建表头
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    //ui->tableWidget_2->setHorizontalHeaderLabels(labels);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //或
    //tableWidget->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Fixed);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    //ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行

    //设置选中行的背景色，必须要显示设置，即代码设置或者在ui文件的控件中设置，用系统默认的是无法代码选中高亮某行
    ui->tableWidget->setStyleSheet("selection-background-color:rgb(255,209,128)");
    //ui->tableWidget_2->setStyleSheet("selection-background-color:rgb(255,209,128)");

    // connect(ui->tableWidget,static_cast<void (QTableWidget::*)(int,int)>(&QTableWidget::cellClicked),this,[this](int row, int column){
    //     ui->tableWidget_2->selectRow(row);
    // });
    // connect(ui->tableWidget_2,static_cast<void (QTableWidget::*)(int,int)>(&QTableWidget::cellClicked),this,[this](int row, int column){
    //     ui->tableWidget->selectRow(row);
    // });


    connect(this,static_cast<void (FileInfoExtractWin::*)(const QString&)>(&FileInfoExtractWin::ClearTableWidget),
            this,&FileInfoExtractWin::ClearTableWidgetSlots,Qt::QueuedConnection);

    connect(this,static_cast<void (FileInfoExtractWin::*)()>(&FileInfoExtractWin::AppendFileInfoMapList),
            this,&FileInfoExtractWin::AppendFileInfoMapListSlots,Qt::QueuedConnection);

    connect(this,static_cast<void (FileInfoExtractWin::*)(const QString&,int,bool,const QString&,const QString&,const QString&)>(&FileInfoExtractWin::UpDateTableWidget),
            this,&FileInfoExtractWin::UpDateTableWidgetSlots,Qt::QueuedConnection);
}

void FileInfoExtractWin::InitTimer()
{
    pTimer=new QTimer();
    pTimer->setInterval(100);
    connect(pTimer,&QTimer::timeout,this,&FileInfoExtractWin::OnTimeOut);

    connect(this,static_cast<void (FileInfoExtractWin::*)(bool)>(&FileInfoExtractWin::TimerIsEnable),
            this,&FileInfoExtractWin::TimerIsEnableSlots,Qt::QueuedConnection);
}

void FileInfoExtractWin::LoadWinStatus()
{
    FileDirPath=DataOperate::Instance()->GetIniFileNode(tr("FileInfoExtractWin"),tr("FileDirPath"), tr(""));

    ui->lineEdit->setText(FileDirPath);
}

void FileInfoExtractWin::SetTBtnIsEnable(bool enable)
{
    int ToolButtonListSize=ToolButtonList.size();
    for (int index = 0; index < ToolButtonListSize; ++index) {
        ToolButtonList[index]->setEnabled(enable);
    }
}

void FileInfoExtractWin::SelectFileOrDirPathFun(QString flag)
{
    QString fileName = QFileDialog::getExistingDirectory();
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
        qDebug()<<"Failed to open the file!";
        return;
    }

    if(flag==QString("SelectDirPathTBtn")){
        FileDirPath=fileName;
        ui->lineEdit->setText(fileName);
        qDebug()<<QString("选择的文件夹路径：%1").arg(fileName);
        DataOperate::Instance()->WriteIniFile(tr("FileInfoExtractWin"),tr("FileDirPath"),fileName);
    }
}

void FileInfoExtractWin::CalculateFun()
{
    emit AppendPossessLog(tr("------------开始创建文件索引树及文件信息计算------------------------"));
    FoEachDirAsyn();
    emit AppendPossessLog(tr("------------创建文件索引树及文件信息计算完成------------------------"));
}

void FileInfoExtractWin::FoEachDirAsyn()
{
    emit AppendPossessLog(tr("正在处理中！！！"));
    QTreeWidgetItem* root = new QTreeWidgetItem();
    int sleepCount=0;

    //qDebug()<<"root before: "<<root;
    emit StartCreateTree(root,tr("StartDir"),tr("dir"),FileDirPath);
    //qDebug()<<"root after: "<<root;

    FoEachDirAsyn(FileDirPath,root,sleepCount);
    emit CreateTreeCmopete();//文件树创建完成
}

void FileInfoExtractWin::FoEachDirAsyn(QString dirPath, QTreeWidgetItem *root, int sleepCount)
{
    // 判断路径是否存在
    QDir dir(dirPath);
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Files |QDir::Dirs | QDir::NoDotAndDotDot );
    QFileInfoList list = dir.entryInfoList();

    int dirCount = list.count();
    for(int index=0; index<dirCount; index++){

        if(sleepCount>=10){//10个休息下给UI喘息
            sleepCount=0;
            QThread::msleep(100);
            //QThread::msleep(3000);
        }else{
            sleepCount++;
        }

        QFileInfo fileInfo = list.at(index);

        QString filePath=fileInfo.absolutePath();
        QString fileName=fileInfo.fileName();
        QString fileAbPath=fileInfo.absoluteFilePath();

        QTreeWidgetItem *childEml = new QTreeWidgetItem();

        if(fileInfo.isFile()){
            emit StartCreateTreeChild(root,childEml,fileName,tr("file"),filePath);
            FileCalExtract(fileAbPath);
        }
        if (fileInfo.isDir()) {
            emit StartCreateTreeChild(root,childEml,fileName,tr("dir"),filePath);
            FoEachDirAsyn(fileInfo.absoluteFilePath(),childEml,sleepCount);
        }
    }
}

void FileInfoExtractWin::UpDatePossess()
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
}

void FileInfoExtractWin::UpTableWidgetPossess(const QString &possessStr)
{
    int rowCount= ui->tableWidget->rowCount()-1;

    ui->tableWidget->item(rowCount,2)->setText(possessStr);
}

void FileInfoExtractWin::FileCalExtract(const QString &filePath)
{
    QStringList itemList;itemList.clear();
    itemList<<tr("文件名")<<tr("文件大小")<<tr("创建时间")<<tr("修改时间")<<tr("读取时间")<<tr("MD5")<<tr("SHA1")<<tr("SHA512");

    QString absoluteFilePath=GetFileBaseInfo(filePath,"absoluteFilePath");
    QString curCalFile=absoluteFilePath.replace(FileDirPath,"");
    emit SetCurCalFile(curCalFile);
    emit ClearTableWidget("tableWidget");

    int itemListSize=itemList.size();
    for (int index = 0; index < itemListSize; ++index) {
        // if(GetIsCancel()==true){//看情况提前结束
        //     break;
        // }

        emit TimerIsEnable(true);

        auto start = std::chrono::high_resolution_clock::now();

        QString itemName=itemList[index];
        QString itemResult="";
        QString itemStatus="进行中";
        //开始计算
        emit UpDateTableWidget("tableWidget",index,true,itemName,itemResult,"");

        if(itemList[index]==tr("文件名")){
            itemResult=GetFileBaseInfo(filePath,"fileName");
        }
        if(itemList[index]==tr("文件大小")){
            itemResult=GetFileBaseInfo(filePath,"size")+" Byte";
        }
        if(itemList[index]==tr("创建时间")){
            itemResult=GetFileBaseInfo(filePath,"birthTime");
        }
        if(itemList[index]==tr("修改时间")){
            itemResult=GetFileBaseInfo(filePath,"lastModified");
        }
        if(itemList[index]==tr("读取时间")){
            itemResult=GetFileBaseInfo(filePath,"lastRead");
        }

        if(itemList[index]==tr("MD5")){
            itemResult=GetFileHashStr(filePath,QCryptographicHash::Md5);
        }
        if(itemList[index]==tr("SHA1")){
            itemResult=GetFileHashStr(filePath,QCryptographicHash::Sha1);
        }
        if(itemList[index]==tr("SHA512")){
            itemResult=GetFileHashStr(filePath,QCryptographicHash::Sha512);
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        QString durationStr=CovDuration(duration);

        //计算完成
        emit UpDateTableWidget("tableWidget",index,false,itemName,itemResult,durationStr);

        emit TimerIsEnable(false);
    }

    emit AppendFileInfoMapList();
}

QString FileInfoExtractWin::GetFileBaseInfo(const QString &filePath, const QString &flag)
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
    if(flag=="absoluteFilePath")
        retValue=fileInfo.absoluteFilePath();

    return retValue;
}

QString FileInfoExtractWin::GetFileHashStr(const QString &fileName, QCryptographicHash::Algorithm algorithm)
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

QString FileInfoExtractWin::CovDuration(long long duration)
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

bool FileInfoExtractWin::FindFileInfoMapList(const QString &key, FileInfoExtract &fileInfoExtract)
{
    bool isFind=false;
    QMap<QString, FileInfoExtract>::const_iterator iterator = FileInfoMapList.constBegin();
    while (iterator != FileInfoMapList.constEnd()) {
        if(iterator.key()==key){
            fileInfoExtract=iterator.value();
            isFind=true;
            break;
        }
        ++iterator;
    }
    return isFind;
}

void FileInfoExtractWin::RefshTableWidget(const FileInfoExtract &fileInfoExtract)
{
    QStringList itemList;itemList.clear();
    itemList<<tr("文件名")<<tr("文件大小")<<tr("创建时间")<<tr("修改时间")<<tr("读取时间")<<tr("MD5")<<tr("SHA1")<<tr("SHA512");
    QStringList itemResultList;itemResultList.clear();
    itemResultList<<fileInfoExtract.FileName.Result<<fileInfoExtract.Size.Result<<fileInfoExtract.BirthTime.Result<<fileInfoExtract.LastModified.Result<<fileInfoExtract.LastRead.Result<<fileInfoExtract.MD5.Result
                   <<fileInfoExtract.SHA1.Result<<fileInfoExtract.SHA512.Result;
    QStringList durationList;durationList.clear();
    durationList<<fileInfoExtract.FileName.Duration<<fileInfoExtract.Size.Duration<<fileInfoExtract.BirthTime.Duration<<fileInfoExtract.LastModified.Duration<<fileInfoExtract.LastRead.Duration<<fileInfoExtract.MD5.Duration
                   <<fileInfoExtract.SHA1.Duration<<fileInfoExtract.SHA512.Duration;

    while (ui->tableWidget->rowCount()!=0) {
        ui->tableWidget->removeRow(0);
    }

    QFont font("Microsoft YaHei", 8, QFont::Bold);
    int rowCount=itemList.size();
    for (int row = 0; row < rowCount; ++row) {
        if(itemResultList[row]==""||durationList[row]==""){
            break;
        }

        ui->tableWidget->insertRow(row);//增加一行
        QStringList LIST;LIST.clear();
        LIST<<itemList[row]<<itemResultList[row]<<durationList[row];
        int LISTSize=LIST.size();
        for (int col = 0; col < LISTSize; ++col) {
            ui->tableWidget->setItem(row,col,new QTableWidgetItem(LIST[col]));
            ui->tableWidget->item(row,col)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(row,col)->setFont(font);
        }
    }
}

void FileInfoExtractWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="SelectDirPathTBtn"){
        SelectFileOrDirPathFun("SelectDirPathTBtn");
    }
    else if(ToolButton->objectName()=="CalculateTBtn"){
        auto ret= QtConcurrent::run([this](){
            CalculateFun();
            return;
        });
    }
}

void FileInfoExtractWin::SetTBtnIsEnableSlots(bool enable)
{
    SetTBtnIsEnable(enable);
}

void FileInfoExtractWin::AppendPossessLogSlots(const QString &logStr)
{
    ui->textEdit->append(logStr);
}

void FileInfoExtractWin::StartCreateTreeSlots(QTreeWidgetItem *root, QString name, QString type, QString path)
{
    PrechildEml=nullptr;
    ui->treeWidget->clear();

    //root = new QTreeWidgetItem(ui->treeWidget);
    ui->treeWidget->addTopLevelItem(root);
    root->setText(0,name);
    root->setText(1,type);
    root->setText(2,path);

    //qDebug()<<"root: "<<root;

    //root->setCheckState(0, Qt::Unchecked);

    ui->treeWidget->expandAll();

    FileInfoMapList.clear();
    CurCalFile="";
    CurSelectFile="";
}

void FileInfoExtractWin::StartCreateTreeChildSlots(QTreeWidgetItem *root, QTreeWidgetItem *childEml, QString name, QString type, QString path)
{
    root->addChild(childEml);
    childEml->setText(0,name);
    childEml->setText(1,type);
    childEml->setText(2,path);
    //childEml->setCheckState(0, Qt::Unchecked);
    //childEml->setSelected(true);

    //重置先前
    if(PrechildEml==nullptr){
    }else{
        PrechildEml->setBackground(0,QColor(255,255,255));
        PrechildEml->setBackground(1,QColor(255,255,255));
        PrechildEml->setBackground(2,QColor(255,255,255));
    }

    //设置当前
    childEml->setBackground(0,QColor(0,191,255));
    childEml->setBackground(1,QColor(0,191,255));
    childEml->setBackground(2,QColor(0,191,255));

    PrechildEml=childEml;

    ui->treeWidget->setCurrentItem(childEml);
    ui->treeWidget->scrollToItem(ui->treeWidget->currentItem());
}

void FileInfoExtractWin::CreateTreeCmopeteSlots()
{
    PrechildEml=nullptr;
    ui->treeWidget->expandAll();
}

void FileInfoExtractWin::ClearTableWidgetSlots(const QString &upDateTableWidget)
{
    if(CurSelectFile!=""&&CurSelectFile!=CurCalFile){
        return;
    }

    if(upDateTableWidget=="tableWidget"){
        while (ui->tableWidget->rowCount()!=0) {
            ui->tableWidget->removeRow(0);
        }
        //ui->tableWidget->clear();
    }
}

void FileInfoExtractWin::UpDateTableWidgetSlots(const QString &upDateTableWidget, int rowIndex, bool isNewAdd, const QString &itemName, const QString &itemResult, const QString &duration)
{
    if(isNewAdd==false){
        if(tr("文件名")==itemName){
            CurCalFileInfoExtract.FileName=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("文件大小")==itemName){
            CurCalFileInfoExtract.Size=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("创建时间")==itemName){
            CurCalFileInfoExtract.BirthTime=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("修改时间")==itemName){
            CurCalFileInfoExtract.LastModified=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("读取时间")==itemName){
            CurCalFileInfoExtract.LastRead=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("MD5")==itemName){
            CurCalFileInfoExtract.MD5=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("SHA1")==itemName){
            CurCalFileInfoExtract.SHA1=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("SHA512")==itemName){
            CurCalFileInfoExtract.SHA512=FileInfoExtractBase(itemResult,duration);
        }
    }

    if(CurSelectFile!=""&&CurSelectFile!=CurCalFile){
        return;
    }

    QFont font("Microsoft YaHei", 8, QFont::Bold);
    if(upDateTableWidget=="tableWidget"){
        if(isNewAdd==true){
            ui->tableWidget->insertRow(rowIndex);//增加一行
            QStringList LIST;LIST.clear();
            LIST<<itemName<<itemResult<<duration;
            int LISTSize=LIST.size();
            for (int col = 0; col < LISTSize; ++col) {
                ui->tableWidget->setItem(rowIndex,col,new QTableWidgetItem(LIST[col]));
                ui->tableWidget->item(rowIndex,col)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->tableWidget->item(rowIndex,col)->setFont(font);
            }

            ui->tableWidget->selectRow(rowIndex);

        }else{
            QStringList LIST;LIST.clear();
            LIST<<itemName<<itemResult<<duration;
            int LISTSize=LIST.size();
            for (int col = 0; col < LISTSize; ++col) {
                ui->tableWidget->item(rowIndex,col)->setText(LIST[col]);
            }
        }
    }
}

void FileInfoExtractWin::AppendFileInfoMapListSlots()
{
    FileInfoExtract fileInfoExtract;
    int rowCount=ui->tableWidget->rowCount();
    for (int index = 0; index < rowCount; ++index) {
        QString itemName=ui->tableWidget->item(index,0)->text();
        QString itemResult=ui->tableWidget->item(index,1)->text();
        QString duration=ui->tableWidget->item(index,2)->text();
        if(tr("文件名")==itemName){
            fileInfoExtract.FileName=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("文件大小")==itemName){
            fileInfoExtract.Size=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("创建时间")==itemName){
            fileInfoExtract.BirthTime=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("修改时间")==itemName){
            fileInfoExtract.LastModified=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("读取时间")==itemName){
            fileInfoExtract.LastRead=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("MD5")==itemName){
            fileInfoExtract.MD5=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("SHA1")==itemName){
            fileInfoExtract.SHA1=FileInfoExtractBase(itemResult,duration);
        }
        if(tr("SHA512")==itemName){
            fileInfoExtract.SHA512=FileInfoExtractBase(itemResult,duration);
        }
    }

    FileInfoMapList.insert(CurCalFile,fileInfoExtract);
}

void FileInfoExtractWin::TimerIsEnableSlots(bool enable)
{
    if(enable==true){
        NCount=0;
        pTimer->start();
    }
    if(enable==false){
        NCount=0;
        pTimer->stop();
    }
}

void FileInfoExtractWin::OnTimeOut()
{
    if(CurSelectFile!=""&&CurSelectFile!=CurCalFile){
        return;
    }

    UpDatePossess();
}

void FileInfoExtractWin::SetCurCalFileSlots(const QString &curCalFile)
{
    CurCalFile=curCalFile;
}

void FileInfoExtractWin::TreeWidgetItemPressed(QTreeWidgetItem *item, int column)
{
    QString fileName=item->text(0);
    QString filePath=item->text(2);
    QString absoluteFilePath=filePath+"/"+fileName;

    CurSelectFile=absoluteFilePath.replace(FileDirPath,"");
    FileInfoExtract fileInfoExtract;
    // bool isFind=FindFileInfoMapList(CurSelectFile, fileInfoExtract);
    // if(){
    // }

    // if(CurSelectFile==CurCalFile){
    //     fileInfoExtract=CurCalFileInfoExtract;
    // }else{
    //     FindFileInfoMapList(CurSelectFile, fileInfoExtract);
    // }

    //FindFileInfoMapList(CurSelectFile, fileInfoExtract);
    RefshTableWidget(fileInfoExtract);
}
