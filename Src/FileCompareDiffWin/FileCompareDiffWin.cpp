# pragma execution_character_set("utf-8")
#include "FileCompareDiffWin.h"
#include "ui_FileCompareDiffWin.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrent>
#include <QMenu>
#include <QDesktopServices>

#include "Src/DataManage/DataOperate.h"
#include "Src/FileCompareDiffWin/FileCompareDiffDetailsWin.h"

FileCompareDiffWin::FileCompareDiffWin(QWidget *parent)
    : WidgetBase(parent)
    , ui(new Ui::FileCompareDiffWin)
{
    ui->setupUi(this);
}

FileCompareDiffWin::~FileCompareDiffWin()
{
    delete ui;
}

void FileCompareDiffWin::InitClass()
{
    InitGroupBox();
    InitLineEdit();
    InitToolButton();
    InitTreeWidget();
    InitLabel();
    InitTextEdit();

    LoadWinStatus();
}

void FileCompareDiffWin::InitGroupBox()
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

void FileCompareDiffWin::InitLineEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);
    ui->lineEdit_3->setFont(font);
    ui->lineEdit_4->setFont(font);

    ui->lineEdit->setPlaceholderText(tr("文件(夹)1路径"));
    ui->lineEdit_2->setPlaceholderText(tr("文件(夹)2路径"));
    ui->lineEdit_3->setPlaceholderText(tr("临时路径"));
    ui->lineEdit_4->setPlaceholderText(tr("整理文件路径"));

    ui->lineEdit->setEnabled(false);ui->lineEdit_2->setEnabled(false);ui->lineEdit_3->setEnabled(false);ui->lineEdit_4->setEnabled(false);
}

void FileCompareDiffWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->OpenCompareTBtn<<ui->SelectFileOrDirPath1<<ui->SelectFileOrDirPath2<<ui->FilesArrangerTBtn
                   <<ui->SelectTempDirPath<<ui->SelectArrangerDirPath<<ui->TestTBtn;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】开启文件(夹)对比")<<tr("【点击】选择文件(夹)1路径")<<tr("【点击】选择文件(夹)2路径")<<tr("【点击】进行运行文件整理")
               <<tr("【点击】选择临时路径")<<tr("【点击】选择整理文件路径") <<tr("XXX");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&FileCompareDiffWin::ToolButtonClicked);
    }

    connect(this,static_cast<void (FileCompareDiffWin::*)(bool)>(&FileCompareDiffWin::SetTBtnIsEnableSignal), this,&FileCompareDiffWin::SetTBtnIsEnableSlots);
}

void FileCompareDiffWin::InitTreeWidget()
{
    // 设置QTreeWidget的列数
    ui->treeWidget->setColumnCount(7);
    ui->treeWidget->setHeaderLabels({tr("文件名"),tr("类型"),tr("路径"),tr("初始"),tr("文件(夹)1存在?"),tr("文件(夹)2存在?"),tr("对比结果")});
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //自动展开
    ui->treeWidget->setItemsExpandable(true);
    ui->treeWidget->expandAll();
    //ui->treeWidget->hideColumn(2);

    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemChanged), this,&FileCompareDiffWin::TreeWidgetItemChanged);
    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemClicked), this,&FileCompareDiffWin::TreeWidgetItemClicked);
    connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemPressed), this,&FileCompareDiffWin::TreeWidgetItemPressed);

    //connect(ui->treeWidget,static_cast<void (QTreeWidget::*)(QTreeWidgetItem *,int)>(&QTreeWidget::itemPressed), this,&TsFileExportWin::TreeWidgetItemPressed);

    connect(this,static_cast<void (FileCompareDiffWin::*)(QTreeWidgetItem *,QString,QString,QString)>(&FileCompareDiffWin::StartCreateTree), this,&FileCompareDiffWin::StartCreateTreeSlots
            ,Qt::QueuedConnection);
    connect(this,static_cast<void (FileCompareDiffWin::*)(QTreeWidgetItem *,QTreeWidgetItem*,QString,QString,QString,QString,QString,QString,QString)>(&FileCompareDiffWin::StartCreateTreeChild),
            this,&FileCompareDiffWin::StartCreateTreeChildSlots,Qt::QueuedConnection);
    connect(this,static_cast<void (FileCompareDiffWin::*)()>(&FileCompareDiffWin::CreateTreeCmopete), this,&FileCompareDiffWin::CreateTreeCmopeteSlots
            ,Qt::QueuedConnection);
    connect(this,static_cast<void (FileCompareDiffWin::*)(QTreeWidgetItem *)>(&FileCompareDiffWin::AssestTreeItem), this,&FileCompareDiffWin::AssestTreeItemSlots
            ,Qt::QueuedConnection);
}

