#include "inventory.h"

inventory::inventory(QWidget *parent) : QTableWidget(parent)
{
    setAcceptDrops(true);
    countColmnAndRow = 3;
    //setDragEnabled(true);
    //setDragDropOverwriteMode(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    //setDefaultDropAction(Qt::CopyAction);
    setRowCount(countColmnAndRow);
    setColumnCount(countColmnAndRow);
    setSize();
    //QModelIndex index = QTableWidget::model();
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
        int amount;
        QPoint oldItemPos;

        dataStream >> pixmap >> amount >> oldItemPos;

        setIconSize(QSize(75,80));
        setFont(QFont("Times", 6));
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setIcon(QIcon(QPixmap(pixmap)));
        if (itemAt(event->pos()))
            item->setText(QString::number(itemAt(event->pos())->text().toInt() + amount));
        else
            item->setText(QString::number(amount));
        item->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
        setItem(rowAt(event->pos().y()), columnAt(event->pos().x()), item);
        event->acceptProposedAction();
        event->accept();
        delete itemAt(oldItemPos);
    }
    else {
        event->ignore();
    }
}

void inventory::mousePressEvent(QMouseEvent *event)
{
    if (itemAt(event->pos())) {
        QTableWidgetItem *item = itemAt(event->pos());
        if (event->button() == Qt::LeftButton){
            QPixmap pixmap = item->icon().pixmap(QSize(100,100));
            int amount;
            amount = item->text().toInt();
            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream << pixmap << amount << event->pos(); // передача иконки, кол-ва элементов в ячейке и позиция перетаскиваемой ячейки,
                                                            // чтобы после её удалить
            QMimeData *mimeData = new QMimeData;
            mimeData->setData("application/x-dnditemdata", itemData);
            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(event->pos() - QPoint(item->column()*100, item->row()*100));
            drag->exec(Qt::CopyAction);
        }
        else if (event->button() == Qt::RightButton){
            QSound::play(":/sounds/soundApple.wav");
            if (item->text().toInt() > 1)
                item->setText(QString::number(item->text().toInt() - 1));
            else
                delete item;
        }
    }

        /*QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
        if (!child)
            return;

        QPixmap pixmap = *child->pixmap();

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << pixmap << amount;

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
