#include "firstwindow.h"
#include "ui_mainwindow.h"


using namespace std;
using namespace cv;

// 一级窗口 组件
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    设置字体
    ft.setPointSize(18);
    //    设置菜单栏
    menu = this ->menuBar();
    menu = new QMenuBar(this);
    menu1 = new QMenu(QString::fromUtf8("功能菜单"),this);
    menu2 = new QMenu(QString::fromUtf8("考试管理"),this);
    action1_1 = new QAction(QString::fromUtf8("报名注册"),this);
    action1_2 = new QAction(QString::fromUtf8("导入数据"),this);
    action1_3 = new QAction(QString::fromUtf8("导出数据"),this);
    action2_1 = new QAction(QString::fromUtf8("安排考试"),this);
    action2_2 = new QAction(QString::fromUtf8("数据管理"),this);
    action2_3 = new QAction(QString::fromUtf8("查询考试"),this);
    action2_4 = new QAction(QString::fromUtf8("查询考生"),this);
    menu ->addMenu(menu1);
    menu ->addMenu(menu2);
    menu1 ->addAction(action1_1);
    menu1 ->addAction(action1_2);
    menu1 ->addAction(action1_3);
    menu2 ->addAction(action2_1);
    menu2 ->addAction(action2_2);
    menu2 ->addAction(action2_3);
    menu2 ->addAction(action2_4);
    menu ->addSeparator();//添加分隔线
    setMenuBar(menu);//使用自定义的菜单

//    second = new SecondWindow;//报名注册
    connect(action1_1, SIGNAL(triggered()),this,SLOT(on_action1_1_opened()));
    third = new ThirdWindow();//数据管理
    connect(action2_2, SIGNAL(triggered()),this,SLOT(on_action2_2_opened()));
    connect(action1_2,SIGNAL(triggered()),this,SLOT(on_action1_2_opened()));
    connect(action1_3,SIGNAL(triggered()),this,SLOT(on_action1_3_opened()));//导出数据

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
    connect(m_login,SIGNAL(sendID(QString&)),this,SLOT(receiveID(QString&)));
    connect(ui->btnGetinfo,SIGNAL(clicked(bool)),this,SLOT(on_btnpassed_clicked()));
    // 绑定时间界面
    QTimer *timer_calendar;
    timer_calendar = new QTimer(this);
    connect(timer_calendar,SIGNAL(timeout()),this,SLOT(showTime()));
    timer_calendar->start(1000);
    // 刷新置信度
    connect(this,SIGNAL(showinfo()),this,SLOT(showTrust()));
    // 发送入场时间、序号并写入
    connect(this,SIGNAL(sendTime(string&)),third,SLOT(receiveTime(string&)));
    connect(this,SIGNAL(sendID(int&)),third,SLOT(receiveID(int&)));
    connect(this,SIGNAL(writetime()),third,SLOT(writetime()));
    // 发送查询的学号
    connect(this,SIGNAL(sendFindedID(QString&)),third,SLOT(receiveFindID(QString&)));
    //参数设置（只为计算数据库中的人脸）
    faces_path = "./faces";
    f.setParam_path("./model/facenet.param");
    f.setBin_path("./model/facenet.bin");
    f.setThreadNum(4);
    f.init();
    //单独计算数据库中的人脸，避免重复计算
    getFeaturelist(featurelist,faces_path);
}

// 析构函数
MainWindow::~MainWindow()
{
    if(capture.isOpened())
        capture.release();
    delete ui;
}
// 图片保真
Mat MainWindow::preprocess_img(Mat &img)
{
    int w, h, x, y;
    float r_w = 160 / (img.cols*1.0);
    float r_h = 160 / (img.rows*1.0);
    if (r_h > r_w) {
        w = 160;
        h = r_w * img.rows;
        x = 0;
        y = (160 - h) / 2;
    }

    else {
        w = r_h * img.cols;
        h = 160;
        x = (160 - w) / 2;
        y = 0;
    }
    cv::Mat re(h, w, CV_8UC3);
    cv::resize(img, re, re.size(), 0, 0, cv::INTER_CUBIC);
    cv::Mat out(160, 160, CV_8UC3, cv::Scalar(128, 128, 128));
    re.copyTo(out(cv::Rect(x, y, re.cols, re.rows)));
    return out;
}
// 计算置信度
float MainWindow::calc_confidence(float distance)
{
    double a = -4.23196996733742;
    double b = 4.40188263713067;
    double x = a*distance + b;
    double y = 1 / (1 + exp(x));
    return (float) y;
}
// 计算数据库中的所有人脸的人脸特征
void MainWindow::getFeaturelist(std::vector<std::pair<std::vector<float>, int>> &featurelist, cv::String faces_path)
{
    cv::glob(faces_path, imgs_path);
    size_t count = imgs_path.size();
    int j = 0;
    for(size_t i = 0; i < count; i++){
        cout<<i<<endl;
        cv::Mat img = cv::imread(imgs_path[i]);
        img = preprocess_img(img);
        std::vector<float> feature;
        f.getFeature(img, feature);
        j++;
        featurelist.emplace_back(feature,j);
        feature.clear();
    }
}
// 显示时间
void MainWindow::showTime()
{
    QDateTime time = QDateTime::currentDateTime();
    time_str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_timer->setText(time_str);
}