void FileCompareDiffWin::InitLabel()
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

void FileCompareDiffWin::InitTextEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->textEdit->setFont(font);

    connect(this,static_cast<void (FileCompareDiffWin::*)(const QString &)>(&FileCompareDiffWin::AppendPossessLog), this,&FileCompareDiffWin::AppendPossessLogSlots
            ,Qt::QueuedConnection);
}

void FileCompareDiffWin::LoadWinStatus()
{
    FileOrDirPath1=DataOperate::Instance()->GetIniFileNode(tr("FileCompareDiffWinGroup"),tr("FileOrDirPath1"), tr(""));
    FileOrDirPath2=DataOperate::Instance()->GetIniFileNode(tr("FileCompareDiffWinGroup"),tr("FileOrDirPath2"), tr(""));
    TargetTempDir=DataOperate::Instance()->GetIniFileNode(tr("FileCompareDiffWinGroup"),tr("TargetTempDir"), tr(""));
    FilesArranger=DataOperate::Instance()->GetIniFileNode(tr("FileCompareDiffWinGroup"),tr("FilesArranger"), tr(""));

    ui->lineEdit->setText(FileOrDirPath1);
    ui->lineEdit_2->setText(FileOrDirPath2);
    ui->lineEdit_3->setText(TargetTempDir);
    ui->lineEdit_4->setText(FilesArranger);
}

void FileCompareDiffWin::OpenCompareFun()
{

    ui->textEdit->clear();

    //同步的方式
    //OpenCompareSynchronousFun();

    //异步的方式
    auto ret= QtConcurrent::run([this](){
        emit SetTBtnIsEnableSignal(false);
        OpenCompareAsynchronousFun();
        emit SetTBtnIsEnableSignal(true);
        return;
    });
}

void FileCompareDiffWin::OpenCompareSynchronousFun()
{
    //开启比较第一步
    OpenCompareSynchronousStep1New();
    //开启比较第二步
    OpenCompareSynchronousStep2New();
    //开启比较第三步
    OpenCompareSynchronousStep3New();
}

void FileCompareDiffWin::OpenCompareAsynchronousFun()
{
    //开启比较第一步
    OpenCompareAsynchronousStep1New();
    //开启比较第二步
    OpenCompareAsynchronousStep2New();
    //开启比较第三步
    OpenCompareAsynchronousStep3New();
}

void FileCompareDiffWin::SelectFileOrDirPathFun(QString flag)
{
    // QString fileName = QFileDialog::getOpenFileName(
    //     this,
    //     tr("open a file or dir."),
    //     "D:/",
    //     tr("All files(*.*)"));

    QString fileName = QFileDialog::getExistingDirectory();
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
        qDebug()<<"Failed to open the file!";
        return;
    }

    if(flag==QString("SelectFileOrDirPath1")){
        FileOrDirPath1=fileName;
        ui->lineEdit->setText(fileName);
        qDebug()<<QString("选择的文件(夹)1路径：%1").arg(fileName);
        DataOperate::Instance()->WriteIniFile(tr("FileCompareDiffWinGroup"),tr("FileOrDirPath1"),fileName);
    }
    else if(flag==QString("SelectFileOrDirPath2")){
        FileOrDirPath2=fileName;
        ui->lineEdit_2->setText(fileName);
        qDebug()<<QString("选择的文件(夹)2路径：%1").arg(fileName);
        DataOperate::Instance()->WriteIniFile(tr("FileCompareDiffWinGroup"),tr("FileOrDirPath2"),fileName);
    }
    else if(flag==QString("SelectTempDirPath")){
        TargetTempDir=fileName;
        ui->lineEdit_3->setText(fileName);
        qDebug()<<QString("选择的临时路径：%1").arg(fileName);
        DataOperate::Instance()->WriteIniFile(tr("FileCompareDiffWinGroup"),tr("TargetTempDir"),fileName);
    }
    else if(flag==QString("SelectArrangerDirPath")){
        FilesArranger=fileName;
        ui->lineEdit_4->setText(fileName);
        qDebug()<<QString("选择的整理文件路径：%1").arg(fileName);
        DataOperate::Instance()->WriteIniFile(tr("FileCompareDiffWinGroup"),tr("FilesArranger"),fileName);
    }
}

