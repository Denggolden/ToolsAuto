#include "MainWin.h"

#include <QApplication>

#include "Src/Common/ReflexObject.h"
#include "Src/DataManage/DataOperate.h"

//#include "ThirdLib/qBreakpad/inc/QBreakpadHandler.h"

#include "Src/Common/LogHandel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QBreakpadInstance.setDumpPath("crashes");    // 设置生成dump文件路径

    DataOperate::Instance()->InitMemVar();
    ReflexObject::Instance()->InitObjectIns();
    LogHandel::Instance()->StartInstallMessageHandler();

    ReflexObject::Instance()->InitClassObj();

    //LogHandel::Instance()->StartInstallMessageHandler();

    MainWin *pMainWin = dynamic_cast<MainWin*>(ReflexObject::Instance()->GetObjectIns("MainWin"));
    pMainWin->show();

    //    MainWin w;
    //    w.show();
    return a.exec();
}

//    #include <QProcess>
//    QProcess *process = new QProcess;
//    QString program = "C:/Users/admin/Desktop/UDPDemo/UdpDemo.exe";

//    connect(process, &QProcess::readyReadStandardOutput,
//            this, [this](){
//        QProcess *process = qobject_cast<QProcess *>(sender());
//        QString OutMsg = QString::fromLocal8Bit(process->readAllStandardOutput());
//        qDebug()<<"OutMsg: "<<OutMsg;
//    });
//    connect(process, &QProcess::readyReadStandardError,
//            this, [this](){
//        QProcess *process = qobject_cast<QProcess *>(sender());
//        QString ErrMsg = QString::fromLocal8Bit(process->readAllStandardError());
//        qDebug()<<"ErrMsg: "<<ErrMsg;
//    });

//    process->start(program);
//    qDebug() << "[Current process ID: " << process->processId() << "]";
