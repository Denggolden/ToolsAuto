# pragma execution_character_set("utf-8")
#include "TitleBarWin.h"
#include "ui_TitleBarWin.h"

#include <QStyle>
#include "MainWin.h"
#include "Src/Common/ReflexObject.h"
#include <QDebug>
#include "Src/StyleCtrl/StatusBarWin.h"

#include <QMenu>

TitleBarWin::TitleBarWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TitleBarWin)
{
    ui->setupUi(this);
}

TitleBarWin::~TitleBarWin()
{
    delete ui;
}

void TitleBarWin::InitClass()
{
    InitToolButton();
    InitEventFilterObj();
    InitSystemTrayIcon();

    this->layout()->setContentsMargins(10, 0, 0, 0);
    this->setAttribute(Qt::WidgetAttribute::WA_StyledBackground);  // 重要
    this->setStyleSheet("background-color:LavenderBlush;");
}

void TitleBarWin::InitToolButton()
{
    //获取最小化、关闭按钮图标
    QPixmap minPix  = this->style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap maxPix = this->style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
    QPixmap closePix = this->style()->standardPixmap(QStyle::SP_TitleBarCloseButton);

    //设置最小化、关闭按钮图标
    ui->MinWinTBtn->setIcon(minPix);
    ui->MaxWinTBtn->setIcon(maxPix);
    ui->CloseWinTBtn->setIcon(closePix);

    //设置鼠标移至按钮上的提示信息
    ui->MinWinTBtn->setToolTip(tr("最小化"));
    ui->MaxWinTBtn->setToolTip(tr("最大化"));
    ui->CloseWinTBtn->setToolTip(tr("关闭"));

    //设置最小尺寸
    int size=25;
    ui->MinWinTBtn->setMinimumSize(size,size);
    ui->MaxWinTBtn->setMinimumSize(size,size);
    ui->CloseWinTBtn->setMinimumSize(size,size);

    ui->MinWinTBtn->setIconSize(QSize(size-3,size-3));
    ui->MaxWinTBtn->setIconSize(QSize(size-3,size-3));
    ui->CloseWinTBtn->setIconSize(QSize(size-3,size-3));


    //设置最小化、关闭按钮的样式
    ui->MinWinTBtn->setStyleSheet("QToolButton{background-color:transparent;}QToolButton:hover{background-color:rgba(0,255,255,50);}");
    ui->MaxWinTBtn->setStyleSheet("QToolButton{background-color:transparent;}QToolButton:hover{background-color:rgba(0,255,255,50);}");
    ui->CloseWinTBtn->setStyleSheet("QToolButton{background-color:transparent;}QToolButton:hover{background-color:rgba(255,0,0,90);}");

    connect(ui->MinWinTBtn, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&TitleBarWin::MinWinTBtnClicked);
    connect(ui->MaxWinTBtn, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&TitleBarWin::MaxWinTBtnClicked);
    connect(ui->CloseWinTBtn, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&TitleBarWin::CloseWinTBtnClicked);
}

void TitleBarWin::InitEventFilterObj()
{
    ui->MinWinTBtn->installEventFilter(this);
    ui->MaxWinTBtn->installEventFilter(this);
    ui->CloseWinTBtn->installEventFilter(this);
}

void TitleBarWin::InitSystemTrayIcon()
{
    //创建菜单对象和托盘图标对象
    pSystemTrayIcon=new QSystemTrayIcon(this);
    QMenu* pMenu=new QMenu(this);
    pMenu->setStyleSheet("QMenu{background:rgba(255,255,255,1);border:none;}"
                         "QMenu::item{color:rgba(51,51,51,1);font-size:12px;padding:3px 20px;font-weight: bold;}"//项目 即QAction
                         "QMenu::icon{padding: 0px 0px 0px 10px;}"//图标 边距
                         "QMenu::item:hover{background-color:#409CE1;}"
                         "QMenu::item:selected{background-color:#409CE1;}");

    QStringList menuTextList;menuTextList.clear();
    QStringList menuObjList;menuObjList.clear();
    QList<QPixmap> pixmapList;pixmapList.clear();
    QStringList tipList;tipList.clear();
    menuObjList<<tr("ShowMainWin")<<tr("Pending")<<tr("Exit");
    menuTextList<<tr("显示主窗口")<<tr("待定")<<tr("退出");
    pixmapList<<this->style()->standardPixmap(QStyle::SP_DesktopIcon)<<this->style()->standardPixmap(QStyle::SP_DriveNetIcon)
             <<this->style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    tipList<<tr("【点击】显示主窗口")<<tr("【点击】待定")<<tr("【点击】退出");
    int menuTextListSize=menuTextList.size();
    for (int index=0;index<menuTextListSize;index++ ) {
        //添加右键菜单栏分割线
        if(index==(menuTextListSize-1))
            pMenu->addSeparator();
        //添加右键菜单栏选项
        QAction* pAction=new QAction(QIcon(pixmapList.at(index)),menuTextList.at(index), this);
        //用 QWidgetAction 或许可以替代 QAction
        pAction->setObjectName(menuObjList.at(index));
        pAction->setToolTip(tipList.at(index));
        pMenu->addAction(pAction);
        pAction->installEventFilter(this);
        connect(pAction, static_cast<void (QAction::*)(bool)>(&QAction::triggered), this,&TitleBarWin::ActionTriggered);
    }

    //为系统托盘设置菜单为 pMenu
    pSystemTrayIcon->setContextMenu(pMenu);
    QPixmap IconPix  = this->style()->standardPixmap(QStyle::SP_MessageBoxQuestion);
    pSystemTrayIcon->setIcon(QIcon(IconPix));

    //    connect(pSystemTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    //            this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    //    pSystemTrayIcon->show();
}