void FileCompareDiffWin::FilesArrangerFun()
{
    int itemCount = ui->treeWidget->topLevelItemCount();
    for(int i=0;i<itemCount;i++){
        QTreeWidgetItem *childItem = ui->treeWidget->topLevelItem(i);
        FoEachTree(childItem);
    }
}

void FileCompareDiffWin::FoEachTree(QTreeWidgetItem *root)
{
    //可能树节点过于多了这样怕UI线程卡死这样缓解ui压力
    int itemCount = root->childCount();
    for(int i=0;i<itemCount;i++)
    {
        QThread::msleep(10);
        QTreeWidgetItem *childItem = root->child(i);
        if(childItem->childCount()!=0)
        {
            FoEachTree(childItem);
        }
        else
        {
            //Qt创建的子线程中是不能对UI对象进行任何操作的，即QWidget及其派生类对象，这个是我掉的第一个坑。可能是由于考虑到安全性的问题，
            //所以Qt中子线程不能执行任何关于界面的处理，包括消息框的弹出。正确的操作应该是通过信号槽，将一些参数传递给主线程，让主线程（也就是Controller）去处理。
            //qDebug()<<childItem->text(0);
            emit AssestTreeItem(childItem);
        }
    }
}

void FileCompareDiffWin::OpenCompareSynchronousStep1New()
{
    FoEachDirAndCreateIndex(FileOrDirPath1,"1");
    FoEachDirAndCreateIndex(FileOrDirPath2,"2");
}

void FileCompareDiffWin::FoEachDirAndCreateIndex(QString dirPath,QString flag)
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

        QString absoluteFilePath=fileInfo.absoluteFilePath();
        QString targetFileFullPath="";
        if(flag=="1")
            targetFileFullPath=absoluteFilePath.replace(FileOrDirPath1,TargetTempDir);
        if(flag=="2")
            targetFileFullPath=absoluteFilePath.replace(FileOrDirPath2,TargetTempDir);

        if(fileInfo.isFile()){
            bool exist = QFile::exists(targetFileFullPath);
            if(exist==true)
                continue;

            QFileInfo fileInfo1(targetFileFullPath);
            QDir dir(fileInfo1.absolutePath());
            if (!dir.exists()) {
                dir.mkpath(fileInfo1.absolutePath());
            }

            QFile file(targetFileFullPath);
            //打开文件，不存在则创建
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            file.close();
        }
        if (fileInfo.isDir()) {
            FoEachDirAndCreateIndex(fileInfo.absoluteFilePath(),flag);

            QDir dir(targetFileFullPath);
            if (dir.exists()==true) {
                continue;
            }

            dir.mkpath(targetFileFullPath);
        }
    }
}

void FileCompareDiffWin::OpenCompareSynchronousStep2New()
{
    FoEachDirTemp();
}

void FileCompareDiffWin::FoEachDirTemp()
{
    ui->treeWidget->clear();

    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0,tr("StartDir"));
    root->setText(1,tr("dir"));
    //root->setText(2,TargetTempDir);

    //root->setCheckState(0, Qt::Unchecked);

    FoEachDirTemp(TargetTempDir,root);

    ui->treeWidget->expandAll();
}

