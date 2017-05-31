#ifndef INVENTORY_H
#define INVENTORY_H

#include <QtWidgets>
#include <QPair>
#include <QSound>
#include "myobject.h"

struct cell     //Создаем структуру!
{
    int index;       //индекс ячейки
    myObject::objectType type;        //тип объекта
    int amount;   //количество объектов
};

class inventory : public QTableWidget
{
public:
    inventory(QWidget *parent = 0);

private:
    void setSize();
    int getCurrentAmount() {return 0;}

    int countColmnAndRow;
    QList<cell> MyListColor;

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
};

#endif // INVENTORY_H
