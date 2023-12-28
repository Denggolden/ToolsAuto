#ifndef LOGHANDEL_H
#define LOGHANDEL_H

#include <QObject>
#include <QMutex>

static QMutex mutex;

class LogHandel : public QObject
{
    Q_OBJECT
//public:
//    explicit LogHandel(QObject *parent = nullptr);
public:
    void StartInstallMessageHandler();
    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    void GoToMsg(const QString &strOutStream);

public:
    LogHandel(LogHandel&)=delete;
    LogHandel& operator=(const LogHandel&)=delete;
    static LogHandel* Instance();

//private:
//    explicit LogHandel(QObject *parent = nullptr);
    LogHandel();
    ~LogHandel();
    static LogHandel Ins;

};

#endif // LOGHANDEL_H