void FileCompareDiffWin::FoEachDirTemp(QString dirPath, QTreeWidgetItem *root)
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

        QString absoluteFilePath1=fileInfo.absoluteFilePath();
        QString absoluteFilePath2=fileInfo.absoluteFilePath();

        QString realAbsoluteFilePath1=absoluteFilePath1.replace(TargetTempDir,FileOrDirPath1);
        QString realAbsoluteFilePath2=absoluteFilePath2.replace(TargetTempDir,FileOrDirPath2);

        QString filePath="";
        QString fileName="";
        QString FileOr="";

        QTreeWidgetItem* childEml = new QTreeWidgetItem(root);

        QFileInfo fileInfo1(realAbsoluteFilePath1);
        QFileInfo fileInfo2(realAbsoluteFilePath2);

        if(fileInfo1.exists()==true&&fileInfo2.exists()==true)//存在1  存在2
        {
            filePath=fileInfo1.absolutePath();
            fileName=fileInfo1.fileName();
            FileOr="1/2";
            childEml->setText(4,tr("是"));
            childEml->setText(5,tr("是"));
            //这里得额外判断到底是文件还是文件夹//最终出childEml->setText(6,tr("不匹配"));结果
        }

        if(fileInfo1.exists()==true&&fileInfo2.exists()==false)//存在1  不存在2
        {
            filePath=fileInfo1.absolutePath();
            fileName=fileInfo1.fileName();
            FileOr="1";
            childEml->setText(4,tr("是"));
            childEml->setText(5,tr("否"));
            childEml->setText(6,tr("不匹配"));
            childEml->setForeground(4,Qt::red);//可将字体颜色变为绿色,更详细的设置请看QBrush
            childEml->setForeground(5,Qt::red);
            childEml->setForeground(6,Qt::red);
        }

        if(fileInfo1.exists()==false&&fileInfo2.exists()==true)//不存在1  存在2
        {
            filePath=fileInfo2.absolutePath();
            fileName=fileInfo2.fileName();
            FileOr="2";
            childEml->setText(4,tr("否"));
            childEml->setText(5,tr("是"));
            childEml->setText(6,tr("不匹配"));
            childEml->setForeground(4,Qt::red);//可将字体颜色变为绿色,更详细的设置请看QBrush
            childEml->setForeground(5,Qt::red);
            childEml->setForeground(6,Qt::red);
        }

        childEml->setText(0,fileName);
        childEml->setText(2,filePath);
        //childEml->setCheckState(0, Qt::Unchecked);
        childEml->setText(3,FileOr);

        if(fileInfo.isFile()){
            childEml->setText(1,tr("file"));
        }
        if (fileInfo.isDir()) {
            childEml->setText(1,tr("dir"));
            FoEachDirTemp(fileInfo.absoluteFilePath(),childEml);
        }
    }
}

void FileCompareDiffWin::OpenCompareSynchronousStep3New()
{
    QString targetTempDir="D:/Users/DJL/Desktop/FurMark_2.3.0.0_win64/Temp";
    FoEachDirAndDeleteIndex(targetTempDir);
}

void FileCompareDiffWin::FoEachDirAndDeleteIndex(QString dirPath)
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

        if(fileInfo.isFile()){
            fileInfo.dir().remove(fileInfo.fileName());
        }
        if (fileInfo.isDir()) {//递归删除
            FoEachDirAndDeleteIndex(fileInfo.absoluteFilePath());
            fileInfo.dir().rmdir(fileInfo.absoluteFilePath());
        }
    }

    //删除根目录
    QFileInfo fileInfo(dirPath);
    fileInfo.dir().rmdir(fileInfo.absoluteFilePath());
}

void FileCompareDiffWin::OpenCompareAsynchronousStep1New()
{
    emit AppendPossessLog(tr("------------开始创建文件索引------------------------"));
    FoEachDirAndCreateIndex(FileOrDirPath1,"1");
    FoEachDirAndCreateIndex(FileOrDirPath2,"2");
    emit AppendPossessLog(tr("------------创建文件索引完成------------------------"));
}

void FileCompareDiffWin::OpenCompareAsynchronousStep2New()
{
    emit AppendPossessLog(tr("------------开始创建文件索引树及文件比较------------------------"));
    FoEachDirTempAsyn();
    emit AppendPossessLog(tr("------------创建文件索引树及文件比较完成------------------------"));
}

void FileCompareDiffWin::FoEachDirTempAsyn()
{
    emit AppendPossessLog(tr("正在对比中！！！"));
    QString targetTempDir="D:/Users/DJL/Desktop/FurMark_2.3.0.0_win64/Temp";
    QTreeWidgetItem* root = new QTreeWidgetItem();
    int sleepCount=0;

    //qDebug()<<"root before: "<<root;
    emit StartCreateTree(root,tr("StartDir"),tr("dir"),targetTempDir);
    //qDebug()<<"root after: "<<root;

    FoEachDirTempAsyn(targetTempDir,root,sleepCount);
    emit CreateTreeCmopete();//文件树创建完成
}

