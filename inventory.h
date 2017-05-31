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
public:
    inventory(QWidget *parent = 0);

private:
    cell setCell(int newIndex, QString newType, int newAmount);


    void setSizeInventory(int size){ sizeInventory = size; }
    int getSizeInventory() {return sizeInventory; }
    /*void setCellIndex(int newIndex);
    int getCellIndex();
    void setCellType(QString newType);
    QString getCellType();
    void setCellAmount(int newAmount);
    int getCellAmount();*/
    void setSize();
    int sizeInventory; // кол-во строк и столбцов
    //QList<cell> myListCells;
    QMap<int, cell> map;

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
};

#endif // INVENTORY_H
