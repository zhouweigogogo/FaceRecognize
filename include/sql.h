#ifndef SQL_H
#define SQL_H

#include <QWidget>
#include <QDesktopWidget>
#include "sqlite3_db.h"
#include <QFont>
#include <QMessageBox>
namespace Ui {
class Sql;
}

class Sql : public QWidget
{
    Q_OBJECT

public:
    explicit Sql(QWidget *parent = 0);
    ~Sql();

private:
    Ui::Sql *ui;
    database d;
    QFont ft;

signals:
    void close_second();

private slots:
    void on_exit_clicked();
    void on_querySql_clicked();
    void on_updateSql_clicked();
    void on_testDB_clicked();
};

#endif // SQL_H
