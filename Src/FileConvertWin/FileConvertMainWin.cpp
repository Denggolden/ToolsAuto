# pragma execution_character_set("utf-8")
#include "FileConvertMainWin.h"
#include "ui_FileConvertMainWin.h"
#include <QDebug>

#include "Src/Common/ReflexObject.h"
#include "JsonHandelWin.h"

FileConvertMainWin::FileConvertMainWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::FileConvertMainWin)
{
    ui->setupUi(this);
}

FileConvertMainWin::~FileConvertMainWin()
{
    delete ui;
}

void FileConvertMainWin::InitClass()
{
    InitTabWidget();
    InitLabel();
}

void FileConvertMainWin::InitTabWidget()
{
    JsonHandelWin *pJsonHandelWin= dynamic_cast<JsonHandelWin*>(ReflexObject::Instance()->GetObjectIns("JsonHandelWin"));

    ui->tabWidget->insertTab(1,pJsonHandelWin,tr("Json处理"));

    ui->tabWidget->widget(1)->setObjectName("JsonHandelWin");

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

void FileConvertMainWin::InitLabel()
{
    ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->label->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
    //ui->label->setStyleSheet("QLabel{background-color:DeepSkyBlue;}");  //设置样式表
}

void FileConvertMainWin::TabWidgetcurrentChanged(int index)
{
    qDebug()<<ui->tabWidget->widget(index)->objectName()<<"  "<<ui->tabWidget->tabText(index);
    qDebug()<<ui->tabWidget->currentWidget()->objectName();
}
