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
    cell setCell(int newIndex, QString newType, int newAmount);
    void setSizeInventory(int size){ sizeInventory = size; }
    int getSizeInventory() {return sizeInventory; }
    void setSize();

    QMap<int, cell> map;
    QSound *mySound;
    int sizeInventory;
    QPoint dragStartPosition;
    QRect highlightedRect;


signals:
    void addedNewInventory(QString, int, int, int);
    void updateCellInventory(int, int);
    void deleteCellInventory(int);
    void clearInventory(int);

private slots:
    void clearTable();
    void updateFromDataBase(QString typeObject, int count, int cellIndex, QString iconPath); // обновление таблицы из БД

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // INVENTORY_H