void FileCompareDiffWin::FoEachDirTempAsyn(QString dirPath, QTreeWidgetItem *root,int sleepCount)
{
    // 判断路径是否存在
    QDir dir(dirPath);
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Files |QDir::Dirs | QDir::NoDotAndDotDot );
    QFileInfoList list = dir.entryInfoList();

    QString targetTempDir="D:/Users/DJL/Desktop/FurMark_2.3.0.0_win64/Temp";

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

        QString absoluteFilePath1=fileInfo.absoluteFilePath();
        QString absoluteFilePath2=fileInfo.absoluteFilePath();

        QString realAbsoluteFilePath1=absoluteFilePath1.replace(targetTempDir,FileOrDirPath1);
        QString realAbsoluteFilePath2=absoluteFilePath2.replace(targetTempDir,FileOrDirPath2);

        QString filePath="";
        QString fileName="";
        QString FileOr="";
        QString dir1IsExist="";
        QString dir2IsExist="";
        QString result="";


        QFileInfo fileInfo1(realAbsoluteFilePath1);
        QFileInfo fileInfo2(realAbsoluteFilePath2);

        if(fileInfo1.exists()==true&&fileInfo2.exists()==true)//存在1  存在2
        {
            filePath=fileInfo1.absolutePath();
            fileName=fileInfo1.fileName();
            FileOr="1/2";
            dir1IsExist=tr("是");
            dir2IsExist=tr("是");
            //这里得额外判断到底是文件还是文件夹//最终出childEml->setText(6,tr("不匹配"));结果
            if(fileInfo1.isFile()&&fileInfo2.isFile()){//文件校验两个文件是否一样
                if(JudgeTowFileIsConsistent(realAbsoluteFilePath1,realAbsoluteFilePath2)==true){//暂时使用单个md5判断
                    result=tr("匹配");
                }else{
                    result=tr("不匹配");
                }
            }
            if (fileInfo1.isDir()&&fileInfo2.isDir()) {//文件夹一定一样
                result=tr("匹配");
            }
        }

        if(fileInfo1.exists()==true&&fileInfo2.exists()==false)//存在1  不存在2
        {
            filePath=fileInfo1.absolutePath();
            fileName=fileInfo1.fileName();
            FileOr="1";
            dir1IsExist=tr("是");
            dir2IsExist=tr("否");
            result=tr("不匹配");
        }

        if(fileInfo1.exists()==false&&fileInfo2.exists()==true)//不存在1  存在2
        {
            filePath=fileInfo2.absolutePath();
            fileName=fileInfo2.fileName();
            FileOr="2";
            dir1IsExist=tr("否");
            dir2IsExist=tr("是");
            result=tr("不匹配");
        }


        QTreeWidgetItem *childEml = new QTreeWidgetItem();

        if(fileInfo.isFile()){
            emit StartCreateTreeChild(root,childEml,fileName,tr("file"),filePath,FileOr,dir1IsExist,dir2IsExist,result);
        }
        if (fileInfo.isDir()) {
            emit StartCreateTreeChild(root,childEml,fileName,tr("dir"),filePath,FileOr,dir1IsExist,dir2IsExist,result);
            FoEachDirTempAsyn(fileInfo.absoluteFilePath(),childEml,sleepCount);
        }
    }
}

bool FileCompareDiffWin::JudgeTowFileIsConsistent(const QString &filePath1, const QString &filePath2)
{
    QString md5Str1 = GetFileHashStr(filePath1,QCryptographicHash::Md5);
    QString md5Str2 = GetFileHashStr(filePath2,QCryptographicHash::Md5);

    QString sha1Str1 = GetFileHashStr(filePath1,QCryptographicHash::Sha1);
    QString sha1Str2 = GetFileHashStr(filePath2,QCryptographicHash::Sha1);

    if(md5Str1==md5Str2&&sha1Str1==sha1Str2){
        return true;
    }else{
        return false;
    }
}

QString FileCompareDiffWin::GetFileHashStr(const QString &fileName, QCryptographicHash::Algorithm algorithm)
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

void FileCompareDiffWin::OpenCompareAsynchronousStep3New()
{
    emit AppendPossessLog(tr("------------开始清理过程数据------------------------"));
    QString targetTempDir="D:/Users/DJL/Desktop/FurMark_2.3.0.0_win64/Temp";
    FoEachDirAndDeleteIndex(targetTempDir);
    emit AppendPossessLog(tr("------------清理过程数据完成------------------------"));
}

void FileCompareDiffWin::SetTBtnIsEnable(bool enable)
{
    int ToolButtonListSize=ToolButtonList.size();
    for (int index = 0; index < ToolButtonListSize; ++index) {
        ToolButtonList[index]->setEnabled(enable);
    }
}

void FileCompareDiffWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="OpenCompareTBtn"){
        if(FileOrDirPath1==""||FileOrDirPath2==""||TargetTempDir==""){
            QMessageBox::warning(this, "Warning!", "请选择文件(夹)1路径、选择文件(夹)2路径、选择临时路径保证不为空！！！");
            return;
        }

        OpenCompareFun();
    }
    else if(ToolButton->objectName()=="SelectFileOrDirPath1"){
        emit SetTBtnIsEnableSignal(false);
        SelectFileOrDirPathFun(QString("SelectFileOrDirPath1"));
        emit SetTBtnIsEnableSignal(true);
    }
    else if(ToolButton->objectName()=="SelectFileOrDirPath2"){
        emit SetTBtnIsEnableSignal(false);
        SelectFileOrDirPathFun(QString("SelectFileOrDirPath2"));
        emit SetTBtnIsEnableSignal(true);
    }
    else if(ToolButton->objectName()=="SelectTempDirPath"){
        emit SetTBtnIsEnableSignal(false);
        SelectFileOrDirPathFun(QString("SelectTempDirPath"));
        emit SetTBtnIsEnableSignal(true);
    }
    else if(ToolButton->objectName()=="SelectArrangerDirPath"){
        emit SetTBtnIsEnableSignal(false);
        SelectFileOrDirPathFun(QString("SelectArrangerDirPath"));
        emit SetTBtnIsEnableSignal(true);
    }
    else if(ToolButton->objectName()=="FilesArrangerTBtn"){
        if(FilesArranger==""){
            QMessageBox::warning(this, "Warning!", "请选择整理文件路径保证不为空！！！");
            return;
        }

        auto ret= QtConcurrent::run([this](){
            emit SetTBtnIsEnableSignal(false);
            emit AppendPossessLog(tr("------------开始执行文件整理------------------------"));
            FilesArrangerFun();
            emit AppendPossessLog(tr("------------执行文件整理完成------------------------"));
            emit SetTBtnIsEnableSignal(true);
            return;
        });
    }
    else if(ToolButton->objectName()=="TestTBtn"){

        // auto ret= QtConcurrent::run([this](){
        //     auto start = std::chrono::high_resolution_clock::now();

        //     // Your code here
        //     // ...
        //     //QThread::msleep(10);.
        //     emit SetTBtnIsEnableSignal(true);

        //     auto stop = std::chrono::high_resolution_clock::now();
        //     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

        //     qDebug() << "Elapsed time: " << duration << " us\n";

        //     return;
        // });
    }
}

void FileCompareDiffWin::SetTBtnIsEnableSlots(bool enable)
{
    SetTBtnIsEnable(enable);
}

void FileCompareDiffWin::AppendPossessLogSlots(const QString &logStr)
{
    ui->textEdit->append(logStr);
}

void FileCompareDiffWin::StartCreateTreeSlots(QTreeWidgetItem *root, QString name, QString type, QString path)
{
    ui->treeWidget->clear();

    //root = new QTreeWidgetItem(ui->treeWidget);
    ui->treeWidget->addTopLevelItem(root);
    root->setText(0,name);
    root->setText(1,type);
    root->setText(2,path);

    //qDebug()<<"root: "<<root;

    //root->setCheckState(0, Qt::Unchecked);

    ui->treeWidget->expandAll();
}

void FileCompareDiffWin::StartCreateTreeChildSlots(QTreeWidgetItem *root, QTreeWidgetItem *childEml, QString name, QString type, QString path, QString Or, QString dir1IsExist, QString dir2IsExist, QString result)
{
    root->addChild(childEml);
    childEml->setText(0,name);
    childEml->setText(1,type);
    childEml->setText(2,path);
    //childEml->setCheckState(0, Qt::Unchecked);
    childEml->setText(3,Or);

    childEml->setText(4,dir1IsExist);
    childEml->setText(5,dir2IsExist);
    childEml->setText(6,result);

    // childEml->setForeground(4,Qt::red);//可将字体颜色变为绿色,更详细的设置请看QBrush
    // childEml->setForeground(5,Qt::red);
    if(result==tr("不匹配")){
        childEml->setForeground(6,Qt::red);
    }

    //qDebug()<<"root: "<<root<<"   childEml: "<<childEml;
}

