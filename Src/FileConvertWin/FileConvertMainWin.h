#ifndef FILECONVERTMAINWIN_H
#define FILECONVERTMAINWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

namespace Ui {
class FileConvertMainWin;
}

class FileConvertMainWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit FileConvertMainWin(QWidget *parent = nullptr);
    ~FileConvertMainWin();

private:
    Ui::FileConvertMainWin *ui;

protected:
    void InitClass() override;

public:
    void InitTabWidget();
    void InitLabel();

public slots:
    void TabWidgetcurrentChanged(int index);
};

#endif // FILECONVERTMAINWIN_H
