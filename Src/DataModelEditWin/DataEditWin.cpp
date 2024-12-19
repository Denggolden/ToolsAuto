# pragma execution_character_set("utf-8")
#include "DataEditWin.h"
#include "ui_DataEditWin.h"

#include <QDebug>

#include <QtXml>
#include <QDomComment>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QMenu>

#include <QClipboard>

DataEditWin::DataEditWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::DataEditWin)
{
    ui->setupUi(this);
}

DataEditWin::~DataEditWin()
{
    delete ui;
}

void DataEditWin::InitClass()
{
    ModListInfoList.clear();
    ModDataInfoList.clear();
    CurModListInfoListIndex=0;

    InitGroupBox();
    InitFrame();
    InitComboBox();
    InitToolButton();
    InitLabel();
    InitTableWidget();

    LoadModTemplate();
    LoadWinData();
}

void DataEditWin::InitGroupBox()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->groupBox->setFont(font);
    ui->groupBox_2->setFont(font);
    ui->groupBox_3->setFont(font);

    ui->groupBox->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_2->layout()->setContentsMargins(10, 0, 10, 5);
    ui->groupBox_3->layout()->setContentsMargins(10, 0, 10, 5);
}

void DataEditWin::InitFrame()
{
    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);
}

void DataEditWin::InitComboBox()
{
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(ui->comboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,&DataEditWin::ComboBoxActivated);
}

void DataEditWin::InitToolButton()
{
    ToolButtonList.clear();
    ToolButtonList<<ui->toolButton;
    QStringList toolTipList;toolTipList.clear();
    toolTipList<<tr("【点击】");
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize;index++) {
        //PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        ToolButtonList.at(index)->setToolTip(toolTipList.at(index));
        //PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        //PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        //connect(ToolButtonList.at(index),SIGNAL(triggered()),this,SLOT(PushButtonSlots()));
        connect(ToolButtonList.at(index),static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this,&DataEditWin::ToolButtonClicked);
    }
}

void DataEditWin::InitLabel()
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

void DataEditWin::InitTableWidget()
{
    InitTableWidget(ui->tableWidget,1);
    InitTableWidget(ui->tableWidget_2,2);
}

void DataEditWin::InitTableWidget(QTableWidget *tableWidget, int flag)
{
    if(flag==1)
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    else if (flag==2)
        tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->verticalHeader()->setVisible(false);  //隐藏垂直表头
    //tableWidget->horizontalHeader()->setVisible(false);  //隐藏水平表头
    tableWidget->setAlternatingRowColors(true); // 隔行变色
    tableWidget->setStyleSheet("selection-background-color:rgb(0,191,255)");

    tableWidget->setContextMenuPolicy (Qt::CustomContextMenu);
    connect(tableWidget,static_cast<void (QTableWidget::*)(const QPoint &)>(&QTableWidget::customContextMenuRequested), this,&DataEditWin::ShowTableWidgetMenuSlots);

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
            ModDataInfo modDataInfo=ModDataInfoList.at(previousRow);
            int iDCount=modDataInfo.IDCount;
            ui->tableWidget_2->item(previousRow,0)->setText(QString::number(iDCount));
            int modDataListSize=modDataInfo.ModData.size();
            for (int Col=0;Col< modDataListSize;Col++ ) {
                QString dataTxt=modDataInfo.ModData.at(Col);
                ui->tableWidget_2->item(previousRow,Col+1)->setText(dataTxt);
            }
        }
    });

    //需要抬起  只能鼠标左键
    //    connect(tableWidget,static_cast<void (QTableWidget::*)(int,int)>(&QTableWidget::cellClicked), this,[this](int row, int column){
    //        QTableWidget *TableWidget = qobject_cast<QTableWidget*>(sender());
    //        if(TableWidget==ui->tableWidget)
    //            return;
    //        CopyCellRowIndex=row;
    //        CopyCellColIndex=column;
    //        qDebug()<<"row: "<<row;qDebug()<<"column: "<<column;
    //    });

    //按下就行  鼠标左右键都能捕获
    connect(tableWidget,static_cast<void (QTableWidget::*)(int,int)>(&QTableWidget::cellPressed), this,[this](int row, int column){
        QTableWidget *TableWidget = qobject_cast<QTableWidget*>(sender());
        if(TableWidget==ui->tableWidget){
            CopyDisRowIndex=row;
            CopyDisColIndex=column;
        }
        if(TableWidget==ui->tableWidget_2){
            CopySrcRowIndex=row;
            CopySrcColIndex=column;
        }

        //        qDebug()<<"cellPressed";
        //        qDebug()<<"row: "<<row;qDebug()<<"column: "<<column;
    });

    //    connect(tableWidget,static_cast<void (QTableWidget::*)(QTableWidgetItem *)>(&QTableWidget::itemEntered), this,[this](QTableWidgetItem *item){
    //        QTableWidget *TableWidget = qobject_cast<QTableWidget*>(sender());
    //        if(TableWidget==ui->tableWidget)
    //            return;

    //        qDebug()<<"cellEntered";
    //        qDebug()<<"row: "<<item->row();qDebug()<<"column: "<<item->column();
    //    });
}

