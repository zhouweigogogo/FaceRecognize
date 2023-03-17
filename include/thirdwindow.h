#ifndef THIRDWINDOW_H
#define THIRDWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include "save_csv.h"
namespace Ui {
class ThirdWindow;
}

class ThirdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ThirdWindow(QWidget *parent = 0);
    ~ThirdWindow();
    string passed_time;
    int passed_ID;
    QString finded_ID;
    void write_csv();


public slots:
    void receiveTime(string& time);
    void receiveID(int& ID);
    void receiveFindID(QString& findedID);
    void writetime();
    void write_flag();

signals:
    void start_flag();

private:
    Ui::ThirdWindow *ui;
    int rows;
    int cols;
    QStringList IDList;
    int ID_place;




};

#endif // THIRDWINDOW_H
