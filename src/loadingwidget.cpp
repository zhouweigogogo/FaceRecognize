#include "loadingwidget.h"
#include "ui_loadingwidget.h"
#include <QTime>

LoadingWidget::LoadingWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);//无边框

    //屏幕居中显示
    int frmX = width();
    int frmY = height();

    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();

    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    move(movePoint);

    //加载gif图片
    QMovie *movie = new QMovie(":/new/prefix1/load");
    ui->lbl_gif->setMovie(movie);
    movie->start();
    ui->lbl_gif->show();
}



LoadingWidget::~LoadingWidget()
{
    delete ui;
}
