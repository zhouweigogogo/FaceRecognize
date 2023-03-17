#include "thirdwindow.h"
#include "ui_thirdwindow.h"

ThirdWindow::ThirdWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThirdWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("后台考试数据管理"));//设置对话框的标题
    ui->qTableWidget->setColumnCount(12);//设置列数
    ui->qTableWidget->setRowCount(10);//设置行数
    ui->qTableWidget->setWindowTitle("QTableWidget");
    QStringList m_Header;
    m_Header<<QString("院系")<<QString("专业班级")<<QString("姓名")<<QString("班级")
           <<QString("性别")<<QString("学号")<<QString("考次")<<QString("考场号")
          <<QString("座位号")<<QString("入场时间")<<QString("人工操作");
    ui->qTableWidget->setHorizontalHeaderLabels(m_Header);//添加横向表头
    ui->qTableWidget->verticalHeader()->setVisible(true);//纵向表头可视化
    ui->qTableWidget->horizontalHeader()->setVisible(true);//横向表头可视化
    //ui->tableWidget->setShowGrid(false);//隐藏栅格
    ui->qTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置编辑方式：禁止编辑表格
    ui->qTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格选择方式：设置表格为整行选中
    //ui->qTableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);//设置表格选择方式：设置表格为整列选中
    ui->qTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//选择目标方式
    ui->qTableWidget->setStyleSheet("selection-background-color:pink");//设置选中颜色：粉色

    connect(this,SIGNAL(start_flag()),this,SLOT(write_flag()));
    // 导入数据
    save_csv save;
    save.test_csv();
    QStringList lines = save.read_csv();
    rows = lines.size();
    // row=1,因为要去掉csv里的列标签
    for(int row=1;row<rows;row++)
    {
        QString line = lines.at(row);
        QStringList split = line.split(",");/*行数据*/
        cols = split.size();

        for(int col=0;col<cols;col++)
        {
            ui->qTableWidget->setItem(row-1,col,new QTableWidgetItem(QString::fromUtf8(split.at(col).toStdString().data())));
            //            if (col == 5)
            //                IDList.push_back(split.at(col));
        }
        ui->qTableWidget->setItem(row-1,cols,new QTableWidgetItem(QString::fromUtf8("无")));
        ui->qTableWidget->setItem(row-1,cols+1,new QTableWidgetItem(QString::fromUtf8("无")));

        IDList.push_back(split.at(5));
    }
    // 样式配置
    for(int row=1;row<rows;row++)
    {
        for(int col=0;col<cols;col++)
        {
            ui->qTableWidget->setColumnWidth(col,125);
            ui->qTableWidget->setRowHeight(row,30);
            ui->qTableWidget->item(row-1,col)->setTextAlignment(Qt::AlignCenter);//居中显示
            ui->qTableWidget->item(row-1,col)->setBackgroundColor(QColor(85,170,255));//设置前景颜色
            ui->qTableWidget->item(row-1,col)->setTextColor(QColor(0,0,0));//设置文本颜色
            ui->qTableWidget->item(row-1,col)->setFont(QFont("Helvetica"));//设置字体为黑体
        }
    }

    ui->qTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//设置水平滚动条
    ui->qTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//设置垂直滚动条
    //设置行和列的大小设为与内容相匹配（如果设置了宽高就不要用了）
    ui->qTableWidget->resizeColumnsToContents();
    ui->qTableWidget->resizeRowsToContents();

    QString strText = ui->qTableWidget->item(1, 1)->text();//获取单元格的内容
    qDebug()<<"单元格内容："<<strText.toUtf8();//输出单元格内容

    //设置列标签
    QStringList HStrList;
    HStrList.push_back(QString::fromUtf8("序号"));
    HStrList.push_back(QString::fromUtf8("院系"));
    HStrList.push_back(QString::fromUtf8("专业班级"));
    HStrList.push_back(QString::fromUtf8("姓名"));
    HStrList.push_back(QString::fromUtf8("性别"));
    HStrList.push_back(QString::fromUtf8("学号"));
    HStrList.push_back(QString::fromUtf8("考试科目"));
    HStrList.push_back(QString::fromUtf8("考次"));
    HStrList.push_back(QString::fromUtf8("考场号"));
    HStrList.push_back(QString::fromUtf8("座位号"));
    HStrList.push_back(QString::fromUtf8("入场时间"));
    HStrList.push_back(QString::fromUtf8("人工操作"));

    ui->qTableWidget->setHorizontalHeaderLabels(HStrList);
}

ThirdWindow::~ThirdWindow()
{
    delete ui;
}

void ThirdWindow::receiveTime(string &time)
{
    passed_time = time;
    cout<<passed_time<<endl;
}

void ThirdWindow::receiveID(int &ID)
{
    passed_ID = ID;
    cout<<passed_ID<<endl;
}

void ThirdWindow::receiveFindID(QString &findedID)
{
    finded_ID = findedID;
    cout<<finded_ID.toStdString().data()<<endl;
    emit start_flag();
}

void ThirdWindow::writetime()
{
    QString time = QString(QString::fromLocal8Bit(passed_time.c_str()));
    ui->qTableWidget->setItem(passed_ID-1,10,new QTableWidgetItem(time));
    ui->qTableWidget->setItem(passed_ID-1,11,new QTableWidgetItem(QString::fromUtf8("否")));
}

// 导出数据(待修改)
void ThirdWindow::write_csv()
{
    QFile outFile("./data/data_output.csv");
    if (outFile.open(QIODevice::WriteOnly)){
        outFile.write("序号,院系,专业班级,姓名,性别,学号,考试科目,考次,考场号,座位号,入场时间,人工操作\n");
        for(int row=0;row<4;row++)
        {
            for(int col=0;col<12;col++)
            {
                QString strText = ui->qTableWidget->item(row,col)->text();
                //                qDebug()<<strText.toUtf8().data();
                if (col == 11){
                    strText.append("\n");
                }
                else {
                    strText.append(",");
                }
                // 这简单的一步凝聚我数小时的心血，QString->string
                outFile.write(strText.toUtf8().data());
            }
        }
    }
    outFile.close();
    cout<<"导出成功"<<endl;

}

void ThirdWindow::write_flag()
{
    QDateTime time = QDateTime::currentDateTime();
    QString dateStr =time.toString("yyyy-MM-dd");
    char passed_time[64] = {0};
    int temp = 0;
    for(int i = 0; i < IDList.size(); i++){
        QString ID = IDList.at(i);
        if (ID.toStdString() == finded_ID.toStdString()){
            ui->qTableWidget->setItem(i,11,new QTableWidgetItem(QString::fromUtf8("是")));

            sprintf(passed_time,"%s_%d-%d-%d",dateStr.toStdString().c_str(),
                    time.time().hour(),time.time().minute(),time.time().second());
            string time_str(passed_time);
            QString end_time = QString(QString::fromLocal8Bit(time_str.c_str()));
            ui->qTableWidget->setItem(i,10,new QTableWidgetItem(end_time));
            temp = 1;
            QMessageBox::about(this, QString::fromUtf8("提示"), QString::fromUtf8("人工操作成功"));
        }
    }
    if (temp == 0)
        QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("数据库未录入该学号！"), QMessageBox::Yes);
}
