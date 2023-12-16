# pragma execution_character_set("utf-8")
#include "ModelEditWin.h"
#include "ui_ModelEditWin.h"

#include <QDebug>
#include <QMenu>

#include <QtXml>
#include <QDomComment>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#include <QClipboard>

ModelEditWin::ModelEditWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::ModelEditWin)
{
    ui->setupUi(this);
}

ModelEditWin::~ModelEditWin()
{
    delete ui;
}

void ModelEditWin::InitClass()
{
    InitGroupBox();
    InitFrame();
    InitLineEdit();
    InitToolButton();
    InitLabel();
    InitTableWidget();

    LoadModTemplate();

    //CreateXML();
    //ModifyXML();
}

void ModelEditWin::InitGroupBox()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->groupBox->setFont(font);
    ui->groupBox_2->setFont(font);
    ui->groupBox_3->setFont(font);

    ui->groupBox->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_2->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_3->layout()->setContentsMargins(10, 0, 10, 5);
}

void ModelEditWin::InitFrame()
{
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);
}

void ModelEditWin::InitLineEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);

    ui->lineEdit->setPlaceholderText(tr("请输入模型名称"));
    ui->lineEdit_2->setPlaceholderText(tr("请输入模型所需字段数（项目数）"));
}

void ModelEditWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->CreateModFieldTBtn<<ui->SaveModTBtn;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】")<<tr("【点击】");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&ModelEditWin::ToolButtonClicked);
    }
}

void ModelEditWin::InitLabel()
{
    QList<QLabel*> LableList = this->findChildren<QLabel*>();
    foreach (QLabel *Lable, LableList) {
        if (Lable != nullptr){
            //Lable->setStyleSheet("border: 2px solid blue");
            Lable->setFont(QFont("Microsoft YaHei", 8, QFont::Bold));        //设置字体
            //Lable->setAlignment(Qt::AlignCenter);
        }
    }
}

void ModelEditWin::InitTableWidget()
{
    InitTableWidget(ui->tableWidget,1);
    InitTableWidget(ui->tableWidget_2,2);
}

void ModelEditWin::InitTableWidget(QTableWidget *tableWidget,int flag)
{
    if(flag==1)
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    else if (flag==2)
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->verticalHeader()->setVisible(false);  //隐藏垂直表头
    //tableWidget->horizontalHeader()->setVisible(false);  //隐藏水平表头
    tableWidget->setAlternatingRowColors(true); // 隔行变色
    tableWidget->setStyleSheet("selection-background-color:rgb(0,191,255)");

    tableWidget->setContextMenuPolicy (Qt::CustomContextMenu);
    connect(tableWidget,static_cast<void (QTableWidget::*)(const QPoint &)>(&QTableWidget::customContextMenuRequested), this,&ModelEditWin::ShowTableWidgetMenuSlots);

    connect(tableWidget,static_cast<void (QTableWidget::*)(int,int,int,int)>(&QTableWidget::currentCellChanged), this,[this](int currentRow, int currentColumn, int previousRow, int previousColumn){
        QTableWidget *TableWidget = qobject_cast<QTableWidget*>(sender());
        if(TableWidget==ui->tableWidget)
            return;

        if(currentRow!=previousRow){//表明行变化的条件   以行为单位
            //            qDebug()<<"currentCellChanged: "<<currentRow<<"   "<<currentColumn;
            //            qDebug()<<"currentCellChanged: "<<previousRow<<"   "<<previousColumn;
            if(previousRow<0)//非法的先前行
                return;

            //目前是无差别恢复先前行的单元格
            ModListInfo modListInfo=ModListInfoList.at(previousRow);
            int modFieldCount=modListInfo.ModFieldCount.toInt();
            ui->tableWidget_2->item(previousRow,0)->setText(modListInfo.ModName);//setItem(curRow,0,new QTableWidgetItem(modName));
            ui->tableWidget_2->item(previousRow,1)->setText(modListInfo.ModFieldCount);
            for (int Col=0;Col< modFieldCount;Col++ ) {
                QString fieldTxt=modListInfo.FieldList.at(Col);
                ui->tableWidget_2->item(previousRow,Col+2)->setText(fieldTxt);
            }
        }
    });
}