void DataEditWin::LoadModTemplate()
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

        ModListInfoList.append(ModListInfo(modName,QString::number(modFieldCount),dataIDCount,fieldList));

        childEle=childEle.nextSiblingElement();
    }
}

void DataEditWin::LoadWinData()
{
    int ModListInfoListSize=ModListInfoList.size();
    if(ModListInfoListSize<=0)
        return;

    ui->comboBox->clear();
    for (int index=0;index< ModListInfoListSize;index++ ) {
        ui->comboBox->addItem(ModListInfoList.at(index).ModName);
    }
    QStringList header1;header1.clear();
    QStringList header2;header2.clear();
    QString modName=ModListInfoList.at(0).ModName;
    header2<<tr("IDCount");
    int FieldListSize=ModListInfoList.at(0).FieldList.size();
    for (int index=0;index< FieldListSize;index++ ) {
        header1<<ModListInfoList.at(0).FieldList.at(index);
        header2<<ModListInfoList.at(0).FieldList.at(index);
    }

    ui->groupBox_2->setTitle(QString(tr("【%1】数据添加")).arg(modName));
    ui->groupBox_3->setTitle(QString(tr("模型【%1】数据列表")).arg(modName));

    ResetTableWidget1Data(ui->tableWidget,header1);
    //然后加载数据到 ui->tableWidget_2 中
    SetTableWidget2Data(ui->tableWidget_2,header2,modName);
}

void DataEditWin::UpDateWinShowData()
{
    LoadModTemplate();
    LoadWinData();
}

void DataEditWin::SetTableWidgetHader(QTableWidget *tableWidget, const QStringList &header)
{
    int headerSize=header.size();
    tableWidget->setColumnCount(headerSize);
    //设置表头
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setFixedHeight(30);//设置表头高度
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int Col=0;Col< headerSize;Col++ ) {
        tableWidget->horizontalHeader()->setSectionResizeMode(Col, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
    }
}

void DataEditWin::ResetTableWidget1Data(QTableWidget *tableWidget, const QStringList &header)
{
    while(tableWidget->rowCount()!=0){
        tableWidget->removeRow(0);
    }
    int headerSize=header.size();
    tableWidget->setColumnCount(headerSize);
    //设置表头
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setFixedHeight(30);//设置表头高度
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int Col=0;Col< headerSize;Col++ ) {
        tableWidget->horizontalHeader()->setSectionResizeMode(Col, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
    }

    int curRow=tableWidget->rowCount();
    tableWidget->insertRow(curRow);//插入一行
    for (int Col=0;Col< headerSize;Col++ ) {
        tableWidget->setItem(curRow,Col,new QTableWidgetItem(""));
    }
    tableWidget->setRowHeight(curRow, 30);
}