void FileCompareDiffWin::CreateTreeCmopeteSlots()
{
    ui->treeWidget->expandAll();
}

void FileCompareDiffWin::AssestTreeItemSlots(QTreeWidgetItem *itemEml)
{
    QString fileName=itemEml->text(0);
    QString filePath=itemEml->text(2);
    QString OR =itemEml->text(3);
    QString result =itemEml->text(6);

    if(result==tr("匹配")){
        return;
    }

    QString FilesArrangerDir1=FilesArranger+"/文件(夹)1相对于文件(夹)2";
    QString FilesArrangerDir2=FilesArranger+"/文件(夹)2相对于文件(夹)1";

    QString fileAbsPath=filePath+"/"+fileName;

    if(OR=="1/2"){
        QString realFileAbsPath1=fileAbsPath;
        QString realFileAbsPath2=fileAbsPath.replace(FileOrDirPath1,FileOrDirPath2);

        QString tempRealFileAbsPath1=realFileAbsPath1;
        QString tempRealFileAbsPath2=realFileAbsPath2;

        QString targetFileAbsPath1=tempRealFileAbsPath1.replace(FileOrDirPath1,FilesArrangerDir1);
        QString targetFileAbsPath2=tempRealFileAbsPath2.replace(FileOrDirPath2,FilesArrangerDir2);

        QFileInfo fileInfo1(targetFileAbsPath1);
        QFileInfo fileInfo2(targetFileAbsPath2);

        QDir dir1(fileInfo1.absolutePath());
        if (dir1.exists()==false) {
            dir1.mkpath(fileInfo1.absolutePath());
        }
        QDir dir2(fileInfo2.absolutePath());
        if (dir2.exists()==false) {
            dir2.mkpath(fileInfo2.absolutePath());
        }

        bool ok1 = QFile::copy(realFileAbsPath1, targetFileAbsPath1);
        bool ok2 = QFile::copy(realFileAbsPath2, targetFileAbsPath2);

        if(ok1==true&&ok2==true){
        }

        emit AppendPossessLog(QString("已整理：\n%1\n至：\n%2").arg(realFileAbsPath1).arg(targetFileAbsPath1));
        emit AppendPossessLog(QString("已整理：\n%1\n至：\n%2").arg(realFileAbsPath2).arg(targetFileAbsPath2));
    }

    if(OR=="1"){
        QString realFileAbsPath1=fileAbsPath;

        QString tempRealFileAbsPath1=realFileAbsPath1;

        QString targetFileAbsPath1=tempRealFileAbsPath1.replace(FileOrDirPath1,FilesArrangerDir1);

        QFileInfo fileInfo1(targetFileAbsPath1);

        QDir dir1(fileInfo1.absolutePath());
        if (dir1.exists()==false) {
            dir1.mkpath(fileInfo1.absolutePath());
        }

        bool ok1 = QFile::copy(realFileAbsPath1, targetFileAbsPath1);

        if(ok1==true){
        }

        emit AppendPossessLog(QString("已整理：\n%1\n至：\n%2").arg(realFileAbsPath1).arg(targetFileAbsPath1));
    }

    if(OR=="2"){
        QString realFileAbsPath2=fileAbsPath;

        QString tempRealFileAbsPath2=realFileAbsPath2;

        QString targetFileAbsPath2=tempRealFileAbsPath2.replace(FileOrDirPath2,FilesArrangerDir2);

        QFileInfo fileInfo2(targetFileAbsPath2);

        QDir dir2(fileInfo2.absolutePath());
        if (dir2.exists()==false) {
            dir2.mkpath(fileInfo2.absolutePath());
        }

        bool ok2 = QFile::copy(realFileAbsPath2, targetFileAbsPath2);

        if(ok2==true){
        }

        emit AppendPossessLog(QString("已整理：\n%1\n至：\n%2").arg(realFileAbsPath2).arg(targetFileAbsPath2));
    }
}

void FileCompareDiffWin::TreeWidgetItemChanged(QTreeWidgetItem *item, int column)
{

}

void FileCompareDiffWin::TreeWidgetItemClicked(QTreeWidgetItem *item, int column)
{
    qDebug()<<"TreeWidgetItemClicked: "<<item->text(0);
}

