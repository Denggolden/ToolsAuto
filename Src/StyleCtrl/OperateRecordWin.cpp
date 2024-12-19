# pragma execution_character_set("utf-8")
#include "OperateRecordWin.h"
#include "ui_OperateRecordWin.h"

#include <QDebug>
#include "Src/Common/ReflexObject.h"
#include "StatusBarWin.h"

#include <QStyleOption>
#include <QBitmap>
#include <QPainter>
#include <Windows.h>
#include <windowsx.h>
#include <QPixmap>

OperateRecordWin::OperateRecordWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::OperateRecordWin)
{
    ui->setupUi(this);
}

OperateRecordWin::~OperateRecordWin()
{
    delete ui;
}

void OperateRecordWin::InitClass()
{
    InitToolButton();
    InitFrame();
    InitTextEdit();

    //this->setMaximumHeight(500);

    this->setMinimumSize(1500,900);
    this->layout()->setSpacing(0);
    this->layout()->setContentsMargins(0, 0, 0, 0);

    //设置无边框
    this->setWindowFlags(Qt::FramelessWindowHint);
}

void OperateRecordWin::InitToolButton()
{
    //int size=20;
    ToolButtonList.clear();
    ToolButtonList<<ui->ShrinkTBtn<<ui->ShrinkTBtn_2;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】")<<tr("【点击】");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //ToolButtonList.at(index)->setMinimumSize(size,size);
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&OperateRecordWin::ToolButtonClicked);
    }

    QPixmap maxPix = this->style()->standardPixmap(QStyle::SP_TitleBarNormalButton);
    ui->ShrinkTBtn->setIcon(maxPix);
    ui->ShrinkTBtn->setToolTip(tr("收缩"));
    //设置最小尺寸
    int size=25;
    ui->ShrinkTBtn->setMinimumSize(size,size);
    ui->ShrinkTBtn->setIconSize(QSize(size-3,size-3));
    ui->ShrinkTBtn->setStyleSheet("QToolButton{background-color:transparent;}QToolButton:hover{background-color:rgba(0,255,255,50);}");
}

void OperateRecordWin::InitFrame()
{
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);

    //ui->frame->hide();
}

void OperateRecordWin::InitTextEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->textEdit->setFont(font);
}

