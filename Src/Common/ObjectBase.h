#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <QObject>

#include "Src/Common/GlobalCus.h"

class ObjectBase : public QObject
{
    Q_OBJECT
public:
    explicit ObjectBase(QObject *parent = nullptr);
    virtual ~ObjectBase();


public:
    virtual void InitClass();

signals:

};

#endif // OBJECTBASE_H
