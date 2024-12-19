# pragma execution_character_set("utf-8")
#include "FileCompareDiffMainWin.h"
#include "ui_FileCompareDiffMainWin.h"

#include "Src/Common/ReflexObject.h"
#include "Src/FileCompareDiffWin/FileCompareDiffWin.h"

FileCompareDiffMainWin::FileCompareDiffMainWin(QWidget *parent)
    : WidgetBase(parent)
    , ui(new Ui::FileCompareDiffMainWin)
{
    ui->setupUi(this);
}

FileCompareDiffMainWin::~FileCompareDiffMainWin()
{
    delete ui;
}

void FileCompareDiffMainWin::InitClass()
{
    InitTabWidget();
    InitLabel();

    this->layout()->setContentsMargins(0, 0, 0, 0);
}

void FileCompareDiffMainWin::InitTabWidget()
{
    FileCompareDiffWin *pFileCompareDiffWin= dynamic_cast<FileCompareDiffWin*>(ReflexObject::Instance()->GetObjectIns("FileCompareDiffWin"));
    // TsFileExportWin *pTsFileExportWin=dynamic_cast<TsFileExportWin*>(ReflexObject::Instance()->GetObjectIns("TsFileExportWin"));
    // TsFileTranslateWin *pTsFileTranslateWin=dynamic_cast<TsFileTranslateWin*>(ReflexObject::Instance()->GetObjectIns("TsFileTranslateWin"));
    // QMFileGenerateWin *pQMFileGenerateWin=dynamic_cast<QMFileGenerateWin*>(ReflexObject::Instance()->GetObjectIns("QMFileGenerateWin"));

    ui->tabWidget->insertTab(1,pFileCompareDiffWin,tr("文件(夹)对比"));
    // ui->tabWidget->insertTab(2,pTsFileExportWin,tr("TS文件导出"));
    // ui->tabWidget->insertTab(3,pTsFileTranslateWin,tr("TS文件翻译"));
    // ui->tabWidget->insertTab(4,pQMFileGenerateWin,tr("QM文件生成"));

    // ui->tabWidget->widget(1)->setObjectName("TsFileCreateWin");
    // ui->tabWidget->widget(2)->setObjectName("TsFileExportWin");
    // ui->tabWidget->widget(3)->setObjectName("TsFileTranslateWin");
    // ui->tabWidget->widget(4)->setObjectName("QMFileGenerateWin");

    //qDebug()<< ui->tabWidget->widget(0)->objectName();

    //ui->tabWidget->setCurrentWidget(pSysteamMainPageWin);
    ui->tabWidget->setCurrentIndex(0);

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(TabWidgetcurrentChanged(int)));
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(TabWidgetcurrentChanged(int)));

    //    //标题栏红色背景，tab选中蓝色，未选中灰色
    //    ui->tabWidget->setStyleSheet("QTabWidget#tabWidget{background-color:rgb(255,255,255);}\
    //                                 QTabBar::tab{background-color:rgb(30,144,255);color:rgb(255,255,255);font: bold 10pt 'Microsoft YaHei'}\
    //                                 QTabBar::tab::selected{background-color:rgb(255,255,255);color:rgb(30,144,255);font: bold 10pt 'Microsoft YaHei'}\
    //                                 QTabBar::tab{width:100}QTabBar::tab{height:20}\
    //                                 QTabBar::tab{border: 3px solid white; border-radius: 5px;}");
}

void FileCompareDiffMainWin::InitLabel()
{
    ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->label->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
    //ui->label->setStyleSheet("QLabel{background-color:DeepSkyBlue;}");  //设置样式表
}

void FileCompareDiffMainWin::TabWidgetcurrentChanged(int index)
{
    qDebug()<<ui->tabWidget->widget(index)->objectName()<<"  "<<ui->tabWidget->tabText(index);
    qDebug()<<ui->tabWidget->currentWidget()->objectName();
}
