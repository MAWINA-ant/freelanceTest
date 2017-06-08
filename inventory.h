#ifndef INVENTORY_H
#define INVENTORY_H

#include <QtWidgets>
#include <QPair>
#include <QSound>
#include "myobject.h"

struct cell     //Создаем структуру!
{
    int index;       //индекс ячейки
    QString type;        //тип объекта
    int amount;   //количество объектов
};

class inventory : public QTableWidget
{
    Q_OBJECT
public:
    inventory(QWidget *parent = 0);

private:
    cell setCell(int newIndex, QString newType, int newAmount); // создает новую структуру
    void setSizeInventory(int size){ sizeInventory = size; }
    int getSizeInventory() {return sizeInventory; }
    void setSettings(); // установка начальных настроек

    QMap<int, cell> map; // контейнер для структур (ячейки таблицы)
    QSound *mySound;
    int sizeInventory;
    QPoint dragStartPosition;
    QRect highlightedRect;


signals:
    //**********************************************
    // сигналы для таблицы Inventory в БД
    void addedNewInventory(QString, int, int, int);
    void updateCellInventory(int, int);
    void deleteCellInventory(int);
    void clearAllInventory(int);
    //***********************************************

    //************************************************
    // сигналы при соединении (для Sockets)
    void addedToSocket(QString, int, int, int, QIcon);
    void updateToSocket(int, int);
    void deleteToSocket(int);
    //************************************************

public slots:
    void clearTable(); // очищает всю таблицу

    //******************************************************
    // слоты для отображения изменения объектов другой роли.
    // произошло действие в сервере - оно отобразилось в клиенте,
    // и наоборот.
    void addFromSocket(QString, int, int , int, QIcon);
    void changeFromSocket(int, int);
    void removeFromSocket(int);
    //********************************************************

protected:
    void dragEnterEvent(QDragEnterEvent *event); // определение доступных для drug'n'drop объектов
                                                 // при наведении на таблицу
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);   // выделяется ячейка на таблице для drop
    void mousePressEvent(QMouseEvent *event);    // начало перетаскивания внутри таблицы
    void paintEvent(QPaintEvent *event);         // отрисовка квадрата ячейки
};

#endif // INVENTORY_H
