#ifndef QTSQL_H
#define QTSQL_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
class qtsql
{
private:

    QSqlDatabase db;
    QSqlQuery query;
public:
   void CreatTable();
   void InsterDate();
   void selectDate();
   void CloseDatebase();
   qtsql();
};

#endif // QTSQL_H
