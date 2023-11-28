# pragma execution_character_set("utf-8")
#include "TsHandelMainWin.h"
#include "ui_TsHandelMainWin.h"

#include <QDebug>
#include "Common/ReflexObject.h"
#include "TsHandelWin/TsFileCreateWin.h"
#include "TsHandelWin/TsFileExportWin.h"
#include "TsHandelWin/TsFileTranslateWin.h"
#include "TsHandelWin/QMFileGenerateWin.h"

TsHandelMainWin::TsHandelMainWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TsHandelMainWin)
{
    ui->setupUi(this);
}

TsHandelMainWin::~TsHandelMainWin()
{
    delete ui;
}

void TsHandelMainWin::InitClass()
{
    InitTabWidget();
    InitLabel();

    this->layout()->setContentsMargins(0, 0, 0, 0);
}

void TsHandelMainWin::InitTabWidget()
{
    TsFileCreateWin *pTsFileCreateWin= dynamic_cast<TsFileCreateWin*>(ReflexObject::Instance()->GetObjectIns("TsFileCreateWin"));
    TsFileExportWin *pTsFileExportWin=dynamic_cast<TsFileExportWin*>(ReflexObject::Instance()->GetObjectIns("TsFileExportWin"));
    TsFileTranslateWin *pTsFileTranslateWin=dynamic_cast<TsFileTranslateWin*>(ReflexObject::Instance()->GetObjectIns("TsFileTranslateWin"));
    QMFileGenerateWin *pQMFileGenerateWin=dynamic_cast<QMFileGenerateWin*>(ReflexObject::Instance()->GetObjectIns("QMFileGenerateWin"));

    ui->tabWidget->insertTab(1,pTsFileCreateWin,tr("TS文件生成"));
    ui->tabWidget->insertTab(2,pTsFileExportWin,tr("TS文件导出"));
    ui->tabWidget->insertTab(3,pTsFileTranslateWin,tr("TS文件翻译"));
    ui->tabWidget->insertTab(4,pQMFileGenerateWin,tr("QM文件生成"));

    ui->tabWidget->widget(1)->setObjectName("TsFileCreateWin");
    ui->tabWidget->widget(2)->setObjectName("TsFileExportWin");
    ui->tabWidget->widget(3)->setObjectName("TsFileTranslateWin");
    ui->tabWidget->widget(4)->setObjectName("QMFileGenerateWin");

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

void TsHandelMainWin::InitLabel()
{
    ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->label->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
    //ui->label->setStyleSheet("QLabel{background-color:DeepSkyBlue;}");  //设置样式表
}

void TsHandelMainWin::TabWidgetcurrentChanged(int index)
{
    qDebug()<<ui->tabWidget->widget(index)->objectName()<<"  "<<ui->tabWidget->tabText(index);
    qDebug()<<ui->tabWidget->currentWidget()->objectName();
}
