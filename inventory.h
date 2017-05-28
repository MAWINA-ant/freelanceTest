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
    bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action);
    void dragMoveEvent(QDragMoveEvent *event);
};

#endif // INVENTORY_H
