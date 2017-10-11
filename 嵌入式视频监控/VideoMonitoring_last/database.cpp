#include "database.h"

/**
 * @brief DataBase::createConnection
 * @return
 *建立一个数据库连接
 */
bool DataBase::createConnection()
{
    //以后就可以用"sqlite1"与数据库进行连接了
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "sqlite1");
    db.setDatabaseName("database.db");
    if( !db.open())
    {
        qDebug() << "无法建立数据库连接";
        return false;
    }
    return true;
}

/**
 * @brief DataBase::createTable
 * @return
 *创建数据库表
 */
bool DataBase::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success = query.exec("create table automobil(id int primary key,photonum int,Videonum int)");
    if(success)
    {
        qDebug() << QObject::tr("数据库表创建成功！\n");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("数据库表创建失败！\n");
        return false;
    }
}

/**
 * @brief DataBase::insert
 * @param PhotoNum
 * @param VideoNum
 * @return
 * 向数据库中插入记录
 */
bool DataBase::insert(int PhotoNum,int VideoNum)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    db.open();
    QSqlQuery query(db);
    query.prepare("insert into automobil values(?, ?, ?)");
    //query.bindValue(0, i);
    query.bindValue(1, PhotoNum);
    query.bindValue(2, VideoNum);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("插入失败"));
        return false;
    }
    db.close();
    return true;
}

/**
 * @brief DataBase::queryAll
 * @return
 * 查询所有信息
 */
bool DataBase::queryAll()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.exec("select * from automobil");
    QSqlRecord rec = query.record();
    //qDebug() << QObject::tr("automobil表字段数：" ) << rec.count();
    while(query.next())
    {
        qDebug()<<query.value(0)<<query.value(1)<<query.value(2);
    }
    return true;

}

/**
 * @brief DataBase::deleteById
 * @param id
 * @return
 * 根据ID删除记录
 */
bool DataBase::deleteById(int id)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("delete from automobil where id = %1").arg(id));
    if(!query.exec())
    {
        qDebug() << "删除记录失败！";
        return false;
    }
    return true;
}

/**
 * @brief DataBase::updateById
 * @param id
 * @return
 * 根据ID更新记录
 */
bool DataBase::updateById(int id)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("update automobil set PhotoNum=?,VideoNum=? where id=%1").arg(id));

    query.bindValue(1,1);
    query.bindValue(2,1);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("更新失败"));
    }
    return true;
}

/**
 * @brief DataBase::sortById
 * @return
 *排序
 */
bool DataBase::sortById()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success=query.exec("select * from automobil order by id desc");
    if(success)
    {
        qDebug() << QObject::tr("排序成功");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("排序失败！");
        return false;
    }
}
