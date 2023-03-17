#include "firstwindow.h"
#include <QApplication>
#include <QTime>
#include <QMetaType>
//#include <QTextCodec>
using namespace std;

int main(int argc, char *argv[])
{
    qRegisterMetaType< face_recognize >("face_recognize&");

//    QTextCodec *codec=QTextCodec::codecForName("utf-8");
//    QTextCodec::setCodecForLocale(codec);
//    qRegisterMetaType< std::vector<std::pair<std::vector<float>, int>>>("std::vector<std::pair<std::vector<float>, int>> &");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
