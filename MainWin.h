#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>

class QListWidget;
class QStackedWidget;

class TitleBarWin;

class TsFileCreateWin;
class TsFileExportWin;
class TsFileTranslateWin;
class QMFileGenerateWin;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWin; }
QT_END_NAMESPACE

class MainWin : public QWidget
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

    TsFileCreateWin *pTsFileCreateWin=nullptr;
    TsFileExportWin *pTsFileExportWin=nullptr;
    TsFileTranslateWin *pTsFileTranslateWin=nullptr;
    QMFileGenerateWin *pQMFileGenerateWin=nullptr;

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
