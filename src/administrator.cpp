#include "administrator.h"
#include "ui_administrator.h"
#include "firstwindow.h"
#include <QMessageBox>
#include <QString>

int flag=0;

Administrator::Administrator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Administrator)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//无边框设置

    ui->ID->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                          "color: rgb(31,31,31);"		//字体颜色
                          "padding-left:20px;"       //内边距-字体缩进
                          "background-color: rgb(255, 255, 255);" //背景颜色
                          "border:2px solid rgb(20,196,188);border-radius:10px;");

    ui->Pwd->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                           "color: rgb(31,31,31);"		//字体颜色
                           "padding-left:20px;"       //内边距-字体缩进
                           "background-color: rgb(255, 255, 255);" //背景颜色
                           "border:2px solid rgb(20,196,188);border-radius:10px;");
    ui->btnConfirm->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                  "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                  "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                  "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                  "border:2px solid rgb(20,196,188);");
    ui->btnExit->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                               "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                               "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                               "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                               "border:2px solid rgb(20,196,188);");
}

Administrator::~Administrator()
{
    delete ui;
}

void Administrator::on_btnConfirm_clicked()
{
    if((ui->ID->text() != "") && (ui->Pwd->text() == "1234")){
        QString ID = ui->ID->text();
        emit sendID(ID);
        ui->ID->clear();
        ui->Pwd->clear();
    }
    // 剩下的情况自己写
    //    else if((ui->ID->text() == "") && (ui->Pwd->text()= "1234")){
    //        QMessageBox::warning(this, QString::fromUtf8("提示"), QString::fromUtf8("用户名密码不匹配，请重新输入"), QMessageBox::Yes);
    //        ui->Pwd->clear();
    //    }
    //    else if((ui->ID->text() == "") && (ui->Pwd->text()!= "1234")){
    //        QMessageBox::warning(this, QString::fromUtf8("提示"), QString::fromUtf8("用户名密码不匹配，请重新输入"), QMessageBox::Yes);
    //        ui->Pwd->clear();
    //    }
}

void Administrator::on_btnExit_clicked()
{
    this->close();
}

void Administrator::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        emit ui->btnConfirm->click();
    }
}