void OperateRecordWin::AppendOperateRecord(const QString &infoMsg, LogGrade logGrade)
{
    int rowCount = ui->textEdit ->document()->lineCount(); //获取行数

    // 先保存当前的文字颜色
    QColor curTextColor = ui->textEdit->textColor();
    if(logGrade==Normal){
        // 设置当前行要使用的颜色，假设为红色
        ui->textEdit->setTextColor(Qt::black);
        // 写入一行内容
        //ui->textEdit->append(infoMsg);
        // 写入一行内容
        ui->textEdit->append(QString("[%1]---%2").arg(rowCount).arg(infoMsg));
    }else {
        ui->textEdit->setTextColor(Qt::yellow);
        ui->textEdit->append(infoMsg);
        ui->textEdit->append(QString("[%1]---%2").arg(rowCount).arg(infoMsg));
    }

    if(rowCount>=500){
        ui->textEdit->clear();
    }
    // 最后恢复原来的颜色
    ui->textEdit->setTextColor(curTextColor);
    //或者HTML
    //    ui->textEdit->setStyleSheet("QTextEdit{padding-top:2px;background:#f7f7f7;border:none;border-radius:5px;font-size:14px;color:#778899;"
    //                                "font-family:Microsoft YaHei;padding-left:5px;padding-right:5px;}");//无边框
    //    QString fromName = QStringLiteral("王维");
    //    fromName = QStringLiteral("<font color = blue>【引用：%1】</font> <br>").arg(fromName);//必须用br作为换行符
    //    //quote和cite都是引用
    //    QString strQuote = QStringLiteral("空山新雨后，天气晚来秋。明月松间照，清泉石上流。");
    //    strQuote = QString("<font color = red>%1</font> <br>").arg(strQuote);
    //    QString strContent = QStringLiteral("这是一首写山水的名诗，于诗情画意中寄托诗人的高洁情怀和对理想的追求。");
    //    strContent = fromName + strQuote + strContent;
    //    ui->textEdit->append(strContent);
    //或者
    //https://blog.csdn.net/jiesunliu3215/article/details/112276428#:~:text=Qt%20QTextEdit%20%E8%AE%BE%E7%BD%AE%E6%9F%90%E8%A1%8C%E6%96%87%E5%AD%97%E4%B8%BA%E7%89%B9%E5%AE%9A%E9%A2%9C%E8%89%B2%201%20%2F%2F%20%E5%85%88%E4%BF%9D%E5%AD%98%E5%BD%93%E5%89%8D%E7%9A%84%E6%96%87%E5%AD%97%E9%A2%9C%E8%89%B2%202%20auto,line%22%3B%207%20m_textline.append%20%28str%29%3B%208%20%2F%2F%20%E6%9C%80%E5%90%8E%E6%81%A2%E5%A4%8D%E5%8E%9F%E6%9D%A5%E7%9A%84%E9%A2%9C%E8%89%B2%20%E6%9B%B4%E5%A4%9A%E9%A1%B9%E7%9B%AE
    //https://github.com/Ging-H/Qt-Note/blob/master/QTextEdit%E8%AE%BE%E7%BD%AE%E6%9F%90%E4%B8%80%E8%A1%8C%E5%AD%97%E4%BD%93%E9%A2%9C%E8%89%B2.md
}

void OperateRecordWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="ShrinkTBtn"){
        this->hide();
        StatusBarWin *pStatusBarWin= dynamic_cast<StatusBarWin*>(ReflexObject::Instance()->GetObjectIns("StatusBarWin"));
        pStatusBarWin->ShowOperateRecordArea();
    }
    else if(ToolButton->objectName()=="ShrinkTBtn_2"){
        AppendOperateRecord("123",Normal);
        AppendOperateRecord("456",Warning);
    }
}

void OperateRecordWin::paintEvent(QPaintEvent *paint)
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

#if (QT_VERSION <= QT_VERSION_CHECK(SplitMajor,SplitMinor,SplitPatch))
bool OperateRecordWin::nativeEvent(const QByteArray &eventType, void *message, long *result)
#else
bool OperateRecordWin::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#endif
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

        // int PosY = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        // if(PosY < MoveHeight){//拖拽生效区域
        //     *result = HTCAPTION;
        // }
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

void OperateRecordWin::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        IsPress=true;
        StartPos= event->globalPos();
        CurPos=event->globalPos();
        //qDebug()<<"mousePressEvent";
        // qDebug()<<"StartPos: "<<StartPos;
        // qDebug()<<"CurPos: "<<CurPos;
    }
}

void OperateRecordWin::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        IsPress=false;
        EndPos= event->globalPos();
        //qDebug()<<"mouseReleaseEvent";
        // qDebug()<<"EndPos: "<<EndPos;
    }
}

void OperateRecordWin::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        //qDebug()<<"mouseDoubleClickEvent:LeftButton ";
        //MaxWinTBtnClicked(false);
    }
}

void OperateRecordWin::mouseMoveEvent(QMouseEvent *event)
{
    if(IsPress==true){
        QPointF tempCurPos=event->globalPos();
        QPointF detPos=QPointF(CurPos.x()-tempCurPos.x(),CurPos.y()-tempCurPos.y());
        CurPos=tempCurPos;//更新当前点
        //qDebug()<<"detPos: "<<detPos;

        QPointF winPos=this->pos()-detPos;//当前位置-偏移值
        //qDebug()<<"winPos: "<<winPos;
        this->move(winPos.x(),winPos.y());
    }else{}
}
