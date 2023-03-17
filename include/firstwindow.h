#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <QMainWindow>
#include <QThread>
#include <QFont>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
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
#include <math.h>
#include "thirdwindow.h"
#include "face_recognize.h"
#include "mythread.h"
#include "administrator.h"
#include "save_csv.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    SecondWindow *second;
    ThirdWindow *third;
    Administrator *m_login;
    cv::Mat preprocess_img(cv::Mat &img);
    float calc_confidence(float distance);


public slots:
    void updateImage();
    void killT1();
    void recognizeThread();
    void receiveNum(int);
    void receiveDist(float);
    void receiveID(QString& ID);
    void showTrust();

signals:
    void starting(face_recognize& f);
    void showinfo();
    void sendTime(string&);
    void sendID(int&);
    void writetime();
    void sendFindedID(QString&);

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void on_btnOpenCam_clicked();
    void on_btnCloseCam_clicked();
    void on_btnStaring_clicked();
    void on_btnpassed_clicked();
//    void on_action1_1_opened();
    void on_action1_2_opened();
    void on_action1_3_opened();
    void on_action2_2_opened();
    void showTime();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture capture;
    QTimer theTimer;
    QString time_str;
    cv::Mat srcImage;
    cv::Mat image_fliped;
    QFont ft;
    face_recognize f;
//    database d;
    QThread* t1 ;
    QMenuBar* menu;
    QMenu *menu1;
    QMenu *menu2;
    QAction *action1_1;
    QAction *action1_2;
    QAction *action1_3;
    QAction *action2_1;
    QAction *action2_2;
    QAction *action2_3;
    QAction *action2_4;
    ncnnthread *ncn;
    std::vector<std::pair<std::vector<float>, int>> featurelist;
    cv::String faces_path;
    std::vector<cv::String> imgs_path;
    int result_num;
    float result_dist;
    QStringList result_data;
    QString name;
    QString place;
    QString finded_ID;
    void getFeaturelist(std::vector<std::pair<std::vector<float>,int>> &featurelist, cv::String faces_path);
    void getData(int num);

};

#endif // MAINWINDOW_H
