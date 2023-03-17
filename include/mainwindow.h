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
#include "secondwindow.h"
#include "face_recognize.h"
#include "mythread.h"
#include "administrator.h"
#include "sqlite3_db.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SecondWindow *m_second;
    Administrator *m_login;
    cv::Mat faceImage;
    int flag;

public slots:
    void updateImage();
    void killT1();
    void recognizeThread();
    void receiveNum(int);
    void receiveDist(float);

signals:
    void starting(face_recognize &f);

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void on_btnOpenCam_clicked();

    void on_btnCloseCam_clicked();

    void on_btnStaring_clicked();

    void on_btnGetinfo_clicked();

    void showTrust();

    void showTime();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture capture;
    QTimer theTimer;
    QTimer timer_show;
    cv::Mat srcImage;
    cv::Mat image_fliped;
    QFont ft;
    face_recognize f;
    database d;
    QThread* t1 ;
    ncnnthread *ncn;
    std::vector<cv::String> imgs_path;

};

#endif // MAINWINDOW_H
