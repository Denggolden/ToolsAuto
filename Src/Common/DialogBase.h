#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <QDialog>>

#include "Src/Common/GlobalCus.h"

class DialogBase : public QDialog
{
    Q_OBJECT
public:
    explicit DialogBase(QWidget *parent = nullptr);

public:
    virtual void InitClass();
};

#endif // DIALOGBASE_H
