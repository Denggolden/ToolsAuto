#ifndef TSHANDELMAINWIN_H
#define TSHANDELMAINWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

#include "Src/Common/GlobalCus.h"

namespace Ui {
class TsHandelMainWin;
}

class TsHandelMainWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit TsHandelMainWin(QWidget *parent = nullptr);
    ~TsHandelMainWin();

private:
    Ui::TsHandelMainWin *ui;

protected:
    void InitClass() override;

public:
    void InitTabWidget();
    void InitLabel();

public slots:
    void TabWidgetcurrentChanged(int index);
};

#endif // TSHANDELMAINWIN_H
