#include "sql.h"
#include "ui_sql.h"


Sql::Sql(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sql)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//无边框设置
    ft.setPointSize(18);
    ui->changelineEdit->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                      "color: rgb(31,31,31);"		//字体颜色
                                      "padding-left:20px;"       //内边距-字体缩进
                                      "background-color: rgb(255, 255, 255);" //背景颜色
                                      "border:2px solid rgb(20,196,188);border-radius:10px;");

    ui->namelineEdit->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid rgb(20,196,188);border-radius:10px;");

    ui->numberlabel->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                   "color: rgb(31,31,31);"		//字体颜色
                                   "padding-left:20px;"       //内边距-字体缩进
                                   "background-color: rgb(255, 255, 255);" //背景颜色
                                   "border:2px solid rgb(20,196,188);border-radius:10px;");
    ui->querySql->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                             "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                             "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                             "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                             "border:2px solid rgb(20,196,188);");
    ui->updateSql->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                 "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                 "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                 "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                 "border:2px solid rgb(20,196,188);");
    ui->exit->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                            "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                            "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                            "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                            "border:2px solid rgb(20,196,188);");
    ui->testDB->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                            "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                            "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                            "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                            "border:2px solid rgb(20,196,188);");

    int frmX = width();
    int frmY = height();

    QDesktopWidget s;
    int deskWidth = s.width();
    int deskHeight = s.height();

    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    move(movePoint);
    d.openDB();
    d.getDB();
}

Sql::~Sql()
{
    delete ui;
}

void Sql::on_exit_clicked()
{
    this->close();
}

void Sql::on_querySql_clicked()
{
    d.name = ui->namelineEdit->text();
    if(d.queryDB() == true){
        ui->numberlabel->setFont(ft);
        ui->numberlabel->setText(d.ID);
        QMessageBox::information(this,  "Tip", QString::fromUtf8("查询成功!"));
    }
    else {
        QMessageBox::information(this,  "Tip", QString::fromUtf8("查询失败!"));
    }
    ui->numberlabel->clear();
}

void Sql::on_updateSql_clicked()
{
    d.name = ui->namelineEdit->text();
    d.ID = ui->changelineEdit->text();
    if (d.ID == ""){
        if(d.deleteDB() == true){
            ui->namelineEdit->clear();
            ui->numberlabel->clear();
            QMessageBox::information(this,  "Tip", QString::fromUtf8("删除成功!"));
        }
        else
            QMessageBox::information(this,  "Tip", QString::fromUtf8("删除失败!"));
    }
    else {
        if(d.updateDB() == true){
            ui->namelineEdit->clear();
            ui->numberlabel->clear();
            ui->changelineEdit->clear();
            QMessageBox::information(this,  "Tip", QString::fromUtf8("更新成功!"));
        }
        else
            QMessageBox::information(this,  "Tip", QString::fromUtf8("更新失败!"));
    }
}

void Sql::on_testDB_clicked()
{
    d.getDB();
    std::cout<<"test"<<std::endl;
}
