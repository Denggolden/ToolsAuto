#include "MainWin.h"

#include <QApplication>

#include "Common/ReflexObject.h"
#include "DataManage/DataOperate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DataOperate::Instance()->InitMemVar();
    ReflexObject::Instance()->InitObjectIns();

    MainWin *pMainWin = dynamic_cast<MainWin*>(ReflexObject::Instance()->GetObjectIns("MainWin"));
    pMainWin->show();

//    MainWin w;
//    w.show();
    return a.exec();
}