void ModelEditWin::CreateModFieldFun()
{
    QString modName=ui->lineEdit->text().trimmed();
    int modFieldCount=ui->lineEdit_2->text().trimmed().toInt();
    QList<QString> fieldList;fieldList.clear();
    QString fieldTxt="";
    for (int index=0;index<modFieldCount ;index++ ) {
        fieldList.append(fieldTxt);
    }

    RefreshModFieldTableWidget(modName,modFieldCount,fieldList);
}

void ModelEditWin::SaveModFun()
{
    QString modName=ui->lineEdit->text().trimmed();
    int modFieldCount=ui->lineEdit_2->text().trimmed().toInt();
    QList<QString> fieldList;fieldList.clear();
    for (int Col=0;Col< modFieldCount;Col++ ) {
        QString fieldTxt=ui->tableWidget->item(0,Col)->text();
        fieldList.append(fieldTxt);
    }

    UpDateModListTableWidget(modName,modFieldCount,fieldList);
    ModListInfoList.append(ModListInfo(modName,QString::number(modFieldCount),0,fieldList));

    //然后写入xml文件
    AddDataTemplate(modName,modFieldCount,fieldList);
}

void ModelEditWin::LoadModTemplate()
{
    // 打开文件
    QFile file("DataTemplate.xml");
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return;
    }

    file.close();

    QDomElement root = doc.documentElement();//读取根节点
    ModListInfoList.clear();

    QDomElement childEle= root.firstChildElement();
    while (!childEle.isNull()){
        //QString tagName = childEle.tagName();QString nodeName = childEle.nodeName();
        //QString nodeText=childEle.firstChild().nodeValue();//QString attrValue=childEle.attribute("FieldCount");

        QString modName=childEle.nodeName();
        int modFieldCount=childEle.attribute("FieldCount").toInt();
        int dataIDCount=childEle.attribute("DataIDCount").toInt();
        QList<QString> fieldList;fieldList.clear();
        for (int index=1;index<=modFieldCount ;index++ ) {
            QString fieldTxt=childEle.attribute(QString("%1.%2").arg(modName).arg(index));
            fieldList.append(fieldTxt);
        }

        UpDateModListTableWidget(modName,modFieldCount,fieldList);
        ModListInfoList.append(ModListInfo(modName,QString::number(modFieldCount),dataIDCount,fieldList));

        childEle=childEle.nextSiblingElement();
    }
}

