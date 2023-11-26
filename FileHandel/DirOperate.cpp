#include "DirOperate.h"

#include <QDir>

DirOperate::DirOperate()
{

}

DirOperate::~DirOperate()
{

}

void DirOperate::CreateDir(QString dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()){
        //dir.mkdir(dirPath);
        dir.mkpath(dirPath);
    }
}
