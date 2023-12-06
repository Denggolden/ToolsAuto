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
    ForEachTSFile();
    //CreateJson();
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
    FoTest1(&rootObject);
    //FoTest1(&rootObject);
    //rootObject.insert("color", colorArray);
    FoTest2(&rootObject);
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

void JsonHandelWin::FoTest1(QJsonObject *rootObj)
{
    //    // 定义 { } 对象
    //    QJsonObject interestObj;
    //    // 插入元素，对应键值对
    //    interestObj.insert(QString("basketball%1").arg(no1), "篮球");
    //    interestObj.insert(QString("badminton%1").arg(no1), "羽毛球");

    ////    if(no1==0){
    ////        no1=1;
    ////        FoTest1(&interestObj);
    ////        qDebug()<<"FoTest1(&interestObj);";
    ////        rootObj->insert(QString("interest%1").arg(no1), interestObj);
    ////    }else {
    ////        rootObj->insert(QString("interest%1").arg(no1), interestObj);
    ////    }
}

void JsonHandelWin::FoTest2(QJsonObject *rootObj)
{
    // 定义 [ ] 对象
    QJsonArray colorArray;
    // 往数组中添加元素
    colorArray.append("black");
    colorArray.append("white");
    rootObj->insert("color", colorArray);
}

void JsonHandelWin::ForEachTSFile()
{
    QString path="C:/Users/admin/Desktop/C6000Pre/language/TsFileCreate/mingw73_32";
    // 打开文件
    QFile fileIn(path+"/CN.ts");
    if (!fileIn.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&fileIn)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
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
