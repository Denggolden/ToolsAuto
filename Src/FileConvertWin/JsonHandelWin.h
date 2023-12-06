#ifndef JSONHANDELWIN_H
#define JSONHANDELWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

class QDomElement;

namespace Ui {
class JsonHandelWin;
}

class JsonHandelWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit JsonHandelWin(QWidget *parent = nullptr);
    ~JsonHandelWin();

private:
    Ui::JsonHandelWin *ui;

protected:
    void InitClass() override;

public:
    void CreateJson();

    void FoTest1(QJsonObject *rootObj);
    void FoTest2(QJsonObject *rootObj);

    void ForEachTSFile();
    void ForEachTSFile(QDomElement *root,QJsonObject *rootObj);
};

#endif // JSONHANDELWIN_H
