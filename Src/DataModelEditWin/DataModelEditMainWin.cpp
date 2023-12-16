# pragma execution_character_set("utf-8")
#include "DataModelEditMainWin.h"
#include "ui_DataModelEditMainWin.h"

#include <QDebug>
#include "Src/Common/ReflexObject.h"
#include "ModelEditWin.h"
#include "DataEditWin.h"

DataModelEditMainWin::DataModelEditMainWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::DataModelEditMainWin)
{
    ui->setupUi(this);
}

DataModelEditMainWin::~DataModelEditMainWin()
{
    delete ui;
}

void DataModelEditMainWin::InitClass()
{
    InitTabWidget();
    InitLabel();
}

void DataModelEditMainWin::InitTabWidget()
{
    ModelEditWin *pModelEditWin= dynamic_cast<ModelEditWin*>(ReflexObject::Instance()->GetObjectIns("ModelEditWin"));
    DataEditWin *pDataEditWin= dynamic_cast<DataEditWin*>(ReflexObject::Instance()->GetObjectIns("DataEditWin"));

    ui->tabWidget->insertTab(1,pModelEditWin,tr("模型编辑"));
    ui->tabWidget->insertTab(2,pDataEditWin,tr("数据编辑"));

    ui->tabWidget->widget(1)->setObjectName("ModelEditWin");
    ui->tabWidget->widget(2)->setObjectName("DataEditWin");

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

void DataModelEditMainWin::InitLabel()
{
    ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->label->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
    //ui->label->setStyleSheet("QLabel{background-color:DeepSkyBlue;}");  //设置样式表
}

void DataModelEditMainWin::TabWidgetcurrentChanged(int index)
{
    qDebug()<<ui->tabWidget->widget(index)->objectName()<<"  "<<ui->tabWidget->tabText(index);
    qDebug()<<ui->tabWidget->currentWidget()->objectName();
}
