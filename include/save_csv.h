#ifndef SAVE_CSV_H
#define SAVE_CSV_H

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <iostream>
#include <QDebug>
//#include <QStringLiteral>
using namespace std;

class save_csv
{

public:
    save_csv();
    QStringList read_csv();
    void test_csv();
    //    void write_csv(stu_info s);
};

#endif // SAVE_CSV_H
