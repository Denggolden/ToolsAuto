#include "LogHandel.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include "Src/StyleCtrl/StatusBarWin.h"
#include "ReflexObject.h"
#include "Src/StyleCtrl/OperateRecordWin.h"

LogHandel LogHandel::Ins;

//LogHandel::LogHandel(QObject *parent) : QObject(parent)
//{

//}

LogHandel::LogHandel()
{

}

LogHandel *LogHandel::Instance()
{
    return &Ins;
}

LogHandel::~LogHandel()
{

}

void LogHandel::StartInstallMessageHandler()
{
    qInstallMessageHandler(myMessageOutput);
}

void LogHandel::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    //加锁：避免对文件的同时读写
    mutex.lock();
    //读写消息
    QString localMsg = msg;//msg.toLocal8Bit();
    //输出的字符串
    QString strOutStream = "";
    switch (type) {
    case QtDebugMsg:
    {
        strOutStream = QString("%1 %2 %3 %4 [Debug] %5 \n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(QString(context.file)).arg(context.line).arg(QString(context.function)).arg(msg);
    }
        break;
    case QtInfoMsg:
    {
        strOutStream = QString("%1 %2 %3 %4 [Info]: %5 \n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(QString(context.file)).arg(context.line).arg(QString(context.function)).arg(msg);
    }
        break;
    case QtWarningMsg:
    {
        strOutStream = QString("%1 %2 %3 %4 [Warning]: %5 \n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(QString(context.file)).arg(context.line).arg(QString(context.function)).arg(msg);
    }
        break;
    case QtCriticalMsg:
    {
        strOutStream = QString("%1 %2 %3 %4 [Critical]: %5 \n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(QString(context.file)).arg(context.line).arg(QString(context.function)).arg(msg);
    }
        break;
    case QtFatalMsg:
    {
        strOutStream = QString("%1 %2 %3 %4 [Fatal]: %5 \n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(QString(context.file)).arg(context.line).arg(QString(context.function)).arg(msg);
    }
        abort();
    }

    //每天生成一个新的log日志文件，文件名 yyyyMMdd.txt
    QString strFileName = QString("%1.log").arg(QDateTime::currentDateTime().date().toString("yyyyMMdd"));
    QFile logfile(strFileName);
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
    if(strOutStream != "")
    {
        QTextStream logStream(&logfile);
        //logStream.setCodec(QTextCodec::codecForName("utf-8"));
        logStream.setCodec("utf-8");
        //logStream<<strOutStream<<"\n";
        logStream<<strOutStream;
    }
    //清楚缓存文件，解锁
    logfile.flush();
    logfile.close();

    Ins.GoToMsg(strOutStream);
    mutex.unlock();
}

void LogHandel::GoToMsg(const QString &strOutStream)
{
    StatusBarWin *pStatusBarWin= dynamic_cast<StatusBarWin*>(ReflexObject::Instance()->GetObjectIns("StatusBarWin"));
    pStatusBarWin->SetLogInfo(strOutStream);

    OperateRecordWin *pOperateRecordWin= dynamic_cast<OperateRecordWin*>(ReflexObject::Instance()->GetObjectIns("OperateRecordWin"));
    pOperateRecordWin->AppendOperateRecord(strOutStream,Normal);
}
