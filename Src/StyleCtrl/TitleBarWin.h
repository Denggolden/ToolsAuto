#ifndef TITLEBARWIN_H
#define TITLEBARWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

#include <QSystemTrayIcon>
#include <QMouseEvent>

#include "Src/Common/GlobalCus.h"

namespace Ui {
class TitleBarWin;
}

class TitleBarWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit TitleBarWin(QWidget *parent = nullptr);
    ~TitleBarWin();

private:
    Ui::TitleBarWin *ui;
    QSystemTrayIcon *pSystemTrayIcon=nullptr;

protected:
    void InitClass() override;

public:
    void InitToolButton();
    void InitEventFilterObj();
    void InitSystemTrayIcon();

private:
    bool IsMaxWin=false;

    bool IsPress=false;
    QPointF StartPos;//开始点
    QPointF EndPos;//最终点
    QPointF CurPos;//移动后当前起点

public slots:
    void MinWinTBtnClicked(bool checked);
    void MaxWinTBtnClicked(bool checked);
    void CloseWinTBtnClicked(bool checked);
    void ActionTriggered(bool);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;           //单击
    void mouseReleaseEvent(QMouseEvent *event) override;       //释放
    void mouseDoubleClickEvent(QMouseEvent *event); //双击
    void mouseMoveEvent(QMouseEvent *event) override;           //移动
};

#endif // TITLEBARWIN_H
