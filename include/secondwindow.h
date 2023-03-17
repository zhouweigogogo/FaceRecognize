#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QTimer>
#include <QPaintEvent>
#include "face_recognize.h"
#include "sql.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include "sqlite3_db.h"

namespace Ui{
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = 0);
    ~SecondWindow();
    Sql *s_window;


private:
    Ui::SecondWindow *ui;
    cv::VideoCapture capture2;
    cv::Mat entryImage;
    cv::Mat image_fliped;
    QTimer entryTimer;
    int num;
    database d;
//    void write_txt(std::string s,std::string path);

protected:
    void paintEvent(QPaintEvent *e);

signals:
    void close_main();

private slots:
    void on_btn2Backmain_clicked(); //返回界面的函数

    void on_btn2Opencam_clicked();   //打开摄像头的函数

    void on_btn2Shot_clicked();

    void updateImage();

    void on_btn2Confirm_clicked();

    void on_btn2Change_clicked();
};


#endif // SECONDWINDOW_H
