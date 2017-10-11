#include "qtsql.h"

qtsql::qtsql()
{
    db =QSqlDatabase::addDatabase("QSTSQL");
    db.setHostName("yhy");
    db.setDatabaseName("MyDB3.db");
    db.setUserName("yhy");
    db.setPassword("123456");
    //打开数据库连接
    db.open();
    query=QSqlQuery(db);

}
void qtsql::CreatTable()
{
    //创建表格

    query.exec("create table Qt_Table(qt_id int primary key,qt_picture integer,qt_videos integer)");
}
void qtsql::InsterDate(int photoNum,int VideoNum )
{
    QString insert_sql = "insert into member values(?,?,?)";
    query.bindValue();
    query.bindValue();
    int success = query.exec();
    if(success) qDebug()<<"插入成功";
    else qDebug()<<"插入失败";
}
void qtsql::selectDate()
{
    query.exec("select * from Qt_Table");
    while(query.next())
    {
        qDebug()<<query.value(0)<<query.value(1)<<query.value(2)<<query.value(3);
    }

}
void qtsql::CloseDatebase()
{
    db->close();
}