void TitleBarWin::MinWinTBtnClicked(bool checked)
{
    MainWin *pMainWin = dynamic_cast<MainWin*>(ReflexObject::Instance()->GetObjectIns("MainWin"));
    pMainWin->showMinimized();
}

void TitleBarWin::MaxWinTBtnClicked(bool checked)
{
    if(IsMaxWin==false){
        IsMaxWin=true;
        MainWin *pMainWin = dynamic_cast<MainWin*>(ReflexObject::Instance()->GetObjectIns("MainWin"));
        pMainWin->showFullScreen();
        QPixmap normalPix = this->style()->standardPixmap(QStyle::SP_TitleBarNormalButton);
        ui->MaxWinTBtn->setIcon(normalPix);
        ui->MaxWinTBtn->setToolTip(tr("还原"));
    }else {
        IsMaxWin=false;
        MainWin *pMainWin = dynamic_cast<MainWin*>(ReflexObject::Instance()->GetObjectIns("MainWin"));
        pMainWin->showNormal();
        QPixmap maxPix = this->style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
        ui->MaxWinTBtn->setIcon(maxPix);
        ui->MaxWinTBtn->setToolTip(tr("最大化"));
    }
}

void TitleBarWin::CloseWinTBtnClicked(bool checked)
{
    //qApp->quit();
    pSystemTrayIcon->show();
    MainWin *pMainWin = dynamic_cast<MainWin*>(ReflexObject::Instance()->GetObjectIns("MainWin"));
    pMainWin->hide();
}

void TitleBarWin::ActionTriggered(bool)
{
    QAction *Action = qobject_cast<QAction*>(sender());
    qDebug()<<Action->objectName()<<"  "<<Action->text();
    if(Action->objectName()==tr("ShowMainWin")){
        pSystemTrayIcon->hide();
        MainWin *pMainWin = dynamic_cast<MainWin*>(ReflexObject::Instance()->GetObjectIns("MainWin"));
        pMainWin->show();
    }
    else if (Action->objectName()==tr("Pending")) {

    }
    else if (Action->objectName()==tr("Exit")) {
        pSystemTrayIcon->hide();
        qApp->quit();
    }
}

void TitleBarWin::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        pSystemTrayIcon->showMessage("title",tr("你单击了"));
        break;
    case QSystemTrayIcon::DoubleClick:
        pSystemTrayIcon->showMessage("title",tr("你双击了"));
        break;
    case QSystemTrayIcon::MiddleClick:
        pSystemTrayIcon->showMessage("title",tr("你中键了"));
        break;
    case QSystemTrayIcon::Context:
        pSystemTrayIcon->showMessage("title",tr("你Context"));
        break;
    default:
        break;
    }
}

bool TitleBarWin::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->MinWinTBtn||watched == ui->MaxWinTBtn||watched == ui->CloseWinTBtn||
            watched->objectName() == tr("ShowMainWin")||watched->objectName() == tr("Pending")||watched->objectName() == tr("Exit")){
        if(event->type() == QEvent::Enter){
            StatusBarWin * pStatusBarWin= dynamic_cast<StatusBarWin*>(ReflexObject::Instance()->GetObjectIns("StatusBarWin"));
            QString tipMsg=tr("");
            if(watched == ui->MinWinTBtn)
                tipMsg=tr("最小化窗口");
            else if (watched == ui->MaxWinTBtn)
                tipMsg=tr("最大化/还原窗口");
            else if (watched == ui->CloseWinTBtn)
                tipMsg=tr("关闭应用");//tr("ShowMainWin")<<tr("Pending")<<tr("Exit");
            else if (watched->objectName() == tr("ShowMainWin"))
                tipMsg=tr("显示窗口");
            else if (watched->objectName() == tr("Pending"))
                tipMsg=tr("待定");
            else if (watched->objectName() == tr("Exit"))
                tipMsg=tr("关闭应用");
            pStatusBarWin->SetTipInfo(tipMsg);
            /*鼠标进入按钮事件*/
            return true;
        }
        else if(event->type() == QEvent::Leave){
            StatusBarWin * pStatusBarWin= dynamic_cast<StatusBarWin*>(ReflexObject::Instance()->GetObjectIns("StatusBarWin"));
            QString tipMsg=tr("");
            pStatusBarWin->SetTipInfo(tipMsg);
            /*鼠标离开按钮事件*/
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}
