#ifndef TITLEBARWIN_H
#define TITLEBARWIN_H

#include <QWidget>

namespace Ui {
class TitleBarWin;
}

class TitleBarWin : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBarWin(QWidget *parent = nullptr);
    ~TitleBarWin();

private:
    Ui::TitleBarWin *ui;

public:
    void InitToolButton();

private:
    bool IsMaxWin=false;

public slots:
    void MinWinTBtnClicked(bool checked);
    void MaxWinTBtnClicked(bool checked);
    void CloseWinTBtnClicked(bool checked);
};

#endif // TITLEBARWIN_H
