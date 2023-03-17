#include "face_recognize.h"
#include <math.h>
#include <fstream>
#include <algorithm>

face_recognize::face_recognize(){
}

face_recognize::~face_recognize(){
    std::cout<<"facenet释放成功"<<std::endl;
}

void face_recognize::setThreadNum(int threads){
    num_thread = threads;
}

void face_recognize::setParam_path(std::string param){
    param_path = param;
}

void face_recognize::setBin_path(std::string bin){
    bin_path = bin;
}

void face_recognize::setFaces_path(cv::String faces){
    faces_path = faces;
}

void face_recognize::setNames_path(std::string names){
    names_path = names;
}

void face_recognize::setIDs_path(std::string IDs){
    IDs_path = IDs;
}


void face_recognize::setFeatureList(std::vector<std::pair<std::vector<float>,int>> &List)
{
    featureList = List;
}
// 比较被检测人脸与数据库中的人脸的人脸距离
bool face_recognize::getCompare(std::vector<float>& face_feature,std::vector<std::pair<float,int>> &results){
    return runcompare(face_feature,results);
}

void face_recognize::resizeImage(cv::Mat &img){
    cv::resize(img, img, cv::Size(resize_w_h ,resize_w_h),cv::INTER_CUBIC);
    cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
}


//void face_recognize::getData(int num)
//{
//    d.openDB();
//    d.queryName(num);
//    name = d.name;
//    d.queryId(num);
//    ID = d.ID;
//}

// 计算欧氏距离
float face_recognize::o_distance(std::vector<float>& feature1, std::vector<float>& feature2){
    float sum = 0;
    float l = 0;
    for (int i = 0; i < feature_dim; i++){
        l = feature1[i] - feature2[i];
        sum += l * l;
    }
    return sqrt(sum);
}

// 得到输入人脸的人脸特征
int face_recognize::getFeature(cv::Mat &img, std::vector<float>& feature){
    runNet(img, feature);
    return 0;
}
// 得到数据库中所有人脸的人脸特征
int face_recognize::getFeatureList(){
    runFeatureList(faces_path);
    return 0;
}

int face_recognize::runFeatureList(cv::String &faces_path){

    //    args:   faces_path:   必须是包含人脸图片的文件夹的绝对路径
    //            featureList:  （人脸特征，对应人脸的序号）的vector
    std::vector<cv::String> imgs_path;
    cv::glob(faces_path, imgs_path);
    size_t count = imgs_path.size();
    facesNum = imgs_path.size();
    int j = 0;
    for(size_t i = 0; i < count; i++){
        cv::Mat img = cv::imread(imgs_path[i]);
        img = preprocess_img(img);
        std::vector<float> feature;
        getFeature(img, feature);
        j++;
        featureList.emplace_back(feature,j);
        feature.clear();
    }
}
// ncnn初始化
bool face_recognize::init(){
    ncnn::Option opt;
    opt.lightmode = true;
    opt.num_threads = num_thread;

    facenet.opt = opt;
    int ret = facenet.load_param(param_path.data());
    if (ret != 0) {
        return false;
    }
    ret = facenet.load_model(bin_path.data());
    if (ret != 0) {
        return false;
    }

    return true;
}

bool face_recognize::runcompare(std::vector<float>& face_feature,std::vector<std::pair<float,int>> &results){

    //    args:   featureList:          （人脸特征，对应人脸的序号）的vector
    //            face_feature:         resize后的被检测的人脸数据
    int size = featureList.size();
    for (int i = 0; i < size; i++){
        std::vector<float> feature;
        feature = featureList[i].first;
        float dist = o_distance(feature,face_feature);
        //        std::cout<<"dist:  "<<dist<<std::endl;
        if (dist < confidence){
            int ID = featureList[i].second;
            results.emplace_back(dist, ID);
        }
    }
    if(results.size()==0)
        return false;
    return true;

}

void face_recognize::getResult(std::vector<std::pair<float,int>> &results){
    runResult(results);
}

// 启动ncnn推理
int face_recognize::runNet(cv::Mat &img, std::vector<float>& feature){
    // 将图片转换为ncnn的格式
    ncnn::Mat in = ncnn::Mat::from_pixels(img.data, ncnn::Mat::PIXEL_BGR, img.cols, img.rows);
    // 预处理
    float mean_vals[3] = {0.f,0.f,0.f};
    float norm_vals[3] = {1/255.f,1/255.f,1/255.f};
    in.substract_mean_normalize(mean_vals, norm_vals);
    // 实例化Extractor
    ncnn::Extractor ex = facenet.create_extractor();
    // 可以设置线程个数，加快计算
    ex.set_num_threads(num_thread);
    //     ex.set_vulkan_compute(use_gpu);
    // 设置执行器是否使用轻量模式
    ex.set_light_mode(true);
    // 输入，将图片放入网络中,进行前向推理
    ex.input("input", in);
    // 提取倒数第二层输出
    ncnn::Mat out;
    ex.extract("259", out);

    // 归一化
    float l = 0;
    float l2 = 0;

    for (int i = 0; i < feature_dim; i++){
        l += out[i] * out[i];
    }
    l2 = sqrt(l);
    for (int i = 0; i < feature_dim; i++){
        feature.emplace_back(out[i] / l2);
    }
    return 0;
}




void face_recognize::runResult(std::vector<std::pair<float,int>> &results){
    std::sort(results.begin(), results.end());
    dist = results[0].first;
    num = results[0].second;
}

// 图片保真
cv::Mat face_recognize::preprocess_img(cv::Mat& img) {
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
