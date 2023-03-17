#include "secondwindow.h"
#include "ui_secondwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QPixmapCache>


using namespace std;
using namespace cv;

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//无边框设置
//    QPixmapCache::setCacheLimit(1);
    connect(&entryTimer, SIGNAL(timeout()), this, SLOT(updateImage()));

    ui->btn2Confirm->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                   "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                   "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                   "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                   "border:2px solid rgb(20,196,188);");
    ui->btn2Opencam->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                   "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                   "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                   "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                   "border:2px solid rgb(20,196,188);");
    ui->btn2Backmain->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                    "border:2px solid rgb(20,196,188);");
    ui->btn2Shot->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                "border:2px solid rgb(20,196,188);");
    ui->editName->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                "color: rgb(31,31,31);"		//字体颜色
                                "padding-left:20px;"       //内边距-字体缩进
                                "background-color: rgb(255, 255, 255);" //背景颜色
                                "border:2px solid rgb(20,196,188);border-radius:10px;");
    ui->editId->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                              "color: rgb(31,31,31);"		//字体颜色
                              "padding-left:20px;"       //内边距-字体缩进
                              "background-color: rgb(255, 255, 255);" //背景颜色
                              "border:2px solid rgb(20,196,188);border-radius:10px;");

    ui->btn2Change->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                  "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                  "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                  "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                  "border:2px solid rgb(20,196,188);");


    s_window = new Sql;
    connect(s_window,SIGNAL(close_second()),this,SLOT(on_btn2Change_clicked()));
    d.openDB();
}

SecondWindow::~SecondWindow()
{
    if (capture2.isOpened())
        capture2.release();
    delete ui;
}

void SecondWindow::on_btn2Backmain_clicked()
{
    capture2.release();
    ui->lblImage2->clear();
    QWidget::close();   //关闭子窗口即实现了返回主界面的功能
}

void SecondWindow::on_btn2Opencam_clicked()
{
    if (capture2.isOpened())
        return;

    if (capture2.open(200))
    {
        cout<<"cam is opened"<<endl;
        // 查询数据库的人数
        std::string namePath = "name.txt";
        std::ifstream in(namePath.data());
        std::vector<std::string> nameData;
        std::string line;
        while (getline(in, line))
        {
            nameData.emplace_back(line);
        }
        num = nameData.size();
        std::cout<<"数据库共有: "<<num<<"人"<<std::endl;
        nameData.clear();

        //视频的捕获
        capture2.set(CV_CAP_PROP_FRAME_WIDTH,320);
        capture2.set(CV_CAP_PROP_FRAME_HEIGHT,240);
        entryImage = Mat::zeros(capture2.get(CV_CAP_PROP_FRAME_HEIGHT),capture2.get(CV_CAP_PROP_FRAME_WIDTH),CV_8UC3);
        entryTimer.start(33);
    }
    else
    {
        cout<<"cam is not opened"<<endl;
    }
}

void SecondWindow::on_btn2Shot_clicked()
{
    char image_name[64] = {0};
    cv::Mat frame2;
    cv::Mat frame3;
    capture2 >> frame2;

    if (frame2.empty())
    {
        std::cout << "frame2 is empty" << std::endl;
    }else{
        cout<<"id="<< num++ <<", frame2.size= "<<frame2.size<<endl;
        sprintf(image_name,"%s%d%s","/apps/VideoCaptureQt/faces/", num,".jpg");
        cout<<image_name<<endl;
        //显示图片
        cv::resize(frame2, frame3, cv::Size(240 ,240),cv::INTER_CUBIC);
        cv::cvtColor(frame3,frame3,cv::COLOR_BGR2RGB);
        QImage image2 = QImage((uchar*)(frame3.data),frame3.cols,frame3.rows,QImage::Format_RGB888);

        ui->lblImage2->setPixmap(QPixmap::fromImage(image2));
        ui->lblImage2->resize(ui->lblImage2->size());
        ui->lblImage2->show();

        QMessageBox::StandardButton result =QMessageBox::information(this,  "Tip", QString::fromUtf8("请确定是否保存这张图片到数据库"), QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(result == QMessageBox::Yes){
            cv::imwrite(image_name, frame2);
            QMessageBox::information(this,  "Tip", QString::fromUtf8("保存图片之后请记得录入姓名和学号哦！"));
        }
        else {
            num--;
        }
    }
}

void SecondWindow::paintEvent(QPaintEvent *e)     //视频定向到Label框
{
    QImage image1 = QImage((uchar*)(image_fliped.data),image_fliped.cols,image_fliped.rows,QImage::Format_RGB888);

    ui->videoLabel->setPixmap(QPixmap::fromImage(image1));
    ui->videoLabel->resize(image1.size());
    ui->videoLabel->show();
}

void SecondWindow::updateImage()
{
    capture2>>entryImage;
    cv::flip(entryImage, image_fliped, 1);
    if(image_fliped.data)
    {
        cvtColor(image_fliped, image_fliped, CV_BGR2RGB);//Qt中支持的是RGB图像, OpenCV中支持的是BGR
        this->update();  //发送刷新消息
    }
}

void SecondWindow::on_btn2Confirm_clicked()
{
    if ((ui->editName->text() == "") && (ui->editId->text() == "")){
        QMessageBox::information(this,  "Tip", QString::fromUtf8("输入不能为空！"));
    }
    else if((ui->editName->text() != "") && (ui->editId->text() == "")){
        QMessageBox::information(this,  "Tip", QString::fromUtf8("学号不能为空！"));
    }
    else if((ui->editName->text() == "") && (ui->editId->text() != "")){
        QMessageBox::information(this,  "Tip", QString::fromUtf8("姓名不能为空！"));
    }
    else {
        d.name = ui->editName->text();
        d.ID = ui->editId->text();
        d.insertDB();
        QMessageBox::information(this,  "Tip", QString::fromUtf8("写入成功!"));
        ui->editName->clear();
        ui->editId->clear();
    }
}

void SecondWindow::on_btn2Change_clicked()
{
    ui->lblImage2->clear();
    s_window->show();
}
