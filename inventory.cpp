#include "inventory.h"

inventory::inventory(QWidget *parent) : QTableWidget(parent)
{
    setAcceptDrops(true);
    //setDragEnabled(true);
    //setDragDropOverwriteMode(true);
    //setDragDropMode(QAbstractItemView::DragDrop);
    //setDefaultDropAction(Qt::CopyAction);
    setRowCount(3);
    setColumnCount(3);
    setSize();
    /*for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            QTableWidgetItem *item = new QTableWidgetItem();
            setItem(i, j, item);
        }*/
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

        QTableWidgetItem *item = new QTableWidgetItem();

        QPixmap newPixmap = pixmap.scaled(70,70,Qt::KeepAspectRatio);
        item->setText("as");
        item->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
        item->setData( Qt::DecorationRole, newPixmap );
        setItem(rowAt(event->pos().y()), columnAt(event->pos().x()), item);
        event->acceptProposedAction();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void inventory::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){

        /*QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
        if (!child)
            return;

        QPixmap pixmap = *child->pixmap();

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << pixmap << QPoint(event->pos() - child->pos());

        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/x-dnditemdata", itemData);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(event->pos() - child->pos());

        QPixmap tempPixmap = pixmap;
        QPainter painter;
        painter.begin(&tempPixmap);
        painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
        painter.end();

        child->setPixmap(tempPixmap);
        drag->exec(Qt::CopyAction);
        child->show();
        child->setPixmap(pixmap);*/
    }
    else if (event->button() == Qt::RightButton){
        if (itemAt(event->pos())){
            delete itemAt(event->pos());
        }
    }
}

/*void inventory::mouseMoveEvent(QMouseEvent *event)
{
    //QTableWidgetItem *item = itemAt(event->pos());
    //item->setSelected(true);
}*/


void inventory::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        event->acceptProposedAction();
    }
    else {
        event->ignore();
    }
}
