# pragma execution_character_set("utf-8")
#include "StatusBarWin.h"
#include "ui_StatusBarWin.h"

#include "Src/Common/ReflexObject.h"
#include "StatusBarWin.h"
#include "OperateRecordWin.h"
#include <QDebug>
#include <QStyle>

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
    InitFrame();
    InitToolButton();
    InitLineEdit();

    this->layout()->setContentsMargins(10, 0, 0, 0);
    this->setAttribute(Qt::WidgetAttribute::WA_StyledBackground);  // 重要
    this->setStyleSheet("background-color:Seashell;");
}

void StatusBarWin::InitLable()
{
    //Lable->setStyleSheet("border: 2px solid blue");
    ui->label->setFont(QFont("Microsoft YaHei", 8, QFont::Bold));        //设置字体
    //Lable->setAlignment(Qt::AlignCenter);
    ui->label->setMinimumWidth(80);
}

void StatusBarWin::InitEventFilterObj()
{
    this->installEventFilter(this);
}

void StatusBarWin::InitFrame()
{
    ui->frame->setStyleSheet("#frame{border:3px solid blue;border-radius: 2px;}");
    //ui->frame->setStyleSheet("#frame{background-color:transparent;}");
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);//控件居中显示 消除四周边距

    //OperateRecordWin *pOperateRecordWin= dynamic_cast<OperateRecordWin*>(ReflexObject::Instance()->GetObjectIns("OperateRecordWin"));
    //    QHBoxLayout *pHBoxLayoutFrame = new QHBoxLayout();//水平布局
    //    pHBoxLayoutFrame->addWidget(pOperateRecordWin);
    //    ui->frame->setLayout(pHBoxLayoutFrame);
}

void StatusBarWin::InitToolButton()
{
    QPixmap maxPix = this->style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
    ui->ExpandTBtn->setIcon(maxPix);
    ui->ExpandTBtn->setToolTip(tr("展开"));
    //设置最小尺寸
    int size=25;
    ui->ExpandTBtn->setMinimumSize(size,size);
    ui->ExpandTBtn->setIconSize(QSize(size-3,size-3));
    ui->ExpandTBtn->setStyleSheet("QToolButton{background-color:transparent;}QToolButton:hover{background-color:rgba(0,255,255,50);}");
    connect(ui->ExpandTBtn, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&StatusBarWin::ExpandTBtnClicked);
}

void StatusBarWin::InitLineEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    //ui->lineEdit->setPlaceholderText(tr("请输入模型名称"));
}

void StatusBarWin::SetTipInfo(const QString &tipMsg)
{
    ui->label->setText(tipMsg);
}

void StatusBarWin::SetLogInfo(const QString &logMsg)
{
    ui->lineEdit->setText(logMsg);
}

void StatusBarWin::ShowOperateRecordArea()
{
    ui->frame->show();
}

void StatusBarWin::HideOperateRecordArea()
{
    ui->frame->hide();
}

void StatusBarWin::ExpandTBtnClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="ExpandTBtn"){
        HideOperateRecordArea();
        OperateRecordWin *pOperateRecordWin= dynamic_cast<OperateRecordWin*>(ReflexObject::Instance()->GetObjectIns("OperateRecordWin"));
        pOperateRecordWin->show();
    }
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