void DataEditWin::SetTableWidget2Data(QTableWidget *tableWidget, const QStringList &header, const QString &modName)
{
    while(tableWidget->rowCount()!=0){
        tableWidget->removeRow(0);
    }
    int headerSize=header.size();
    tableWidget->setColumnCount(headerSize);
    //设置表头
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setFixedHeight(30);//设置表头高度
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int Col=0;Col< headerSize;Col++ ) {
        tableWidget->horizontalHeader()->setSectionResizeMode(Col, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
    }

    ModDataInfoList.clear();
    GetModData(modName,headerSize-1,ModDataInfoList);//应该是 headerSize-1 不是 headerSize

    int modDataInfoListSize=ModDataInfoList.size();
    for (int index=0;index<modDataInfoListSize ;index++) {
        int curRow=tableWidget->rowCount();
        tableWidget->insertRow(curRow);//插入一行

        tableWidget->setItem(curRow,0,new QTableWidgetItem(QString("%1").arg(ModDataInfoList.at(index).IDCount)));
        int ModDataSize= ModDataInfoList.at(index).ModData.size();
        for (int col=0;col<ModDataSize ;col++ ) {
            tableWidget->setItem(curRow,col+1,new QTableWidgetItem(ModDataInfoList.at(index).ModData.at(col)));
        }
        tableWidget->setRowHeight(curRow, 30);
    }
}

void DataEditWin::GetModData(const QString &modName,const int &modFieldCount, QList<ModDataInfo> &modDataInfoList)
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

    QDomElement childEle= root.firstChildElement();
    while (!childEle.isNull()){
        QString tModName=childEle.nodeName();
        if(tModName==modName){
            QDomElement dataEle= childEle.firstChildElement();
            while (!dataEle.isNull()){
                int iDCount=dataEle.attribute("IDCount").toInt();
                QList<QString> ModDataList;ModDataList.clear();
                for (int index=1;index<=modFieldCount ;index++ ) {
                    QString fieldTxt=dataEle.attribute(QString("%1.%2").arg(modName).arg(index));
                    ModDataList.append(fieldTxt);
                }
                modDataInfoList.append(ModDataInfo(modName,iDCount,ModDataList));

                dataEle=dataEle.nextSiblingElement();
            }
            break;
        }

        childEle=childEle.nextSiblingElement();
    }
}

void DataEditWin::AddDataToMod(const QString &modName, const int &iDCount, QList<QString> &fieldList)
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

    QDomElement childEle= root.firstChildElement();
    while (!childEle.isNull()){
        QString tModName=childEle.nodeName();
        if(tModName==modName){
            childEle.setAttribute("DataIDCount",iDCount);

            QDomElement dataEmelNode = doc.createElement("Data");
            dataEmelNode.setAttribute("IDCount",iDCount);
            int fieldListSize=fieldList.size();
            for (int col=1;col<=fieldListSize;col++ ) {
                QString fieldKey=QString("%1.%2").arg(modName).arg(col);
                QString fieldValue=fieldList.at(col-1);//ui->tableWidget_2->item(row,col+1)->text();
                dataEmelNode.setAttribute(fieldKey,fieldValue);// 给节点创建属性 // 参数一是字符串，参数二可以是任何类型
            }
            childEle.appendChild(dataEmelNode);
            break;
        }
        childEle=childEle.nextSiblingElement();
    }

    if (!file.open(QFileDevice::WriteOnly|QIODevice::Truncate)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }

    QTextStream stream(&file);
#if (QT_VERSION <= QT_VERSION_CHECK(SplitMajor,SplitMinor,SplitPatch))
    stream.setCodec("utf-8");
#else

#endif
    doc.save(stream, 2);		// 缩进2格
    file.close();
}