void FileCompareDiffWin::TreeWidgetItemPressed(QTreeWidgetItem *item, int column)
{
    // 弹出一个菜单, 菜单项是 QAction 类型
    QMenu *pMenu=new QMenu(this);

    QAction* openFileDirPathAct  = new QAction(tr("打开文件所在路径"),pMenu);
    QAction* viewComparisonDetailsAct = new QAction("查看对比详情",pMenu);

    pMenu->addAction(openFileDirPathAct);
    pMenu->addAction(viewComparisonDetailsAct);

    connect(openFileDirPathAct, &QAction::triggered, this, [=](){
        QString fileName=item->text(0);
        QString filePath=item->text(2);
        QString OR=item->text(3);
        QString fileAbsPath=filePath+"/"+fileName;

        if(OR=="1/2"){
            QString realFileAbsPath1=fileAbsPath;
            QString realFileAbsPath2=fileAbsPath.replace(FileOrDirPath1,FileOrDirPath2);

            // // realFileAbsPath1.replace("/", "\\"); // 只能识别 "\"
            // // realFileAbsPath2.replace("/", "\\"); // 只能识别 "\"
            // realFileAbsPath1.remove(realFileAbsPath1.split("/").last());
            // realFileAbsPath2.remove(realFileAbsPath2.split("/").last());

            // //这是直接打开文件
            // // QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(realFileAbsPath1)));
            // // QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(realFileAbsPath2)));
            // //或是这样
            // QDesktopServices::openUrl(QUrl(realFileAbsPath1));
            // QDesktopServices::openUrl(QUrl(realFileAbsPath2));
            // //Qt打开指定目录并选中文件

            // realFileAbsPath1.replace("/", "\\"); // 只能识别 "\"
            // QString cmd = QString("explorer.exe /select,\"%1\"").arg(realFileAbsPath1);

            // QProcess process;
            // process.startDetached(cmd);

            QProcess::execute(QString("explorer /select,\"%1\"").arg(realFileAbsPath1));

            // process.startDetached(QString("explorer.exe /select,\"%1\"").arg(realFileAbsPath1));//启动后分离，即非阻塞
            // process.startDetached(QString("explorer.exe /select,\"%1\"").arg(realFileAbsPath2));//启动后分离，即非阻塞
        }
        if(OR=="1"){
            QString realFileAbsPath1=fileAbsPath;

            // //这是直接打开文件
            // QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(realFileAbsPath1)));
            //Qt打开指定目录并选中文件
            QProcess process;

            realFileAbsPath1.replace("/", "\\"); // 只能识别 "\"

            process.startDetached(QString("explorer.exe /select,\"%1\"").arg(realFileAbsPath1));//启动后分离，即非阻塞
        }
        if(OR=="2"){
            QString realFileAbsPath2=fileAbsPath;

            // //这是直接打开文件
            // QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(realFileAbsPath2)));
            //Qt打开指定目录并选中文件
            QProcess process;

            realFileAbsPath2.replace("/", "\\"); // 只能识别 "\"

            process.startDetached(QString("explorer.exe /select,\"%1\"").arg(realFileAbsPath2));//启动后分离，即非阻塞
        }
    });

    //自定义一个对比详情界面
    connect(viewComparisonDetailsAct, &QAction::triggered, this, [=](){
        QString fileName=item->text(0);
        QString fileType=item->text(1);
        QString filePath=item->text(2);
        QString OR=item->text(3);
        QString fileAbsPath=filePath+"/"+fileName;

        if(OR!="1/2"||fileType!=tr("file")){
            return;
        }

        QString realFileAbsPath1=fileAbsPath;
        QString realFileAbsPath2=fileAbsPath.replace(FileOrDirPath1,FileOrDirPath2);

        DialogBase *pDialogBaseWin=new FileCompareDiffDetailsWin("other");
        FileCompareDiffDetailsWin *pFileCompareDiffDetailsWin= dynamic_cast<FileCompareDiffDetailsWin*>(pDialogBaseWin);
        pDialogBaseWin->setAttribute(Qt::WA_DeleteOnClose);
        pDialogBaseWin->InitClass();
        pFileCompareDiffDetailsWin->SetFile1And2Path(realFileAbsPath1,realFileAbsPath2);
        pFileCompareDiffDetailsWin->OpenDiff();
        pFileCompareDiffDetailsWin->exec();
    });
    pMenu->exec(QCursor::pos());	// 右键菜单被模态显示出来了
    delete pMenu;
    //qDebug()<<"TreeWidgetItemPressed: "<<item->text(0);
}
