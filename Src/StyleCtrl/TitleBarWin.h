#ifndef TITLEBARWIN_H
#define TITLEBARWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

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

protected:
    void InitClass() override;

public:
    void InitToolButton();
    void InitEventFilterObj();

private:
    bool IsMaxWin=false;

public slots:
    void MinWinTBtnClicked(bool checked);
    void MaxWinTBtnClicked(bool checked);
    void CloseWinTBtnClicked(bool checked);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // TITLEBARWIN_H
