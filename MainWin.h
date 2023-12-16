#ifndef MAINWIN_H
#define MAINWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

class QListWidget;
class QStackedWidget;

class TitleBarWin;
class StatusBarWin;

class TsHandelMainWin;
class FileConvertMainWin;
class DataModelEditMainWin;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWin; }
QT_END_NAMESPACE

class MainWin : public WidgetBase
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = nullptr);
    ~MainWin();

private:
    Ui::MainWin *ui;
    int Width,Height;

    QListWidget *pListWidget=nullptr;
    QStackedWidget *pStackedWidget=nullptr;

    TitleBarWin *pTitleBarWin=nullptr;
    StatusBarWin *pStatusBarWin=nullptr;

    TsHandelMainWin *pTsHandelMainWin=nullptr;
    FileConvertMainWin *pFileConvertMainWin=nullptr;
    DataModelEditMainWin *pDataModelEditMainWin=nullptr;

protected:
    void InitClass() override;

public:
    void InitListWidget();
    void InitStackedWidget();
    void InitWinCtrl();



public slots:
    void CurrentRowChangedSlots(int curRow);


protected:
    void showEvent(QShowEvent *) override;;//打开窗口时执行
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *paint) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};
#endif // MAINWIN_H
