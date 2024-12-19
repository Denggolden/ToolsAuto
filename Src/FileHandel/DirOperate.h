#ifndef DIROPERATE_H
#define DIROPERATE_H

#include <QObject>

#include "Src/Common/GlobalCus.h"

class DirOperate
{
    //Q_OBJECT
public:
    DirOperate();
    ~DirOperate();

public:
    void CreateDir(QString dirPath);
};

#endif // DIROPERATE_H
