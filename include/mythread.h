#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
//#include <QMutex>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include "face_recognize.h"
using namespace std;

class ncnnthread : public QObject {
    Q_OBJECT
public:
    explicit ncnnthread(QObject *parent = nullptr);
//    volatile bool stopped;


signals:
    void finished1(float dist);
    void finished2(int num);
    void finished();

public slots:
    void working(face_recognize& f);
//    void start();

private:
    face_recognize f;

};

#endif // MYTHREAD_H
