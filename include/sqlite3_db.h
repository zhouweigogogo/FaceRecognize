#ifndef SQLITE3_DB_H
#define SQLITE3_DB_H

#include "sqlite3.h"
#include <iostream>
#include <QString>
#define DBName "./database.db"
#define TABLEName "stu_info"

class database
{
public:
    database();
    ~database();
    QString name;
    QString ID;
    int num;
    int nrow;
    void openDB();
    int getRow();
    void new_table();
    void insertDB();
    void getDB();
    bool queryDB();
    void queryName(int n);
    void queryId(int n);
    bool deleteDB();
    bool updateDB();
    void closeDB();


private:
    sqlite3 *db;
    int rc;
    char *zErrMsg = 0;
    char **dbresult;


};

#endif // SQLITE3_DB_H
