#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include "face_recognize.h"
#include "mythread.h"
#include <QThread>
#include <net.h>
#include <string>
#include <QElapsedTimer>

using namespace std;
using namespace cv;

// 一级窗口 组件
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 主界面配置
    ft.setPointSize(18);
    ui->btnGetinfo->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                  "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                  "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                  "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                  "border:2px solid rgb(20,196,188);");
    ui->btnOpenCam->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                  "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                  "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                  "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                  "border:2px solid rgb(20,196,188);");
    ui->btnCloseCam->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                   "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                   "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                   "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                   "border:2px solid rgb(20,196,188);");
    ui->btnStaring->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                  "border: 2px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                  "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                  "QPushButton:pressed{background-color: rgb(17,171,164);}"//pressed
                                  "border:2px solid rgb(20,196,188);");

    // 刷新画面
    connect(&theTimer, SIGNAL(timeout()), this, SLOT(updateImage()));
    // 绑定管理员界面
    m_login = new Administrator;
    connect(m_login,SIGNAL(get_login()),this,SLOT(on_btnGetinfo_clicked()));
    // 绑定时间界面
    QTimer *timer_calendar;
    timer_calendar = new QTimer(this);
    connect(timer_calendar,SIGNAL(timeout()),this,SLOT(showTime()));
    timer_calendar->start(1000);
    // 刷新置信度
    connect(&timer_show,SIGNAL(timeout()),this,SLOT(showTrust()));


    f.setParam_path("./model/facenet.param");
    f.setBin_path("./model/facenet.bin");
    f.setNames_path("name.txt");
    f.setIDs_path("ID.txt");
    f.setThreadNum(2);
    f.setFaces_path("./faces");
    cv::glob("./faces", imgs_path);
    d.openDB();
    d.new_table();
    flag = true;
    int ret = f.init();
    if (ret == true){
        cout<<"ncnn初始化成功"<<endl;
        f.getFeatureList();
        cout<<"ncnn载入人脸数据库成功"<<endl;

    }
    else {
        cout<<"ncnn初始化失败"<<endl;
    }

}

// 析构函数
MainWindow::~MainWindow()
{
    if(capture.isOpened())
        capture.release();
    delete ui;
}


// 一级窗口：显示时间
void MainWindow::showTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_timer->setText(str);
}


// 一级窗口：打开摄像头
void MainWindow::on_btnOpenCam_clicked()
{
    if (capture.isOpened())
        return;

    if (capture.open(200)) {
        cout<<"cam is  opened"<<endl;
        //从摄像头捕获视频
        capture.set(CV_CAP_PROP_FRAME_WIDTH,320);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);
        srcImage = Mat::zeros(capture.get(CV_CAP_PROP_FRAME_HEIGHT), capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
        theTimer.start(33);
    }else {
        cout<<"cam is not opened"<<endl;
        capture.release();
    }
}


// 一级窗口：关闭摄像头
void MainWindow::on_btnCloseCam_clicked()
{
    capture.release();
}

// 加载视频流
void MainWindow::paintEvent(QPaintEvent *e)
{
    QImage image1 = QImage((uchar*)(image_fliped.data), image_fliped.cols, image_fliped.rows, QImage::Format_RGB888);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image1));
    ui->imageLabel->resize(image1.size());
    ui->imageLabel->show();
}


// 刷新视频流
void MainWindow::updateImage()
{
    capture>>srcImage;
    f.faceImage = srcImage;
    cv::flip(srcImage, image_fliped, 1);

    if(image_fliped.data)
    {
        cvtColor(image_fliped, image_fliped, CV_BGR2RGB);//Qt中支持的是RGB图像, OpenCV中支持的是BGR
        this->update();  //发送刷新消息
    }
}

int id = 0;
void MainWindow::showTrust()
{
    // 得到返回数据
    f.getData(f.num);
    std::cout<<"人脸距离："<<f.dist<<std::endl;

    // 人脸距离
    char temp[20];
    if (f.dist >0.8 ){
        f.dist = f.dist + 2;
        if (f.num != id){
            id = f.num;
            // 设置字体
            ui->namelabel->setFont(ft);
            ui->numberlabel->setFont(ft);

            ui->namelabel->setText(f.name);
            ui->numberlabel->setText(f.ID);
            // 图片
            cv::Mat showImage = cv::imread(imgs_path[f.num-1]);
            cv::resize(showImage, showImage, cv::Size(160 ,160),cv::INTER_CUBIC);
            cv::cvtColor(showImage,showImage,cv::COLOR_BGR2RGB);
            QImage image = QImage((uchar*)(showImage.data),showImage.cols,showImage.rows,QImage::Format_RGB888);

            ui->lblImage->setPixmap(QPixmap::fromImage(image));
            ui->lblImage->resize(ui->lblImage->size());
            ui->lblImage->show();
        }
        sprintf(temp, "%f", f.calc_confidence(f.dist));
        string dist = temp;
        ui->trust->setFont(ft);
        ui->trust->setText(dist.data());
    }
    else{
        ui->namelabel->setFont(ft);
        ui->numberlabel->setFont(ft);

        ui->namelabel->setText(QString::fromStdString("unkown"));
        ui->numberlabel->setText(QString::fromStdString("0000"));
        // 图片
        cv::Mat showImage = cv::imread(imgs_path[4]);
        cv::resize(showImage, showImage, cv::Size(160 ,160),cv::INTER_CUBIC);
        cv::cvtColor(showImage,showImage,cv::COLOR_BGR2RGB);
        QImage image = QImage((uchar*)(showImage.data),showImage.cols,showImage.rows,QImage::Format_RGB888);

        ui->lblImage->setPixmap(QPixmap::fromImage(image));
        ui->lblImage->resize(ui->lblImage->size());
        ui->lblImage->show();
    }

}

void MainWindow::killT1()
{
    t1->quit();
    t1->wait();
    t1->deleteLater();
}

void MainWindow::on_btnStaring_clicked()
{
    if(flag == true){
        recognizeThread();
    }

    emit starting(f);

}

void MainWindow::recognizeThread()
{

    t1= new QThread();
    ncn = new ncnnthread();
    ncn->moveToThread(t1);
    // 让t1自杀
    connect(t1,SIGNAL(finished()),t1,SLOT(deleteLater()));
    // 启动线程
    connect(this,SIGNAL(starting(face_recognize&)),ncn,SLOT(working(face_recognize&)));
    // 回收线程
    //    connect(this,SIGNAL(stop()),this,SLOT(killT1()));

    connect(ncn,SIGNAL(finished1(float)),this,SLOT(receiveDist(float)));

    connect(ncn,SIGNAL(finished2(int)),this,SLOT(receiveNum(int)));

    t1->start();

    timer_show.start(1500);
}

void MainWindow::receiveNum(int num)
{
    f.num = num;
}

void MainWindow::receiveDist(float dist)
{
    f.dist = dist;
}

// 一级窗口：管理员登录
void MainWindow::on_btnGetinfo_clicked()
{
    capture.release();
    ui->lblImage->clear();
    ui->namelabel->clear();
    ui->numberlabel->clear();
    ui->trust->clear();
    m_login->show();
}


