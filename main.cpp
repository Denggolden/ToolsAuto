#include "MainWin.h"

#include <QApplication>

#include "Src/Common/ReflexObject.h"
#include "Src/DataManage/DataOperate.h"

//#include "ThirdLib/qBreakpad/inc/QBreakpadHandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QBreakpadInstance.setDumpPath("crashes");    // 设置生成dump文件路径

    DataOperate::Instance()->InitMemVar();
    ReflexObject::Instance()->InitObjectIns();

    MainWin *pMainWin = dynamic_cast<MainWin*>(ReflexObject::Instance()->GetObjectIns("MainWin"));
    pMainWin->show();

    //    MainWin w;
    //    w.show();
    return a.exec();
}