void ModelEditWin::UpDateModListTableWidget(const QString &modName, const int &modFieldCount, const QList<QString> &fieldList)
{
    int curColCount=ui->tableWidget_2->columnCount();

    if(curColCount<(modFieldCount+2)){
        QStringList header;header.clear();
        header<<QString(tr("模型名"));
        header<<QString(tr("字段数"));
        for (int Col=1;Col<= modFieldCount;Col++ ) {
            header<<QString(tr("字段%1")).arg(Col);
        }
        int headerSize=header.size();
        ui->tableWidget_2->setColumnCount(headerSize);
        //设置表头
        ui->tableWidget_2->setHorizontalHeaderLabels(header);
        ui->tableWidget_2->horizontalHeader()->setFixedHeight(30);//设置表头高度
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        for (int Col=0;Col< headerSize;Col++ ) {
            ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(Col, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
        }
        int curRow=ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(curRow);//插入一行
        ui->tableWidget_2->setItem(curRow,0,new QTableWidgetItem(modName));
        ui->tableWidget_2->setItem(curRow,1,new QTableWidgetItem(QString::number(modFieldCount)));
        ui->tableWidget_2->item(curRow,0)->setFlags(ui->tableWidget_2->item(curRow,0)->flags() & (~Qt::ItemIsEditable));
        ui->tableWidget_2->item(curRow,1)->setFlags(ui->tableWidget_2->item(curRow,1)->flags() & (~Qt::ItemIsEditable));
        for (int Col=0;Col< modFieldCount;Col++ ) {
            QString fieldTxt=fieldList.at(Col);
            ui->tableWidget_2->setItem(curRow,Col+2,new QTableWidgetItem(fieldTxt));
        }
        ui->tableWidget_2->setRowHeight(curRow, 30);
    }else {
        int curRow=ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(curRow);//插入一行
        ui->tableWidget_2->setItem(curRow,0,new QTableWidgetItem(modName));
        ui->tableWidget_2->setItem(curRow,1,new QTableWidgetItem(QString::number(modFieldCount)));
        ui->tableWidget_2->item(curRow,0)->setFlags(ui->tableWidget_2->item(curRow,0)->flags() & (~Qt::ItemIsEditable));
        ui->tableWidget_2->item(curRow,1)->setFlags(ui->tableWidget_2->item(curRow,1)->flags() & (~Qt::ItemIsEditable));
        for (int Col=0;Col< modFieldCount;Col++ ) {
            QString fieldTxt=fieldList.at(Col);
            ui->tableWidget_2->setItem(curRow,Col+2,new QTableWidgetItem(fieldTxt));
        }
        ui->tableWidget_2->setRowHeight(curRow, 30);
    }
}

void ModelEditWin::AddDataTemplate(const QString &modName, const int &modFieldCount, const QList<QString> &fieldList)
{
    // 打开文件
    QFile file("DataTemplate.xml");
    if(file.exists()){//文件存在
        if (!file.open(QFileDevice::ReadOnly)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
            file.close();
            return;
        }

        file.close();

        QDomElement root = doc.documentElement();//读取根节点

        QDomElement modEmelNode = doc.createElement(modName);
        modEmelNode.setAttribute("FieldCount",modFieldCount);
        modEmelNode.setAttribute("DataIDCount",0);
        for (int col=1;col<=modFieldCount;col++ ) {
            QString fieldKey=QString("%1.%2").arg(modName).arg(col);
            QString fieldValue=fieldList.at(col-1);//ui->tableWidget_2->item(row,col+1)->text();
            modEmelNode.setAttribute(fieldKey,fieldValue);// 给节点创建属性 // 参数一是字符串，参数二可以是任何类型
        }
        root.appendChild(modEmelNode);

        if (!file.open(QFileDevice::WriteOnly|QIODevice::Truncate)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QTextStream stream(&file);
        stream.setCodec("utf-8");
        doc.save(stream, 2);		// 缩进2格
        file.close();

    }else {//文件不存在
        if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }
        //创建一个XML类
        QDomDocument doc;
        // 创建XML处理类，通常用于处理第一行描述信息 创建XML头部格式
        QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
        // 添加到XML文件中
        doc.appendChild(instruction);
        // 创建根节点
        QDomElement root = doc.createElement("DataTemplate");
        doc.appendChild(root);

        QDomElement modEmelNode = doc.createElement(modName);
        modEmelNode.setAttribute("FieldCount",modFieldCount);
        modEmelNode.setAttribute("DataIDCount",0);
        for (int col=1;col<=modFieldCount;col++ ) {
            QString fieldKey=QString("%1.%2").arg(modName).arg(col);
            QString fieldValue=fieldList.at(col-1);//ui->tableWidget_2->item(row,col+1)->text();
            modEmelNode.setAttribute(fieldKey,fieldValue);// 给节点创建属性 // 参数一是字符串，参数二可以是任何类型
        }
        root.appendChild(modEmelNode);

        QTextStream stream(&file);
        stream.setCodec("utf-8");
        doc.save(stream, 2);		// 缩进2格
        file.close();
    }
}

void ModelEditWin::DeleteDataTemplate(const QString &modName)
{
    // 打开文件
    QFile file("DataTemplate.xml");
    if(file.exists()){//文件存在
        if (!file.open(QFileDevice::ReadOnly)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
            file.close();
            return;
        }

        file.close();

        QDomElement root = doc.documentElement();//读取根节点

        QDomElement childEle= root.firstChildElement();
        while (!childEle.isNull()){
            //QString tagName = childEle.tagName();QString nodeName = childEle.nodeName();
            //QString nodeText=childEle.firstChild().nodeValue();//QString attrValue=childEle.attribute("FieldCount");
            QString nodeName = childEle.nodeName();
            if(nodeName==modName){
                root.removeChild(childEle);
                break;
            }
            childEle=childEle.nextSiblingElement();
        }

        if (!file.open(QFileDevice::WriteOnly|QIODevice::Truncate)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QTextStream stream(&file);
        stream.setCodec("utf-8");
        doc.save(stream, 2);		// 缩进2格
        file.close();
    }
}

void ModelEditWin::AddFieldDataTemplate(const QString &modName, const int &modFieldCount, const QString &attrValue)
{
    // 打开文件
    QFile file("DataTemplate.xml");
    if(file.exists()){//文件存在
        if (!file.open(QFileDevice::ReadOnly)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
            file.close();
            return;
        }

        file.close();

        QDomElement root = doc.documentElement();//读取根节点

        QDomElement childEle= root.firstChildElement();
        while (!childEle.isNull()){
            //QString tagName = childEle.tagName();QString nodeName = childEle.nodeName();
            //QString nodeText=childEle.firstChild().nodeValue();//QString attrValue=childEle.attribute("FieldCount");
            QString nodeName = childEle.nodeName();
            if(nodeName==modName){
                //模板节点
                childEle.setAttribute("FieldCount", modFieldCount);
                childEle.setAttribute(QString("%1.%2").arg(modName).arg(modFieldCount), attrValue);
                //模板数据节点
                QDomElement dataEle= childEle.firstChildElement();
                while (!dataEle.isNull()){
                    dataEle.setAttribute(QString("%1.%2").arg(modName).arg(modFieldCount), "");

                    dataEle=dataEle.nextSiblingElement();
                }
                break;
            }
            childEle=childEle.nextSiblingElement();
        }

        if (!file.open(QFileDevice::WriteOnly|QIODevice::Truncate)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QTextStream stream(&file);
        stream.setCodec("utf-8");
        doc.save(stream, 2);		// 缩进2格
        file.close();
    }
}

void ModelEditWin::EditFieldDataTemplate(const ModListInfo &preModListInfo, const ModListInfo &curModListInfo)
{
    // 打开文件
    QFile file("DataTemplate.xml");
    if(file.exists()){//文件存在
        if (!file.open(QFileDevice::ReadOnly)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
            file.close();
            return;
        }

        file.close();

        QDomElement root = doc.documentElement();//读取根节点

        QString modName=preModListInfo.ModName;

        QDomElement childEle= root.firstChildElement();
        while (!childEle.isNull()){
            QString nodeName = childEle.nodeName();
            if(nodeName==modName){
                int fieldListSize= preModListInfo.FieldList.size();
                for (int index=0;index<fieldListSize ;index++ ) {
                    if(preModListInfo.FieldList.at(index)!=curModListInfo.FieldList.at(index))
                        childEle.setAttribute(QString("%1.%2").arg(modName).arg(index+1), curModListInfo.FieldList.at(index));
                }
                break;
            }
            childEle=childEle.nextSiblingElement();
        }

        if (!file.open(QFileDevice::WriteOnly|QIODevice::Truncate)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QTextStream stream(&file);
        stream.setCodec("utf-8");
        doc.save(stream, 2);		// 缩进2格
        file.close();
    }
}

void ModelEditWin::DeleteDataFieldTemplate(const QString &modName, const QString &fieldKey)
{
    //关键在于重排
    // 打开文件
    QFile file("DataTemplate.xml");
    if(file.exists()){//文件存在
        if (!file.open(QFileDevice::ReadOnly)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
            file.close();
            return;
        }

        file.close();

        QDomElement root = doc.documentElement();//读取根节点

        QDomElement childEle= root.firstChildElement();
        while (!childEle.isNull()){
            QString nodeName = childEle.nodeName();
            if(nodeName==modName){
                //模板节点
                //需要的就替换不需要的不要替换
                int fieldCount=childEle.attribute("FieldCount").toInt();
                childEle.setAttribute("FieldCount",fieldCount-1);//字段减一
                QList<FieldKeyReplaceInfo> KeyReplaceInfoList;KeyReplaceInfoList.clear();
                bool isReplace=false;
                for (int index=1;index<=fieldCount ;index++ ) {
                    QString oldFieldKey=QString("%1.%2").arg(modName).arg(index);
                    QString newFieldKey=QString("%1.%2").arg(modName).arg(index-1);
                    if(oldFieldKey==fieldKey)
                        isReplace=true;

                    if(isReplace==true&&oldFieldKey!=fieldKey){
                        KeyReplaceInfoList.append(FieldKeyReplaceInfo(oldFieldKey,newFieldKey));//获取字段替换列表
                    }
                }

                //先删除需要移除的字段  再替换
                //这样替换重排效率最高  基本上不存在浪费的情况  一轮循环走下来就完成一个结点的修改
                childEle.removeAttribute(fieldKey);//移除指定属性
                int KeyReplaceInfoListSize=KeyReplaceInfoList.size();
                for (int index=0;index< KeyReplaceInfoListSize ;index++ ) {
                    QString oldFieldKey=KeyReplaceInfoList.at(index).OldFieldKey;
                    QString newFieldKey=KeyReplaceInfoList.at(index).NewFieldKey;

                    QString attrValue=childEle.attribute(oldFieldKey);//获取oldFieldKey value
                    childEle.removeAttribute(oldFieldKey);//移除oldFieldKey

                    childEle.setAttribute(newFieldKey,attrValue);//重新设置新属性
                }

                //模板数据节点
                QDomElement dataEle= childEle.firstChildElement();
                while (!dataEle.isNull()){

                    //先删除需要移除的字段  再替换
                    dataEle.removeAttribute(fieldKey);//移除指定属性
                    int KeyReplaceInfoListSize=KeyReplaceInfoList.size();
                    for (int index=0;index< KeyReplaceInfoListSize ;index++ ) {
                        QString oldFieldKey=KeyReplaceInfoList.at(index).OldFieldKey;
                        QString newFieldKey=KeyReplaceInfoList.at(index).NewFieldKey;

                        QString attrValue=dataEle.attribute(oldFieldKey);//获取oldFieldKey value
                        dataEle.removeAttribute(oldFieldKey);//移除oldFieldKey

                        dataEle.setAttribute(newFieldKey,attrValue);//重新设置新属性
                    }

                    dataEle=dataEle.nextSiblingElement();
                }

                break;
            }
            childEle=childEle.nextSiblingElement();
        }

        if (!file.open(QFileDevice::WriteOnly|QIODevice::Truncate)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
        }

        QTextStream stream(&file);
        stream.setCodec("utf-8");
        doc.save(stream, 2);		// 缩进2格
        file.close();
    }
}

void ModelEditWin::ClearModFieldTableWidget()
{
    //    ui->tableWidget->clear();

    while(ui->tableWidget->rowCount()!=0){
        ui->tableWidget->removeRow(0);
    }

    int headerSize=0;//header.size();
    ui->tableWidget->setColumnCount(headerSize);
    //    ui->tableWidget->setHorizontalHeaderLabels();
}

void ModelEditWin::RefreshModFieldTableWidget(const QString &modName, const int &modFieldCount, const QList<QString> &fieldList)
{
    QStringList header;header.clear();
    for (int Col=1;Col<= modFieldCount;Col++ ) {
        header<<QString(tr("%1.%2")).arg(modName).arg(Col);
    }
    while(ui->tableWidget->rowCount()!=0){
        ui->tableWidget->removeRow(0);
    }

    int headerSize=header.size();
    ui->tableWidget->setColumnCount(headerSize);
    //设置表头
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setFixedHeight(30);//设置表头高度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int Col=0;Col< headerSize;Col++ ) {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(Col, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
    }
    int curRow=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(curRow);//插入一行
    for (int Col=0;Col< modFieldCount;Col++ ) {
        ui->tableWidget->setItem(curRow,Col,new QTableWidgetItem(fieldList.at(Col)));
        ui->tableWidget->setRowHeight(curRow, 30);
    }
}

void ModelEditWin::PoptableWidget1Menu()
{
    //创建菜单
    QMenu *pMenu = new QMenu(this);
    //设置快捷键为T
    //QAction *pTest1 = new QAction("test(&T)", this);
    //添加一个图标
    //QAction *pTest2 = new QAction(QIcon(":/new/prefix1/resource/soccer_ball.ico"),"测试2", this);
    QAction *pPasteField = new QAction(tr("粘贴字段"), this);
    QAction *pAddField = new QAction(tr("添加字段"), this);
    QAction *pDeleteField = new QAction(tr("删除字段"), this);
    QAction *pRedesign = new QAction(tr("重新设计"), this);

    pPasteField->setObjectName("PasteField");
    pAddField->setObjectName("AddField");
    pDeleteField->setObjectName("DeleteField");
    pRedesign->setObjectName("Redesign");

    //把QAction对象添加到菜单上
    pMenu->addAction(pPasteField);
    pMenu->addAction(pAddField);
    //添加分隔线
    pMenu->addSeparator();
    pMenu->addAction(pDeleteField);
    pMenu->addSeparator();
    pMenu->addAction(pRedesign);

    //连接鼠标右键点击信号
    connect(pPasteField, &QAction::triggered, this, &ModelEditWin::OnClickedPopMenu);
    connect(pAddField, &QAction::triggered, this, &ModelEditWin::OnClickedPopMenu);
    connect(pDeleteField, &QAction::triggered, this, &ModelEditWin::OnClickedPopMenu);
    connect(pRedesign, &QAction::triggered, this, &ModelEditWin::OnClickedPopMenu);

    //在鼠标右键点击的地方显示菜单
    pMenu->exec(cursor().pos());

    //释放内存,若此处不手动释放，则必须等到程序结束时才都能释放
    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list)
        delete pAction;
    delete pMenu;
}

void ModelEditWin::PoptableWidget2Menu()
{
    QMenu *pMenu = new QMenu(this);
    QAction *pDeleteMod = new QAction(tr("删除模型"), this);
    QAction *pModifyAddField = new QAction(tr("添加字段-修改"), this);
    QAction *pModifyEditField = new QAction(tr("编辑字段-修改"), this);
    QAction *pModifyDeleteField = new QAction(tr("删除字段-修改"), this);

    pDeleteMod->setObjectName("DeleteMod");
    pModifyAddField->setObjectName("ModifyAddField");
    pModifyEditField->setObjectName("ModifyEditField");
    pModifyDeleteField->setObjectName("ModifyDeleteField");

    pMenu->addAction(pDeleteMod);
    pMenu->addSeparator();
    pMenu->addAction(pModifyAddField);
    pMenu->addAction(pModifyEditField);
    pMenu->addAction(pModifyDeleteField);

    if(CurColIndex<2)
        pModifyDeleteField->setEnabled(false);
    else
        pModifyDeleteField->setEnabled(true);

    connect(pDeleteMod, &QAction::triggered, this, &ModelEditWin::OnClickedPopMenu);
    connect(pModifyAddField, &QAction::triggered, this, &ModelEditWin::OnClickedPopMenu);
    connect(pModifyEditField, &QAction::triggered, this, &ModelEditWin::OnClickedPopMenu);
    connect(pModifyDeleteField, &QAction::triggered, this, &ModelEditWin::OnClickedPopMenu);

    pMenu->exec(cursor().pos());

    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list)
        delete pAction;
    delete pMenu;
}

void ModelEditWin::PasteFieldFun()
{
    QString modName=ui->lineEdit->text().trimmed();
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    QString originalText  = clipboard->text();            //获取剪贴板上文本信息
    QString txt=QString(originalText.replace("\n",""));
    qDebug()<<"txt: "<<txt;
    QStringList srcLIST=txt.split("\t");
    QList<QString> outLIST;outLIST.clear();
    int srcLISTSize=srcLIST.size();
    for (int index=0;index<srcLISTSize ;index++ ) {
        qDebug()<<srcLIST[index];
        if(srcLIST[index].trimmed()!="")
            outLIST.append(srcLIST[index]);
    }
    int outLISTSize=outLIST.size();
    ui->lineEdit_2->setText(QString::number(outLISTSize));
    RefreshModFieldTableWidget(modName,outLISTSize,outLIST);
}

void ModelEditWin::AddFieldFun(int curFieldIndex)
{
    QString modName=ui->lineEdit->text().trimmed();
    int modFieldCount=ui->lineEdit_2->text().trimmed().toInt()+1;
    QList<QString> fieldList;fieldList.clear();
    QString fieldTxt="";
    for (int index=0;index<modFieldCount ;index++ ) {
        if(index<=curFieldIndex)
            fieldTxt=ui->tableWidget->item(0,index)->text();
        else if(index==curFieldIndex+1)
            fieldTxt="";
        else
            fieldTxt=ui->tableWidget->item(0,index-1)->text();

        fieldList.append(fieldTxt);
    }

    ui->lineEdit_2->setText(QString::number(modFieldCount));
    RefreshModFieldTableWidget(modName,modFieldCount,fieldList);
}

void ModelEditWin::DeleteFieldFun(int curFieldIndex)
{
    QString modName=ui->lineEdit->text().trimmed();
    int modFieldCount=ui->lineEdit_2->text().trimmed().toInt()-1;
    if(modFieldCount<=0)
        return;
    QList<QString> fieldList;fieldList.clear();
    QString fieldTxt="";
    for (int index=0;index<modFieldCount+1 ;index++ ) {
        if(index==curFieldIndex)
            continue;
        else
            fieldTxt=ui->tableWidget->item(0,index)->text();
        fieldList.append(fieldTxt);
    }

    ui->lineEdit_2->setText(QString::number(modFieldCount));
    RefreshModFieldTableWidget(modName,modFieldCount,fieldList);
}

void ModelEditWin::RedesignFun()
{
    ClearModFieldTableWidget();
    ui->groupBox->setEnabled(true);
}

void ModelEditWin::DeleteModFun(int curRowIndex)
{
    ModListInfo modListInfo= ModListInfoList.at(curRowIndex);//先前的
    //或许还需要删除与之相关联的数据
    ui->tableWidget_2->removeRow(curRowIndex);
    ModListInfoList.removeAt(curRowIndex);

    DeleteDataTemplate(modListInfo.ModName);
}

void ModelEditWin::ModifyAddFieldFun(int curRowIndex)
{
    QString modName=ui->tableWidget_2->item(curRowIndex,0)->text();
    int modFieldCount=ui->tableWidget_2->item(curRowIndex,1)->text().toInt();
    int curColCount=ui->tableWidget_2->columnCount();

    if(curColCount<=(modFieldCount+2)){
        QStringList header;header.clear();
        header<<QString(tr("模型名"));
        header<<QString(tr("字段数"));
        for (int Col=1;Col<= modFieldCount+1;Col++ ) {
            header<<QString(tr("字段%1")).arg(Col);
        }
        int headerSize=header.size();
        ui->tableWidget_2->setColumnCount(headerSize);
        //设置表头
        ui->tableWidget_2->setHorizontalHeaderLabels(header);
        ui->tableWidget_2->horizontalHeader()->setFixedHeight(30);//设置表头高度
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        for (int Col=0;Col< headerSize;Col++ ) {
            ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(Col, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
        }

        ui->tableWidget_2->setItem(curRowIndex,modFieldCount+2,new QTableWidgetItem(QString("%1.NewField").arg(modName)));
    }else {
        ui->tableWidget_2->setItem(curRowIndex,modFieldCount+2,new QTableWidgetItem(QString("%1.NewField").arg(modName)));
    }

    //字段数+1 增加新字段
    ui->tableWidget_2->item(curRowIndex,1)->setText(QString("%1").arg(modFieldCount+1));
    ModListInfoList[curRowIndex].ModFieldCount=QString("%1").arg(modFieldCount+1);
    ModListInfoList[curRowIndex].FieldList.append(QString("%1.NewField").arg(modName));

    AddFieldDataTemplate(modName,modFieldCount+1,QString("%1.NewField").arg(modName));
}

void ModelEditWin::ModifyEditFieldFun(int curRowIndex)
{
    ModListInfo preModListInfo= ModListInfoList.at(curRowIndex);//先前的
    QString modName=ui->tableWidget_2->item(curRowIndex,0)->text();
    int modFieldCount=ui->tableWidget_2->item(curRowIndex,1)->text().toInt();
    QList<QString> fieldList;fieldList.clear();
    for (int col=0;col<modFieldCount ;col++ ) {
        QString fieldTxt=ui->tableWidget_2->item(curRowIndex,col+2)->text();
        fieldList.append(fieldTxt);
    }
    ModListInfo curModListInfo=ModListInfo(modName,QString::number(modFieldCount),preModListInfo.DataIDCount,fieldList);//当前的
    ModListInfoList[curRowIndex]=curModListInfo;

    //或许还需要修改与之相关联的数据的Key
    EditFieldDataTemplate(preModListInfo,curModListInfo);
}

void ModelEditWin::ModifyDeleteFieldFun(int curRowIndex, int curColIndex)
{
    QString modName=ui->tableWidget_2->item(curRowIndex,0)->text();
    int modFieldCount=ui->tableWidget_2->item(curRowIndex,1)->text().toInt();
    QString fieldKey=QString("%1.%2").arg(modName).arg(curColIndex-1);

    if(modFieldCount<=1)
        return;

    //字段数-1 删除选定字段
    ui->tableWidget_2->item(curRowIndex,1)->setText(QString("%1").arg(modFieldCount-1));
    ModListInfoList[curRowIndex].ModFieldCount=QString("%1").arg(modFieldCount-1);
    ModListInfoList[curRowIndex].FieldList.removeAt(curColIndex-2);

    //重新加载数据设计表格  这样可以极大程度上避免内存泄漏的风险
    while(ui->tableWidget_2->rowCount()!=0){
        ui->tableWidget_2->removeRow(0);
    }
    ui->tableWidget_2->setColumnCount(0);

    int ModListInfoListSize=ModListInfoList.size();
    for (int index=0;index<ModListInfoListSize ;index++) {
        int modFieldCount=ModListInfoList.at(index).ModFieldCount.toInt();
        UpDateModListTableWidget(modName,modFieldCount,ModListInfoList[index].FieldList);
    }

    DeleteDataFieldTemplate(modName,fieldKey);
}

void ModelEditWin::ModifyXML()
{
    // 打开文件
    QFile file("QT_XML.xml");
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return;
    }

    file.close();

    QDomElement root = doc.documentElement();//读取根节点

    ModifyXML(&root);

    if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }

    // 输出到文件
    QTextStream stream(&file);
    doc.save(stream, 2);	// 缩进2格
    file.close();
}

