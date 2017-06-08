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
    void createDatabase(); // создает в БД таблицы, если соединение с базой установилось
    bool createConnection(); // создание БД и установка соединения с ней
    QSqlDatabase db;

signals:
    void sendData(QString, int, int ,QString);

private slots:
    void insertDataInventory(QString, int, int, int); //добавление строки в таблице Inventory
    void updateDataInventory(int, int); // обновление данных по номеру cellIndex (изменение кол-ва)
    void removeDataInventory(int); //удалить строку по cellIdex
    void insertDataObject(QString, QString); //добавить строку в таблицу object
    void removeDataObject(QString); //удалить строку в таблице object по наименованию типа
    void clearDataBase(int); //очищает таблицу Inventory от всех данных
    void receivedData();
};

#endif // MYDATABASE_H
