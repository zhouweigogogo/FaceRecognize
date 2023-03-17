#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <QDialog>
#include <string>
#include <QString>
#include "thirdwindow.h"
using namespace std;
namespace Ui {
class Administrator;
}

class Administrator : public QDialog
{
    Q_OBJECT

public:
    explicit Administrator(QWidget *parent = 0);
    ~Administrator();
    void keyPressEvent(QKeyEvent *event);

public slots:

signals:
    void sendID(QString&);

private slots:
    void on_btnConfirm_clicked();
    void on_btnExit_clicked();

private:
    Ui::Administrator *ui;
};

#endif // ADMINISTRATOR_H
