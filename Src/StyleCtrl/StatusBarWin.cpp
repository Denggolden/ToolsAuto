# pragma execution_character_set("utf-8")
#include "StatusBarWin.h"
#include "ui_StatusBarWin.h"

#include "Src/Common/ReflexObject.h"
#include "StatusBarWin.h"

StatusBarWin::StatusBarWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::StatusBarWin)
{
    ui->setupUi(this);
}

StatusBarWin::~StatusBarWin()
{
    delete ui;
}

void StatusBarWin::InitClass()
{
    InitLable();
    InitEventFilterObj();

    this->layout()->setContentsMargins(10, 0, 0, 0);
    this->setAttribute(Qt::WidgetAttribute::WA_StyledBackground);  // 重要
    this->setStyleSheet("background-color:Seashell;");
}

void StatusBarWin::InitLable()
{
    //Lable->setStyleSheet("border: 2px solid blue");
    ui->label->setFont(QFont("Microsoft YaHei", 8, QFont::Bold));        //设置字体
    //Lable->setAlignment(Qt::AlignCenter);
}

void StatusBarWin::InitEventFilterObj()
{
    this->installEventFilter(this);
}

void StatusBarWin::SetTipInfo(const QString &tipMsg)
{
    ui->label->setText(tipMsg);
}

bool StatusBarWin::eventFilter(QObject *watched, QEvent *event)
{
    if(watched ==this){
        if(event->type() == QEvent::Enter){
            StatusBarWin * pStatusBarWin= dynamic_cast<StatusBarWin*>(ReflexObject::Instance()->GetObjectIns("StatusBarWin"));
            QString tipMsg=tr("状态显示");
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
