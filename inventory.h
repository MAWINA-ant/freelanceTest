#ifndef INVENTORY_H
#define INVENTORY_H

#include <QtWidgets>

class inventory : public QTableWidget
{
public:
    inventory(QWidget *parent = 0);

private:
    void setSize();


    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
};

#endif // INVENTORY_H
