# pragma execution_character_set("utf-8")
#include "JsonHandelWin.h"
#include "ui_JsonHandelWin.h"

#include <QJsonObject> // { }
#include <QJsonArray> // [ ]
#include <QJsonDocument> // 解析Json
#include <QJsonValue> // int float double bool null { } [ ]
#include <QJsonParseError>

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QDomElement>
#include <QMessageBox>

JsonHandelWin::JsonHandelWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::JsonHandelWin)
{
    ui->setupUi(this);
}

JsonHandelWin::~JsonHandelWin()
{
    delete ui;
}

void JsonHandelWin::InitClass()
{
    //ForEachTSFile();
    //CreateJson();
    //ForEachJsonFile();
    ModifyJsonFile();
}

void JsonHandelWin::CreateJson()
{
    //一定要注意 QJsonObject应当先把叶子节点产生才可以
    //不可从根节点开始构建  应该先构建最外侧叶子节点
    //也就是说先分后总
    //已插入的节点不可更改
    //    // 定义 { } 对象
    //    QJsonObject interestObj;
    //    // 插入元素，对应键值对
    //    interestObj.insert("basketball", "篮球");
    //    interestObj.insert("badminton", "羽毛球");

    //    // 定义 [ ] 对象
    //    QJsonArray colorArray;
    //    // 往数组中添加元素
    //    colorArray.append("black");
    //    colorArray.append("white");

    // 定义根节点	也即是最外层 { }
    QJsonObject rootObject;

    // 插入元素
    rootObject.insert("name", "老王");
    rootObject.insert("age", 26);
    //rootObject.insert("interest", interestObj);
    //FoTest1(&rootObject);
    //FoTest1(&rootObject);
    //rootObject.insert("color", colorArray);
    //FoTest2(&rootObject);
    //FoTest2(&rootObject);
    rootObject.insert("vip", true);
    rootObject.insert("address", QJsonValue::Null);

    // 将json对象里的数据转换为字符串
    QJsonDocument doc;
    // 将object设置为本文档的主要对象
    doc.setObject(rootObject);

    QFile file("JsonFile.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "can't open error!";
        return;

    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");		// 设置写入编码是UTF8
    // 写入文件
    stream << doc.toJson();
    file.close();
}

void JsonHandelWin::ForEachTSFile()
{
    QString path="path";
    // 打开文件
    QFile fileIn(path+"/CN.ts");
    if (!fileIn.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, tr("提示"), tr("文件打开失败！"));
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&fileIn)) {
        QMessageBox::information(NULL, tr("提示"), tr("操作的文件不是XML文件！"));
        fileIn.close();
        return;
    }

    fileIn.close();

    QDomElement root = doc.documentElement();//读取根节点
    QJsonObject rootObject;

    ForEachTSFile(&root,&rootObject);

    // 将json对象里的数据转换为字符串
    QJsonDocument docJson;
    // 将object设置为本文档的主要对象
    docJson.setObject(rootObject);
    //qDebug()<<docJson.toJson();

    QFile file("JsonFile.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "can't open error!";
        return;

    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");		// 设置写入编码是UTF8
    // 写入文件
    stream << docJson.toJson();
    file.close();
}

void JsonHandelWin::ForEachTSFile(QDomElement *root,QJsonObject *rootObj)
{
    QDomElement childEle= (*root).firstChildElement();
    int no=0;
    while (!childEle.isNull()){
        no++;
        QJsonObject chileObj;
        QString tagName = childEle.toElement().tagName();
        QDomNamedNodeMap map= childEle.attributes();
        QString attrs="";
        for(int i = 0 ; i < map.length() ; ++i){
            QDomNode debug = map.item(i);
            QDomAttr attr = debug.toAttr();
            if(!attr.isNull()){
                attrs+= attr.name()+"="+attr.value()+" ";
                chileObj.insert(attr.name(),attr.value());
            }
        }
        QDomText dText=childEle.firstChild().toText();//节点下第一个元素（节点）文本
        QString all=QString("<%1 %2>%3").arg(tagName).arg(attrs).arg(dText.data());
        qDebug()<<all;
        chileObj.insert("text",dText.data());

        ForEachTSFile(&childEle,&chileObj);

        rootObj->insert(QString("%1%2").arg(tagName).arg(no),chileObj);
        childEle=childEle.nextSiblingElement();
    }
}

void JsonHandelWin::ForEachJsonFile()
{
    QFile file("JsonFile.json");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "can't open error!";
        return;
    }

    // 读取文件的全部内容
    QTextStream stream(&file);
    stream.setCodec("UTF-8");		// 设置读取编码是UTF8
    QString str = stream.readAll();

    file.close();

    // QJsonParseError类用于在JSON解析期间报告错误。
    QJsonParseError jsonError;
    // 将json解析为UTF-8编码的json文档，并从中创建一个QJsonDocument。
    // 如果解析成功，返回QJsonDocument对象，否则返回null
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
    // 判断是否解析失败
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        qDebug() << tr("Json格式错误！") << jsonError.error;
        return;
    }

    QJsonObject rootObj = doc.object();

    ForEachJsonFile(&rootObj);
}

