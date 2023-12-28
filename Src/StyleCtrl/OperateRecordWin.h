#ifndef OPERATERECORDWIN_H
#define OPERATERECORDWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"
#include "Src/DataManage/DataModel.h"

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

public slots:
    void ToolButtonClicked(bool checked);

protected:
    void paintEvent(QPaintEvent *paint) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};

#endif // OPERATERECORDWIN_H
