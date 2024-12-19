#include "DataOperate.h"

#include <QCoreApplication>
#include <QSettings>

DataOperate DataOperate::Ins;

DataOperate::DataOperate()
{

}

void DataOperate::InitMemVar()
{
    KitDir=DataOperate::Instance()->GetIniFileNode(QObject::tr("TsFileCreateWinGroup"),QObject::tr("KitDir"), QObject::tr(""));
    ProFilePath=DataOperate::Instance()->GetIniFileNode(QObject::tr("TsFileCreateWinGroup"),QObject::tr("ProFilePath"), QObject::tr(""));
}

DataOperate *DataOperate::Instance()
{
    return &Ins;
}

QString DataOperate::GetIniFileNode(QString groupName, QString key, QString defaultValue)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/TranslationAuto.ini";
    QString value=defaultValue;
    //创建QSettings对象并指定ini文件路径并将格式设置为ini
    QSettings setting(fileName, QSettings::IniFormat);
#if (QT_VERSION <= QT_VERSION_CHECK(SplitMajor,SplitMinor,SplitPatch))
    setting.setIniCodec("utf-8");// 解决乱码
#else

#endif
    //添加第二个分组以及其键值对（创建分组的第二种方法）
    QString Key=groupName+"/"+key;
    if (setting.contains(Key)==true)
        value= setting.value(Key).toString();
    else {
        WriteIniFile(groupName,key,defaultValue);
    }
    return value;
}

void DataOperate::WriteIniFile(QString groupName, QString key, QString value)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/TranslationAuto.ini";
    //创建QSettings对象并指定ini文件路径并将格式设置为ini
    QSettings setting(fileName, QSettings::IniFormat);
#if (QT_VERSION <= QT_VERSION_CHECK(SplitMajor,SplitMinor,SplitPatch))
    setting.setIniCodec("utf-8");// 解决乱码
#else

#endif
    //添加第二个分组以及其键值对（创建分组的第二种方法）
    QString Key=groupName+"/"+key;
    setting.setValue(Key, value);
}

void DataOperate::SetKitDir(const QString &kitDir)
{
    KitDir=kitDir;
    WriteIniFile(QObject::tr("TsFileCreateWinGroup"),QObject::tr("KitDir"), kitDir);
}

QString DataOperate::GetKitDir()
{
    return KitDir;
}

void DataOperate::SetProFilePath(const QString &proFilePath)
{
    ProFilePath=proFilePath;
    WriteIniFile(QObject::tr("TsFileCreateWinGroup"),QObject::tr("ProFilePath"), proFilePath);
}

QString DataOperate::GetProFilePath()
{
    return ProFilePath;
}

