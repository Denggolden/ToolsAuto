#ifndef STATUSBARWIN_H
#define STATUSBARWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

namespace Ui {
class StatusBarWin;
}

class StatusBarWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit StatusBarWin(QWidget *parent = nullptr);
    ~StatusBarWin();

private:
    Ui::StatusBarWin *ui;

protected:
    void InitClass() override;

public:
    void InitLable();
    void InitEventFilterObj();
    void InitFrame();
    void InitToolButton();
    void InitLineEdit();

    void SetTipInfo(const QString &tipMsg);
    void SetLogInfo(const QString &logMsg);

    void ShowOperateRecordArea();
    void HideOperateRecordArea();

public slots:
    void ExpandTBtnClicked(bool checked);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // STATUSBARWIN_H
