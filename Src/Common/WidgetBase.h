#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include <QWidget>

#include "Src/Common/GlobalCus.h"

namespace Ui {
class WidgetBase;
}

class WidgetBase : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBase(QWidget *parent = nullptr);
    virtual ~WidgetBase();

public:
    virtual void InitClass();
};

#endif // WIDGETBASE_H
