#ifndef FACE_RECOGNIZE
#define FACE_RECOGNIZE
#pragma once

#include "net.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QString>
//#include "sqlite3_db.h"
class face_recognize {
public:
    face_recognize();

    ~face_recognize();

    float dist ;
    // 序号
    int num = 0;
    // 被检测人脸
    cv::Mat faceImage;
    // 返回数据
    QString name;

    QString ID;

    int facesNum;

    volatile bool stopped;

    void setThreadNum(int threads);

    void setParam_path(std::string param);

    void setBin_path(std::string bin);
    // faces 必须是绝对路径
    void setFaces_path(cv::String faces);

    void setNames_path(std::string names);

    void setIDs_path(std::string IDs);

    void setFeatureList(std::vector<std::pair<std::vector<float>,int>> &List);

    bool init();

    bool getCompare(std::vector<float>& face_feature,std::vector<std::pair<float,int>> &results);
    // resize,三线性插值
    void resizeImage(cv::Mat &img);

    int getFeature(cv::Mat &img, std::vector<float> &feature);

    int getFeatureList();

    void getData(int num);

    void getResult(std::vector<std::pair<float,int>> &results);

    float o_distance(std::vector<float>& feature1, std::vector<float>& feature2);

    cv::Mat preprocess_img(cv::Mat &img);
private:
    // 参数设置

    std::string param_path;

    std::string bin_path;

    cv::String faces_path;

    std::string names_path;

    std::string IDs_path;

    std::vector<std::string> nameData;

    std::vector<std::string> IdData;

    std::vector<std::pair<std::vector<float>,int>> featureList;

    int num_thread = 4;

    float confidence = 1.0;

    ncnn::Net facenet;

//    database d;

    int runNet(cv::Mat &img, std::vector<float>& feature);

    int runFeatureList(cv::String &faces_path);
    // 加载模型权重、参数

    bool runcompare(std::vector<float>& face_feature, std::vector<std::pair<float,int>> &results);

    void runResult(std::vector<std::pair<float,int>> &results);



};

const int feature_dim = 128;

const int resize_w_h = 160;

#endif // FACE_RECOGNIZE