void DataEditWin::AddDataToTableWidget2(QTableWidget *tableWidget, const int &iDCount, QList<QString> &fieldList)
{
    int curRow=tableWidget->rowCount();
    tableWidget->insertRow(curRow);//插入一行

    tableWidget->setItem(curRow,0,new QTableWidgetItem(QString("%1").arg(iDCount)));
    int fieldListSize= fieldList.size();
    for (int col=0;col<fieldListSize ;col++ ) {
        tableWidget->setItem(curRow,col+1,new QTableWidgetItem(fieldList.at(col)));
    }
    tableWidget->setRowHeight(curRow, 30);
}

void DataEditWin::DeleteDataToMod(const QString &modName, const QString &dataId)
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

    QDomElement childEle= root.firstChildElement();
    while (!childEle.isNull()){
        QString tModName=childEle.nodeName();
        if(tModName==modName){
            QDomElement dataEle= childEle.firstChildElement();
            while (!dataEle.isNull()){
                QString iDCount=dataEle.attribute("IDCount");
                if(iDCount==dataId){
                    childEle.removeChild(dataEle);
                    break;
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
#if (QT_VERSION <= QT_VERSION_CHECK(SplitMajor,SplitMinor,SplitPatch))
    stream.setCodec("utf-8");
#else

#endif
    doc.save(stream, 2);		// 缩进2格
    file.close();
}

void DataEditWin::ModifyDataToMod(const QString &modName,const QString &dataId, const QList<QString> &modDataList)
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

    QDomElement childEle= root.firstChildElement();
    while (!childEle.isNull()){
        QString tModName=childEle.nodeName();
        if(tModName==modName){
            QDomElement dataEle= childEle.firstChildElement();
            while (!dataEle.isNull()){
                QString iDCount=dataEle.attribute("IDCount");
                if(iDCount==dataId){
                    int modDataListSize=modDataList.size();
                    for (int index=0;index<modDataListSize ;index++ ) {
                        QString fieldKey=QString("%1.%2").arg(modName).arg(index+1);
                        QString fieldValue=modDataList.at(index);
                        dataEle.setAttribute(fieldKey,fieldValue);
                    }
                    break;
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
#if (QT_VERSION <= QT_VERSION_CHECK(SplitMajor,SplitMinor,SplitPatch))
    stream.setCodec("utf-8");
#else

#endif
    doc.save(stream, 2);		// 缩进2格
    file.close();
}

void DataEditWin::PoptableWidget1Menu()
{
    //创建菜单
    QMenu *pMenu = new QMenu(this);
    QAction *pPasteData = new QAction(tr("粘贴数据"), this);
    QAction *pAddData = new QAction(tr("添加数据"), this);
    QAction *pResetData = new QAction(tr("重置数据"), this);

    pPasteData->setObjectName("PasteData");
    pAddData->setObjectName("AddData");
    pResetData->setObjectName("ResetData");

    pMenu->addAction(pPasteData);
    pMenu->addSeparator();
    pMenu->addAction(pAddData);
    pMenu->addSeparator();
    pMenu->addAction(pResetData);

    //连接鼠标右键点击信号
    connect(pPasteData, &QAction::triggered, this, &DataEditWin::OnClickedPopMenu);
    connect(pAddData, &QAction::triggered, this, &DataEditWin::OnClickedPopMenu);
    connect(pResetData, &QAction::triggered, this, &DataEditWin::OnClickedPopMenu);

    //在鼠标右键点击的地方显示菜单
    pMenu->exec(cursor().pos());

    //释放内存,若此处不手动释放，则必须等到程序结束时才都能释放
    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list)
        delete pAction;
    delete pMenu;
}

void DataEditWin::PoptableWidget2Menu()
{
    //创建菜单
    QMenu *pMenu = new QMenu(this);
    QAction *pDeleteData = new QAction(tr("删除数据"), this);
    QAction *pModifyData = new QAction(tr("修改数据"), this);
    QAction *pCopyData = new QAction(tr("复制数据"), this);

    pDeleteData->setObjectName("DeleteData");
    pModifyData->setObjectName("ModifyData");
    pCopyData->setObjectName("CopyData");

    //把QAction对象添加到菜单上
    pMenu->addAction(pDeleteData);
    //添加分隔线
    pMenu->addSeparator();
    pMenu->addAction(pModifyData);
    pMenu->addSeparator();
    pMenu->addAction(pCopyData);

    //连接鼠标右键点击信号
    connect(pDeleteData, &QAction::triggered, this, &DataEditWin::OnClickedPopMenu);
    connect(pModifyData, &QAction::triggered, this, &DataEditWin::OnClickedPopMenu);
    connect(pCopyData, &QAction::triggered, this, &DataEditWin::OnClickedPopMenu);

    //在鼠标右键点击的地方显示菜单
    pMenu->exec(cursor().pos());

    //释放内存,若此处不手动释放，则必须等到程序结束时才都能释放
    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list)
        delete pAction;
    delete pMenu;
}

void DataEditWin::AddDataFun()
{
    QString modName=ModListInfoList.at(CurModListInfoListIndex).ModName;
    int iDCount=ModListInfoList.at(CurModListInfoListIndex).DataIDCount+1;
    int modFieldCount=ui->tableWidget->columnCount();
    QList<QString> fieldList;fieldList.clear();
    for (int Col=0;Col< modFieldCount;Col++ ) {
        QString fieldTxt=ui->tableWidget->item(0,Col)->text();
        fieldList.append(fieldTxt);
    }

    ModListInfoList[CurModListInfoListIndex].DataIDCount=iDCount;

    ModDataInfoList.append(ModDataInfo(modName,iDCount,fieldList));
    AddDataToMod(modName,iDCount,fieldList);
    AddDataToTableWidget2(ui->tableWidget_2,iDCount,fieldList);
}

void DataEditWin::PasteDataFun()
{
    //复制逻辑 粘贴逻辑 具体如何待定
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    QString originalText  = clipboard->text();            //获取剪贴板上文本信息
    QString txt=QString(originalText.replace("\n",""));//originalText.replace("\n","") 有问题待定
    qDebug()<<"txt: "<<txt;
    QStringList srcLIST=txt.split("\t");
    QList<QString> outLIST;outLIST.clear();
    int srcLISTSize=srcLIST.size();
    for (int index=0;index<srcLISTSize ;index++ ) {
        qDebug()<<srcLIST[index];
        if(srcLIST[index].trimmed()!="")
            outLIST.append(srcLIST[index]);
    }
    int colCount=ui->tableWidget->columnCount();
    int outLISTSize=outLIST.size();
    //int colCount=ui->tableWidget->columnCount();
    //这里或许需要比较 outLISTSize 与 colCount 关键是 originalText格式是否标准
    for (int Col=0;Col< outLISTSize;Col++ ) {
        if((CopyDisColIndex+Col)>=colCount)
            break;
        //ui->tableWidget->item(0,Col)->setText(outLIST.at(Col));//from 0 to outLISTSize
        ui->tableWidget->item(0,CopyDisColIndex+Col)->setText(outLIST.at(Col));
    }
}

void DataEditWin::ResetDataFun()
{
    int colCount=ui->tableWidget->columnCount();
    for (int Col=0;Col< colCount;Col++ ) {
        ui->tableWidget->item(0,Col)->setText("");
    }
}

void DataEditWin::DeleteDataFun(int curRowIndex)
{
    QString modName=ModListInfoList.at(CurModListInfoListIndex).ModName;
    QString dataId=ui->tableWidget_2->item(curRowIndex,0)->text().trimmed();
    ui->tableWidget_2->removeRow(curRowIndex);
    ModDataInfoList.removeAt(curRowIndex);
    DeleteDataToMod(modName,dataId);
}

void DataEditWin::ModifyDataFun(int curRowIndex)
{
    QString modName=ModListInfoList.at(CurModListInfoListIndex).ModName;
    int modFieldCount=ModListInfoList.at(CurModListInfoListIndex).ModFieldCount.toInt();
    QString dataId=ui->tableWidget_2->item(curRowIndex,0)->text().trimmed();
    QList<QString> modDataList;modDataList.clear();
    for (int col=1;col<=modFieldCount ;col++ ) {
        QString modData=ui->tableWidget_2->item(curRowIndex,col)->text().trimmed();
        modDataList.append(modData);
    }
    ModifyDataToMod(modName,dataId,modDataList);

    int ModDataInfoListSize=ModDataInfoList.size();
    for (int index=0;index<ModDataInfoListSize ;index++ ) {
        if(ModDataInfoList.at(index).ModName==modName&&ModDataInfoList.at(index).IDCount==dataId.toInt()){
            ModDataInfoList[index].ModData=modDataList;
            break;
        }
    }
}

void DataEditWin::CopyDataFun()
{
    QString copyTxt="";
    QList<QTableWidgetItem*>items=ui->tableWidget_2->selectedItems();
    int selectedCount=items.count();
    if(selectedCount<=0)
        return;

    int row=items.at(0)->row();
    for(int index=0;index<selectedCount;index++){
        //int row=ui->tableWidget_2->row(items.at(index));//获取选中的行
        //        if(CopySrcRowIndex!=item->row())
        //            continue;
        QTableWidgetItem*item=items.at(index);
        QString txt=item->text();//获取内容
        if(row==item->row()){
            copyTxt+=txt+"\t";
        }else {
            row=item->row();
            copyTxt+="\n";
            copyTxt+=txt+"\t";
        }
    }

    qDebug()<<"copyTxt: "<<copyTxt;

    QClipboard *clip = QApplication::clipboard();
    clip->setText(copyTxt);
}

void DataEditWin::ComboBoxActivated(int index)
{
    QComboBox *ComboBox = qobject_cast<QComboBox*>(sender());
    qDebug()<<ComboBox->objectName()<<"  "<<ComboBox->currentText();
    if(ComboBox->objectName()=="comboBox"){
        if(index<0)
            return;

        CurModListInfoListIndex=index;
        QStringList header1;header1.clear();
        QStringList header2;header2.clear();
        QString modName=ModListInfoList.at(index).ModName;
        header2<<tr("IDCount");
        int FieldListSize=ModListInfoList.at(index).FieldList.size();
        for (int tIndex=0;tIndex< FieldListSize;tIndex++ ) {
            header1<<ModListInfoList.at(index).FieldList.at(tIndex);
            header2<<ModListInfoList.at(index).FieldList.at(tIndex);
        }

        ui->groupBox_2->setTitle(QString(tr("【%1】数据添加")).arg(modName));
        ui->groupBox_3->setTitle(QString(tr("模型【%1】数据列表")).arg(modName));

        ResetTableWidget1Data(ui->tableWidget,header1);
        //然后加载数据到 ui->tableWidget_2 中
        SetTableWidget2Data(ui->tableWidget_2,header2,modName);
    }
}

void DataEditWin::ToolButtonClicked(bool checked)
{
    QToolButton *ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="toolButton"){

    }
}

void DataEditWin::ShowTableWidgetMenuSlots(const QPoint &pos)
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

void DataEditWin::OnClickedPopMenu()
{
    QAction *pAction = qobject_cast<QAction*>(this->sender());
    qDebug()<<pAction->objectName();
    if(pAction->objectName()=="PasteData"){
        PasteDataFun();
    }
    else if (pAction->objectName()=="AddData") {
        AddDataFun();
    }
    else if (pAction->objectName()=="ResetData") {
        ResetDataFun();
    }

    else if (pAction->objectName()=="DeleteData") {
        DeleteDataFun(CurRowIndex);
    }
    else if (pAction->objectName()=="ModifyData") {
        ModifyDataFun(CurRowIndex);
    }
    else if (pAction->objectName()=="CopyData") {
        CopyDataFun();
    }
}
