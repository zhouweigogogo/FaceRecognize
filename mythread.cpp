#include "mythread.h"
#include <net.h>
#include <QElapsedTimer>
#include <QVector>
using namespace std;
using namespace cv;


ncnnthread::ncnnthread(QObject *parent) : QObject(parent)
{

}

void ncnnthread::working(face_recognize& f)
{
    f.setParam_path("./model/facenet.param");
    f.setBin_path("./model/facenet.bin");
    f.setThreadNum(4);
    f.init();
    cout<<"ncnn初始化成功"<<endl;

    QElapsedTimer time;
    vector<float> face_feature;
    vector<pair<float,int>> results;
    time.start();
    Mat face = f.preprocess_img(f.faceImage);
    f.getFeature(face,face_feature);
    int ret = f.getCompare(face_feature,results);
    if(ret == true){
        f.getResult(results);
        cout<<"人脸距离："<<f.dist<<"人脸序号："<<f.num<<endl;
        emit finished1(f.dist);
        emit finished2(f.num);
    }
    else {
        emit finished1(0);
        emit finished2(0);
        cout<<"人脸距离："<<0<<"人脸序号："<<0<<endl;
    }
    emit finished();
    int milsec = time.elapsed();
    std::cout<<"耗时："<<milsec<<"毫秒"<<std::endl;
}
