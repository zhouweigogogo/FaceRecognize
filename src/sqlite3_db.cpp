#include "sqlite3_db.h"
#include <iostream>
#include <fstream>
#include <string>
database::database()
{
}

database::~database()
{
    std::cout << "关闭数据库" <<std::endl;
    sqlite3_close(db);

}

void database::openDB()
{
    rc = sqlite3_open(DBName, &db);
    if(rc == 0){
        std::cout << "数据库打开成功" <<std::endl;
    }
    else{
        std::cout << "数据库打开失败" <<std::endl;
    }
}
// 获取行数
int database::getRow()
{
    char **dbresult;
    int nrow,ncolumn;
    QString sql = QString("SELECT * from %1;").arg(TABLEName);
    int res = sqlite3_get_table(db,sql.toStdString().data(),&dbresult,&nrow,&ncolumn,&zErrMsg);
    if(res == SQLITE_OK)
    {
        //查询成功
        return nrow;
    }
    sqlite3_free_table(dbresult);
    return 0;
}

void database::new_table()
{
    QString sql = QString("create table %1 (NUM INTEGER PRAMAEY KEY,NAME varchar,ID varchar);").arg(TABLEName);
    std::cout<<sql.toStdString()<<std::endl;
    int result = sqlite3_exec(db, sql.toStdString().data(), NULL, NULL, NULL);
    if (result == SQLITE_OK) {
        std::clog<< "创建数据成功\n";
    }
    else {
        std::cout<<"创建数据失败"<<std::endl;
    }
}

void database::insertDB()
{
    int row = getRow();
    row++;
    QString sql = QString("insert into %1 (NUM,NAME,ID) values (%2,'%3','%4');").arg(TABLEName,QString::number(row,10), name, ID);
    std::cout<<sql.toStdString()<<std::endl;
    sqlite3_stmt *stmt = NULL;        //stmt语句句柄
    int result = sqlite3_prepare_v2(db, sql.toStdString().data(), -1, &stmt, NULL);

    if (result == SQLITE_OK) {
        std::clog<< "添加数据语句done\n";
        sqlite3_step(stmt);
    }
    else {
        std::clog << "添加数据语句wrong\n";
    }
    sqlite3_finalize(stmt);
}
// 获取所有数据
void database::getDB()
{

    QString sql = QString("SELECT * FROM %1;").arg(TABLEName);
    sqlite3_stmt *stmt = NULL;    // stmt语句句柄
    std::ofstream outfile("data.txt", std::ios::trunc);

    int result = sqlite3_prepare_v2(db,sql.toStdString().data() , -1, &stmt, NULL);

    if (result == SQLITE_OK) {
        std::clog <<  "查询语句done\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int Num = sqlite3_column_int(stmt, 0);
            QString Name= sqlite3_column_text(stmt, 1);
            QString id= sqlite3_column_text(stmt, 2);
            //输出相关查询的数据
            outfile<<Num<<"  "<<Name.toStdString().data()<<"  "<<id.toStdString().data()<<std::endl;
            std::clog<<"序号:"<<Num;
            std::clog<<"姓名:"<< Name.toStdString().data();
            std::clog<<"学号:"<<id.toStdString().data();
            printf("\n");
        }
    }
    else {
        std::clog << "查询语句wrong";
    }
    sqlite3_finalize(stmt);
    outfile.close();

}

// 根据名字索引学号
bool database::queryDB()
{
    QString sql = QString("select ID from %1 where NAME = '%2';").arg(TABLEName, name);
    sqlite3_stmt *stmt = NULL;    // stmt语句句柄
    std::cout<<sql.toStdString()<<std::endl;
    int result = sqlite3_prepare_v2(db,sql.toStdString().data() , -1, &stmt, NULL);

    if (result == SQLITE_OK) {
        std::clog <<  "查询语句done\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            ID = QString::fromLocal8Bit(sqlite3_column_text(stmt, 0));
            //输出相关查询的数据
            std::clog<<"学号:"<<ID.toStdString().data();
            return true;
        }
    }
    else {
        std::clog << "查询语句wrong";
        return false;
    }
}

void database::queryName(int n)
{
    std::cout<<"n: "<<n<<std::endl;
    QString sql = QString("select NAME from %1 where NUM = '%2';").arg(TABLEName, QString::number(n));
    sqlite3_stmt *stmt = NULL;    // stmt语句句柄
    std::cout<<sql.toStdString()<<std::endl;
    int result = sqlite3_prepare_v2(db,sql.toStdString().data() , -1, &stmt, NULL);

    if (result == SQLITE_OK) {
        std::clog <<  "查询语句done\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            name = QString::fromLocal8Bit(sqlite3_column_text(stmt, 0));
            //输出相关查询的数据
            std::clog<<"姓名:"<<name.toStdString().data();
        }
    }
    else {
        std::clog << "查询语句wrong";
    }
}

void database::queryId(int n)
{
    std::cout<<"n: "<<n<<std::endl;
    QString sql = QString("select ID from %1 where NUM = %2;").arg(TABLEName, QString::number(n));
    sqlite3_stmt *stmt = NULL;    // stmt语句句柄
    std::cout<<sql.toStdString()<<std::endl;
    int result = sqlite3_prepare_v2(db,sql.toStdString().data() , -1, &stmt, NULL);

    if (result == SQLITE_OK) {
        std::clog <<  "查询语句done\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            ID = QString::fromLocal8Bit(sqlite3_column_text(stmt, 0));
            //输出相关查询的数据
            std::clog<<"姓名:"<<ID.toStdString().data();
        }
    }
    else {
        std::clog << "查询语句wrong";
    }
}

bool database::deleteDB()
{
    QString sql = QString("delete from %1 where NAME='%2';").arg(TABLEName,name);
    sqlite3_stmt *stmt = NULL;        //stmt语句句柄
    std::cout<<sql.toStdString()<<std::endl;
    int result = sqlite3_prepare_v2(db, sql.toStdString().data(), -1, &stmt, NULL);

    if (result == SQLITE_OK) {
        std::clog<< "删除数据语句done";
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return true;
    }
    else {
        std::clog << "删除数据语句wrong";
        sqlite3_finalize(stmt);
        return false;
    }
}

bool database::updateDB()
{
    QString sql = QString("update %1 set ID = '%2' where NAME = '%3';").arg(TABLEName,ID, name);
    std::cout<<sql.toStdString()<<std::endl;
    sqlite3_stmt *stmt = NULL;        //stmt语句句柄
    int result = sqlite3_prepare_v2(db, sql.toStdString().data(), -1, &stmt, NULL);

    if (result == SQLITE_OK) {
        std::clog<< "添加数据语句done\n";
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return true;
    }
    else {
        std::clog << "添加数据语句wrong\n";
        sqlite3_finalize(stmt);
        return false;
    }
}

void database::closeDB()
{
    if (db) {
        sqlite3_close_v2(db);
        db = nullptr;
    }
}


