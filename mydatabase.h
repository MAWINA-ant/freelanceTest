#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>

class mydatabase : public QObject
{
    Q_OBJECT

public:
    explicit mydatabase(QObject *parent = 0);


private:
    void createDatabase();
    bool createConnection();
    QSqlDatabase db;

signals:

public slots:
    void insertDataInventory(QString, int, int, int);
    void updateDataInventory(int, int);
    void removeDataInventory(int);
    void insertDataObject(QString, QString);
    void removeDataObject(QString);
    void clearDataBase(int);
};

#endif // MYDATABASE_H
