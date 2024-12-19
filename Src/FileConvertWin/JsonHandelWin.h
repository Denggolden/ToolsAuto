#ifndef JSONHANDELWIN_H
#define JSONHANDELWIN_H

//#include <QWidget>
#include "Src/Common/WidgetBase.h"

#include "Src/Common/GlobalCus.h"


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

    void ForEachTSFile();
    void ForEachTSFile(QDomElement *root,QJsonObject *rootObj);

    void ForEachJsonFile();
    void ForEachJsonFile(QJsonObject *rootObj);

    void ModifyJsonFile();
    void ModifyJsonFile(QJsonObject *rootObj);
};

#endif // JSONHANDELWIN_H
