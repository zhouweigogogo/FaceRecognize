#include <QDialog>
#include <QDesktopWidget>
#include <QMovie>

namespace Ui {
class LoadingWidget;
}

class LoadingWidget : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingWidget(QWidget *parent = 0);
    ~LoadingWidget();

private:
    Ui::LoadingWidget *ui;
};
