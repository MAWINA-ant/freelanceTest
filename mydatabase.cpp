#include "mydatabase.h"
#include <QDebug>

mydatabase::mydatabase(QObject *parent) : QObject(parent)
{
    createDatabase();
}

void mydatabase::createDatabase()
{
    if (!createConnection()) {
        return;
    }
    QSqlQuery query;
    QString table1 = "CREATE TABLE inventory("
                  "typeObject TEXT, "
                  "count INTEGER, "
                  "cellIndex INTEGER, "
                  "size INTEGER)";
    if (!query.exec(table1)) {
        qDebug() << "Unable to create a table inventory";
    }
    QString table2 = "CREATE TABLE object("
                   "typeObject TEXT, "
                   "iconPath TEXT)";
    if (!query.exec(table2)) {
        qDebug() << "Unable to create a table object";
    }
}

void mydatabase::insertDataInventory(QString strNewType, int newCount, int newCellIndex, int newSize)
{
    QSqlQuery query;
    query.prepare("INSERT INTO inventory (typeObject, count, cellIndex, size)"
                                  "VALUES (:typeObject, :count, :cellIndex, :size);");
    query.bindValue(":typeObject", strNewType);
    query.bindValue(":count", QString::number(newCount));
    query.bindValue(":cellIndex", QString::number(newCellIndex));
    query.bindValue(":size", QString::number(newSize));
        if (!query.exec()) {
            qDebug() << "Unable to insert data in table inventory";
        }
}

void mydatabase::updateDataInventory(int newCount, int CellIndex)
{
    QSqlQuery query;
    query.prepare("UPDATE inventory SET count = :count WHERE cellIndex = :cellIndex");
    query.bindValue(":count", QString::number(newCount));
    query.bindValue(":cellIndex", QString::number(CellIndex));
        if (!query.exec()) {
            qDebug() << "Unable to update data in table inventory";
        }
}

void mydatabase::removeDataInventory(int CellIndex)
{
    QSqlQuery query;
    query.prepare("DELETE FROM inventory  WHERE cellIndex = :cellIndex");
    query.bindValue(":cellIndex", QString::number(CellIndex));
        if (!query.exec()) {
            qDebug() << "Unable to remove data in table inventory";
        }
}

void mydatabase::insertDataObject(QString typeObject, QString typeObjectPath)
{
    QSqlQuery query;
    query.prepare("INSERT INTO object (typeObject, iconPath)"
                                  "VALUES (:typeObject, :iconPath);");
    query.bindValue(":typeObject", typeObject);
    query.bindValue(":iconPath", typeObjectPath);
        if (!query.exec()) {
            qDebug() << "Unable to insert data in table object";
        }
}

void mydatabase::removeDataObject(QString typeObject)
{
    QSqlQuery query;
    query.prepare("DELETE FROM object  WHERE typeObject = :typeObject");
    query.bindValue(":typeObject", typeObject);
        if (!query.exec()) {
            qDebug() << "Unable to remove data in table object";
        }
}

void mydatabase::clearDataBase(int size)
{
    QSqlQuery query;
    query.prepare("DELETE FROM inventory  WHERE size = :size");
    query.bindValue(":size", QString::number(size));
        if (!query.exec()) {
            qDebug() << "Unable to remove data in table inventory";
        }
}

bool mydatabase::createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("myDB");
    db.setUserName ("user") ;
    db.setHostName ("localhost") ;
    db.setPassword ( "password") ;
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
        return false;
    }
    return true;
}