void ModelEditWin::ModifyXML(QDomElement *root)
{
    QDomElement childEle= (*root).firstChildElement();
    while (!childEle.isNull()){
        //QString tagName = childEle.tagName();QString nodeName = childEle.nodeName();
        //QString nodeText=childEle.firstChild().nodeValue();//QString attrValue=childEle.attribute("FieldCount");
        QString attrValue=childEle.attribute("Price");
        qDebug()<<"attrValue: "<<attrValue;
        childEle.removeAttribute("Price");//移除指定属性
        childEle.setAttribute("Price1", 666);//重新设置属性

        childEle.setAttribute("ID", 11);//覆盖原有属性值

        childEle=childEle.nextSiblingElement();
        ModifyXML(&childEle);
    }
}

void ModelEditWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="CreateModFieldTBtn"){
        ui->groupBox->setEnabled(false);
        CreateModFieldFun();
    }
    else if (ToolButton->objectName()=="SaveModTBtn") {
        ui->groupBox->setEnabled(true);
        SaveModFun();
    }
}

void ModelEditWin::ShowTableWidgetMenuSlots(const QPoint &pos)
{
    QTableWidget *TableWidget = qobject_cast<QTableWidget*>(sender());
    QModelIndex index = TableWidget->indexAt (pos);
    int row  = index.row();//获得QTableWidget列表点击的行数
    //qDebug()<<row;
    if(row<0)
        return;
    CurRowIndex=index.row();
    CurColIndex=index.column();

    if(TableWidget==ui->tableWidget)
        PoptableWidget1Menu();
    else if (TableWidget==ui->tableWidget_2)
        PoptableWidget2Menu();
}

void ModelEditWin::OnClickedPopMenu()
{
    QAction *pAction = qobject_cast<QAction*>(this->sender());
    qDebug()<<pAction->objectName();
    if(pAction->objectName()=="PasteField"){
        PasteFieldFun();
    }
    else if(pAction->objectName()=="AddField"){
        AddFieldFun(CurColIndex);
    }
    else if (pAction->objectName()=="DeleteField") {
        DeleteFieldFun(CurColIndex);
    }
    else if (pAction->objectName()=="Redesign") {
        RedesignFun();
    }

    else if (pAction->objectName()=="DeleteMod") {
        DeleteModFun(CurRowIndex);
    }
    else if (pAction->objectName()=="ModifyAddField") {
        ModifyAddFieldFun(CurRowIndex);
    }
    else if (pAction->objectName()=="ModifyEditField") {
        ModifyEditFieldFun(CurRowIndex);
    }
    else if (pAction->objectName()=="ModifyDeleteField") {
        ModifyDeleteFieldFun(CurRowIndex,CurColIndex);
    }
}
