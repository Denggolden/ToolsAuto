#ifndef DATAMODELEDITMAINWIN_H
#define DATAMODELEDITMAINWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

#include "Src/Common/GlobalCus.h"

namespace Ui {
class DataModelEditMainWin;
}

class DataModelEditMainWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit DataModelEditMainWin(QWidget *parent = nullptr);
    ~DataModelEditMainWin();

private:
    Ui::DataModelEditMainWin *ui;

protected:
    void InitClass() override;

public:
    void InitTabWidget();
    void InitLabel();

public slots:
    void TabWidgetcurrentChanged(int index);
};

#endif // DATAMODELEDITMAINWIN_H