void JsonHandelWin::ForEachJsonFile(QJsonObject *rootObj)
{
    for(QJsonObject::Iterator it=(*rootObj).begin();it!=(*rootObj).end();it++){
        //QString value=it.value().toString();
        QString key=it.key();
        qDebug()<<"key: "<<key;
        QJsonValue value= it.value();
        //QJsonValue value= (*rootObj).value(key);  // 获取指定 key 对应的 value
        if(value.isArray()){//处理 Array  就差遍历json数组了
        }
        else if(value.isBool()){
            bool b=value.toBool();
            qDebug()<<QString("type: %1  value: %2").arg("Bool").arg(b);
        }
        else if (value.isDouble()){
            double d=value.toDouble();
            qDebug()<<QString("type: %1  value: %2").arg("Double").arg(d);
        }
        else if (value.isNull()){
            qDebug()<<QString("type: %1  value: %2").arg("Null").arg("Null");
        }
        else if (value.isObject()){//处理 Object
            QJsonObject obj=value.toObject();
            ForEachJsonFile(&obj);
        }
        else if (value.isString()){
            QString str=value.toString();
            qDebug()<<QString("type: %1  value: %2").arg("String").arg(str);
        }
        else if (value.isUndefined()){
            qDebug()<<QString("type: %1  value: %2").arg("Undefined").arg("Undefined");
        }
    }
}

void JsonHandelWin::ModifyJsonFile()
{
    QFile file("JsonFile.json");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "can't open error!";
        return;
    }

    // 读取文件的全部内容
    QTextStream stream(&file);
    stream.setCodec("UTF-8");		// 设置读取编码是UTF8
    QString str = stream.readAll();

    file.close();

    // QJsonParseError类用于在JSON解析期间报告错误。
    QJsonParseError jsonError;
    // 将json解析为UTF-8编码的json文档，并从中创建一个QJsonDocument。
    // 如果解析成功，返回QJsonDocument对象，否则返回null
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
    // 判断是否解析失败
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        qDebug() << tr("Json格式错误！") << jsonError.error;
        return;
    }

    QJsonObject rootObj = doc.object();

    ModifyJsonFile(&rootObj);

    // 将object设置为本文档的主要对象
    QJsonDocument docOut;
    docOut.setObject(rootObj);

    // 重写打开文件，覆盖原有文件，达到删除文件全部内容的效果
    QFile writeFile("JsonFileOut.json");
    if (!writeFile.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "can't open error!";
        return;
    }

    // 将修改后的内容写入文件
    QTextStream wirteStream(&writeFile);
    wirteStream.setCodec("UTF-8");		// 设置读取编码是UTF8
    wirteStream << docOut.toJson();		// 写入文件
    writeFile.close();					// 关闭文件

}

void JsonHandelWin::ModifyJsonFile(QJsonObject *rootObj)
{
    for(QJsonObject::Iterator it=rootObj->begin();it!=rootObj->end();it++){
        //QString value=it.value().toString();
        QString key=it.key();
        qDebug()<<"key: "<<key;
        QJsonValue value= it.value();
        //QJsonValue value= (*rootObj).value(key);  // 获取指定 key 对应的 value
        if(value.isArray()){//处理 Array  就差遍历json数组了
        }
        else if(value.isBool()){
            bool b=value.toBool();
            qDebug()<<QString("type: %1  value: %2").arg("Bool").arg(b);
        }
        else if (value.isDouble()){
            double d=value.toDouble();
            qDebug()<<QString("type: %1  value: %2").arg("Double").arg(d);
        }
        else if (value.isNull()){
            qDebug()<<QString("type: %1  value: %2").arg("Null").arg("Null");
        }
        else if (value.isObject()){//处理 Object
            QJsonObject obj=value.toObject();
            ForEachJsonFile(&obj);
        }
        else if (value.isString()){
            QString str=value.toString();
            qDebug()<<QString("type: %1  value: %2").arg("String").arg(str);
            //            QJsonValueRef valueRef=it.value();
            //            QJsonObject obj = valueRef.toObject();
            //            obj[key]="www.baidu.com";
            //            valueRef=obj;


            //            QJsonObject A_obj=(*rootObj).value(key).toObject();
            //            A_obj["AA"]=33;
            (*rootObj)[key]="www.baidu.com";
            //it.value()="baiduCom";
        }
        else if (value.isUndefined()){
            qDebug()<<QString("type: %1  value: %2").arg("Undefined").arg("Undefined");
        }
    }
}
