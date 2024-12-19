#ifndef FILECOMPAREDIFFMAINWIN_H
#define FILECOMPAREDIFFMAINWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

#include "Src/Common/GlobalCus.h"


namespace Ui {
class FileCompareDiffMainWin;
}

class FileCompareDiffMainWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit FileCompareDiffMainWin(QWidget *parent = nullptr);
    ~FileCompareDiffMainWin();

private:
    Ui::FileCompareDiffMainWin *ui;

protected:
    void InitClass() override;

public:
    void InitTabWidget();
    void InitLabel();

public slots:
    void TabWidgetcurrentChanged(int index);
};

#endif // FILECOMPAREDIFFMAINWIN_H
