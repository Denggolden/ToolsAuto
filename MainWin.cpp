# pragma execution_character_set("utf-8")
#include "MainWin.h"
#include "ui_MainWin.h"

#include <QListWidget>
#include <QStackedWidget>
#include <QDebug>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QBitmap>
#include <QPainter>
#include <Windows.h>
#include <windowsx.h>
#include <QPixmap>

#include "Src/Common/ReflexObject.h"
#include "Src/StyleCtrl/TitleBarWin.h"
#include "Src/DataManage/DataOperate.h"
#include "Src/TsHandelWin/TsHandelMainWin.h"
#include "Src/StyleCtrl/StatusBarWin.h"

#include <QDesktopWidget>

#pragma comment(lib,"user32")

MainWin::MainWin(QWidget *parent)
    : WidgetBase(parent)
    , ui(new Ui::MainWin)
{
    ui->setupUi(this);
}

MainWin::~MainWin()
{
    delete ui;
}

void MainWin::InitClass()
{
    InitListWidget();
    InitStackedWidget();
    InitWinCtrl();

    QDesktopWidget* desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry();

    Width=DataOperate::Instance()->GetIniFileNode(tr("MainWinGroup"),tr("Width"), tr("800")).toInt();
    Height=DataOperate::Instance()->GetIniFileNode(tr("MainWinGroup"),tr("Height"), tr("600")).toInt();

    int x = (screen.width() - Width) / 2;
    int y = (screen.height() - Height) / 2;
    this->setGeometry(x, y, Width, Height); // 让窗口位于屏幕中心
    //this->resize(Width,Height);

    //设置无边框
    this->setWindowFlags(Qt::FramelessWindowHint);
}


void MainWin::InitListWidget()
{
    pListWidget=new QListWidget();
    pListWidget->setFixedWidth(150);
    pListWidget->setAlternatingRowColors(true);
    pListWidget->setStyleSheet("alternate-background-color:rgb(255,245,238);background-color:Honeydew");//;background-color:rgb(222,191,255)");
    pListWidget->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));

    QStringList listName;listName.clear();
    listName<<tr("Qt翻译自动化")<<tr("待定");
    pListWidget->addItems(listName);

    connect(pListWidget,&QListWidget::currentRowChanged,this,&MainWin::CurrentRowChangedSlots);
}

void MainWin::InitStackedWidget()
{
    pStackedWidget=new QStackedWidget();

    pTsHandelMainWin= dynamic_cast<TsHandelMainWin*>(ReflexObject::Instance()->GetObjectIns("TsHandelMainWin"));

    pStackedWidget->addWidget(pTsHandelMainWin);
}

void MainWin::InitWinCtrl()
{
    QHBoxLayout* pLayout = new QHBoxLayout();//水平布局
    pLayout->addWidget(pListWidget);
    pLayout->addWidget(pStackedWidget);

    QVBoxLayout *pVBoxLayout=new QVBoxLayout();

    pTitleBarWin= dynamic_cast<TitleBarWin*>(ReflexObject::Instance()->GetObjectIns("TitleBarWin"));
    pTitleBarWin->setFixedHeight(30);
    pStatusBarWin= dynamic_cast<StatusBarWin*>(ReflexObject::Instance()->GetObjectIns("StatusBarWin"));
    pStatusBarWin->setFixedHeight(30);

    pVBoxLayout->addWidget(pTitleBarWin);
    pVBoxLayout->addItem(pLayout);
    pVBoxLayout->addWidget(pStatusBarWin);
    pVBoxLayout->setContentsMargins(0, 0, 0, 0);//控件居中显示 消除四周边距

    this->setLayout(pVBoxLayout);
}

void MainWin::CurrentRowChangedSlots(int curRow)
{
    qDebug()<<curRow<<"   "<<pListWidget->currentItem()->text();
    if(curRow==0){
        pStackedWidget->setCurrentWidget(pTsHandelMainWin);
    }
}

void MainWin::showEvent(QShowEvent *)
{
//    QDesktopWidget* desktop = QApplication::desktop();
//    QRect screen = desktop->screenGeometry();

//    Width=DataOperate::Instance()->GetIniFileNode(tr("MainWinGroup"),tr("Width"), tr("800")).toInt();
//    Height=DataOperate::Instance()->GetIniFileNode(tr("MainWinGroup"),tr("Height"), tr("600")).toInt();

//    int x = (screen.width() - Width) / 2;
//    int y = (screen.height() - Height) / 2;
//    //this->setGeometry(x, y, Width, Height); // 让窗口位于屏幕中心
//    this->resize(Width,Height);
}

void MainWin::resizeEvent(QResizeEvent *event)
{
    Width=this->width();
    Height=this->height();
    DataOperate::Instance()->WriteIniFile(tr("MainWinGroup"),tr("Width"), QString::number(Width));
    DataOperate::Instance()->WriteIniFile(tr("MainWinGroup"),tr("Height"), QString::number(Height));

    //qDebug()<<"Width: "<<Width;qDebug()<<"Height: "<<Height;
}

void MainWin::paintEvent(QPaintEvent *paint)
{
    //绘制样式
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//绘制样式

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter painter(&bmp);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(bmp.rect(), 10, 10);
    setMask(bmp);
}

bool MainWin::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_NCHITTEST) {
        *result = 0;
        const int BORDERWIDTH = 8; // 拉伸边框的宽度
        const int MoveHeight = 15; // 调整拖动生效的范围
        RECT winrect;
        GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        int PosY = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        if(PosY < MoveHeight){//拖拽生效区域
            *result = HTCAPTION;
        }
        // 判断鼠标位置是否在拉伸区域内
        if (y < winrect.top + BORDERWIDTH) {
            *result = HTTOP;
        }
        if (y > winrect.bottom - BORDERWIDTH) {
            *result = HTBOTTOM;
        }
        if (x < winrect.left + BORDERWIDTH) {
            *result = HTLEFT;
        }
        if (x > winrect.right - BORDERWIDTH) {
            *result = HTRIGHT;
        }
        if (y < winrect.top + BORDERWIDTH && x < winrect.left + BORDERWIDTH) {
            *result = HTTOPLEFT;
        }
        if (y < winrect.top + BORDERWIDTH && x > winrect.right - BORDERWIDTH) {
            *result = HTTOPRIGHT;
        }
        if (y > winrect.bottom - BORDERWIDTH && x < winrect.left + BORDERWIDTH) {
            *result = HTBOTTOMLEFT;
        }
        if (y > winrect.bottom - BORDERWIDTH && x > winrect.right - BORDERWIDTH) {
            *result = HTBOTTOMRIGHT;
        }
        if (*result != 0) {
            return true;
            //return false;
            //return QWidget::nativeEvent(eventType, message, result);
        }
    }
    return QWidget::nativeEvent(eventType, message, result);
}



