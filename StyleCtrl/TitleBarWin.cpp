# pragma execution_character_set("utf-8")
#include "TitleBarWin.h"
#include "ui_TitleBarWin.h"

#include <QStyle>
#include "MainWin.h"
#include "Common/ReflexObject.h"
#include <QDebug>
#include "StyleCtrl/StatusBarWin.h"

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

    //    this->layout()->setContentsMargins(3, 0, 3, 0);//控件居中显示 消除四周边距
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
    ui->CloseWinTBtn->setStyleSheet("QToolButton{background-color:transparent;}QToolButton:hover{background-color:rgba(0,255,255,50);}");

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
    qApp->quit();
}

bool TitleBarWin::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->MinWinTBtn||watched == ui->MaxWinTBtn||watched == ui->CloseWinTBtn){
        if(event->type() == QEvent::Enter){
            StatusBarWin * pStatusBarWin= dynamic_cast<StatusBarWin*>(ReflexObject::Instance()->GetObjectIns("StatusBarWin"));
            QString tipMsg=tr("");
            if(watched == ui->MinWinTBtn)
                tipMsg=tr("最小化窗口");
            else if (watched == ui->MaxWinTBtn)
               tipMsg=tr("最大化/还原窗口");
            else if (watched == ui->CloseWinTBtn)
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
