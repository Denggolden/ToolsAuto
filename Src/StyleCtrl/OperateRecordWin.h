#ifndef OPERATERECORDWIN_H
#define OPERATERECORDWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"
#include "Src/DataManage/DataModel.h"

#include <QMouseEvent>

#include "Src/Common/GlobalCus.h"

class QToolButton;

namespace Ui {
class OperateRecordWin;
}

class OperateRecordWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit OperateRecordWin(QWidget *parent = nullptr);
    ~OperateRecordWin();

private:
    Ui::OperateRecordWin *ui;
    QList<QToolButton*>ToolButtonList;

protected:
    void InitClass() override;

public:
    void InitToolButton();
    void InitFrame();
    void InitTextEdit();

    void AppendOperateRecord(const QString &infoMsg,LogGrade logGrade);

private:
    bool IsPress=false;
    QPointF StartPos;//开始点
    QPointF EndPos;//最终点
    QPointF CurPos;//移动后当前起点

public slots:
    void ToolButtonClicked(bool checked);

protected:
    void paintEvent(QPaintEvent *paint) override;
#if (QT_VERSION <= QT_VERSION_CHECK(SplitMajor,SplitMinor,SplitPatch))
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#else
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#endif

protected:
    void mousePressEvent(QMouseEvent *event) override;           //单击
    void mouseReleaseEvent(QMouseEvent *event) override;       //释放
    void mouseDoubleClickEvent(QMouseEvent *event); //双击
    void mouseMoveEvent(QMouseEvent *event) override;           //移动
};

#endif // OPERATERECORDWIN_H
