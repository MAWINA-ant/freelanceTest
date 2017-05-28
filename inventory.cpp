#include "inventory.h"

inventory::inventory(QWidget *parent) : QTableWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropOverwriteMode(true);
    //setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::CopyAction);
    setRowCount(3);
    setColumnCount(3);
    setSize();
}

void inventory::setSize(){
    setMaximumHeight(302);
    setMinimumHeight(302);
    setMaximumWidth(302);
    setMinimumWidth(302);
    for (int i=0; i<3; i++){
        setRowHeight(i,100);
        setColumnWidth(i,100);
    }
    verticalHeader()->hide();
    horizontalHeader()->hide();
}

void inventory::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        event->acceptProposedAction();
    }
    else {
        event->ignore();
    }
}

void inventory::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        QLabel *newIcon = new QLabel(this);
        newIcon->setPixmap(pixmap);
        newIcon->move(event->pos() - offset);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);

        event->acceptProposedAction();
        event->accept();
        dropMimeData(1,1,event->mimeData(),Qt::CopyAction);
    }
    else {
        event->ignore();
    }
}

bool inventory::dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action)
{

}

void inventory::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        event->acceptProposedAction();
    }
    else {
        event->ignore();
    }
}