// 打开摄像头
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
// 关闭摄像头
void MainWindow::on_btnCloseCam_clicked()
{
    capture.release();
    ui->lblImage->clear();
    ui->namelabel->clear();
    ui->numberlabel->clear();
    ui->trust->clear();
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
// 显示置信度
int id = 0;
void MainWindow::showTrust()
{
    char temp[20];
    ui->namelabel->setFont(ft);
    ui->numberlabel->setFont(ft);
    ui->trust->setFont(ft);

    if(result_num == 0){
        ui->namelabel->setText(QString::fromStdString("unkown"));
        ui->numberlabel->setText(QString::fromStdString("0000"));
        cv::Mat showImage = cv::imread(imgs_path[4]);
        showImage = preprocess_img(showImage);
        cv::cvtColor(showImage,showImage,cv::COLOR_BGR2RGB);
        QImage image = QImage((uchar*)(showImage.data),showImage.cols,showImage.rows,QImage::Format_RGB888);
        ui->lblImage->setPixmap(QPixmap::fromImage(image));
        ui->lblImage->resize(ui->lblImage->size());
        ui->lblImage->show();
    }
    else{
        // 得到返回数据
        //                f.getData(result_num);
        QString line = result_data.at(result_num-1);
        QStringList info = line.split(",");
        name = info.at(3);
        place = info.at(9);
        qDebug()<<name;
        qDebug()<<place;
        std::cout<<"人脸距离："<<result_dist<<std::endl;

        ui->namelabel->setText(QString::fromUtf8(name.toStdString().data()));
        ui->numberlabel->setText(place);
        emit sendID(result_num);
        // 图片
        cv::Mat showImage = cv::imread(imgs_path[result_num-1]);
        showImage = preprocess_img(showImage);
        cv::cvtColor(showImage,showImage,cv::COLOR_BGR2RGB);
        QImage image = QImage((uchar*)(showImage.data),showImage.cols,showImage.rows,QImage::Format_RGB888);

        ui->lblImage->setPixmap(QPixmap::fromImage(image));
        ui->lblImage->resize(ui->lblImage->size());
        ui->lblImage->show();
    }
    sprintf(temp, "%f", calc_confidence(result_dist));
    string dist = temp;
    ui->trust->setText(dist.data());
    // 发送入场时间写入后台
    emit writetime();
    //        // 人脸距离

    //        if (f.dist >0.8 ){
    //            f.dist = f.dist + 2;

    //            }

}
// 销毁线程
void MainWindow::killT1()
{
    t1->quit();
    t1->wait();
    t1->deleteLater();
    ncn->deleteLater();
    std::cout << "线程结束" << std::endl;
    emit showinfo();
}
// 开始识别
void MainWindow::on_btnStaring_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QString dateStr =time.toString("yyyy-MM-dd");

    char image_name[64] = {0};
    char passed_time[64] = {0};
    cv::Mat frame;
    capture >> frame;
    f.faceImage = frame;

    if (frame.empty())
    {
        std::cout << "frame is empty" << std::endl;
    }else{
        cout<<"frame.size= "<<frame.size<<endl;
        sprintf(image_name,"%s%s_%d-%d-%d%s","/apps/VideoCaptureQt/passed_faces/",dateStr.toStdString().c_str(),
                time.time().hour(),time.time().minute(),time.time().second(),".jpg");
        cout<<image_name<<endl;
        cv::imwrite(image_name, frame);
        // 发送时间
        sprintf(passed_time,"%s_%d-%d-%d",dateStr.toStdString().c_str(),
                time.time().hour(),time.time().minute(),time.time().second());
        string time(passed_time);
        emit sendTime(time);
        // 显示图片
        frame = preprocess_img(frame);//图片保真
        cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
        QImage image = QImage((uchar*)(frame.data),frame.cols,frame.rows,QImage::Format_RGB888);
        ui->lblImage->setPixmap(QPixmap::fromImage(image));
        ui->lblImage->resize(ui->lblImage->size());
        ui->lblImage->show();
        // 启动识别
        f.setFeatureList(featurelist);
        recognizeThread();
        emit starting(f);
    }
}
// 人工操作
void MainWindow::on_btnpassed_clicked()
{
    ui->lblImage->clear();
    ui->namelabel->clear();
    ui->numberlabel->clear();
    ui->trust->clear();
    m_login->show();
}
// 导入数据
void MainWindow::on_action1_2_opened()
{
    QFile inFile("./data/data_input.csv");
    QStringList lines;/*行数据*/
    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd())
        {
            lines.push_back(stream_text.readLine());
        }
        for(int i = 1; i < lines.size(); i++){
            QString line = lines.at(i);
            result_data.push_back(line);
        }
        inFile.close();
    }
    cout<<"导入成功"<<endl;
}

void MainWindow::on_action1_3_opened()
{
    third->write_csv();
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
    connect(ncn,SIGNAL(finished()),this,SLOT(killT1()));
    // 接收计算结果
    connect(ncn,SIGNAL(finished1(float)),this,SLOT(receiveDist(float)));
    connect(ncn,SIGNAL(finished2(int)),this,SLOT(receiveNum(int)));

    t1->start();
}
void MainWindow::receiveNum(int num)
{
    result_num = num;
}
void MainWindow::receiveDist(float dist)
{
    result_dist = dist;
}

void MainWindow::receiveID(QString & ID)
{
    finded_ID = ID;
    emit sendFindedID(finded_ID);
}

// 打开注册界面（暂时没用）
//void MainWindow::on_action1_1_opened()
//{
//    second->show();
//}
// 打开数据管理界面
void MainWindow::on_action2_2_opened()
{
    third->show();
}


