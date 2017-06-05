#include "inventory.h"

inventory::inventory(QWidget *parent) : QTableWidget(parent)
{
    //setDragDropOverwriteMode(true);
    setAcceptDrops(true);
    //setDragDropMode(QAbstractItemView::DragDrop);
    //setSelectionBehavior(QAbstractItemView::SelectItems);
    //setSelectionMode(QAbstractItemView::SingleSelection);
    //setDragEnabled(true);
    //setDropIndicatorShown(true);
    setSize();
    mySound = new QSound(":/sounds/soundApple.wav");
}

cell inventory::setCell(int newIndex, QString newType, int newAmount)
{
    cell newCell;
    newCell.index = newIndex;
    newCell.type = newType;
    newCell.amount = newAmount;
    return newCell;
}

void inventory::setSize(){
    setSizeInventory(3);
    setRowCount(sizeInventory);
    setColumnCount(sizeInventory);
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

void inventory::clearTable()
{
    map.clear();
    clear();
    emit clearInventory(getSizeInventory());
}

void inventory::updateFromDataBase(QString typeObject, int count, int cellIndex, QString iconPath)
{
    if (map.contains(cellIndex)){
        map[cellIndex].amount = count;
        QTableWidgetItem *item = this->item(cellIndex / getSizeInventory(), cellIndex % getSizeInventory());
        item->setText(QString::number(map[cellIndex].amount));
        item->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
    }
    else{
        //QTableWidgetItem *oldItem = this->item(cellIndex / getSizeInventory(), cellIndex % getSizeInventory());
        cell newCell = setCell(cellIndex, typeObject, count);
        map[newCell.index] = newCell;
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setIcon(QIcon(QPixmap(iconPath)));
        item->setText(QString::number(map[cellIndex].amount));
        item->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
        setItem(cellIndex / getSizeInventory(), cellIndex % getSizeInventory(), item);
    }
}

void inventory::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        event->acceptProposedAction();
    }
    else {
        event->accept();
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
        QString myObjectType;
        int currentCellIndex;
        currentCellIndex = rowAt(event->pos().y()) * getSizeInventory() + columnAt(event->pos().x());

        dataStream >> pixmap >> amount >> myObjectType >> oldItemPos;

        setIconSize(QSize(75,80));
        setFont(QFont("Times", 6));
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setIcon(QIcon(QPixmap(pixmap)));
        if (itemAt(oldItemPos)){ // если перетаскиваем внутри таблицы
            itemAt(oldItemPos)->setSelected(false);
            if (itemAt(event->pos())){ //если складываем стопки
                int oldCellIndex;
                oldCellIndex = itemAt(oldItemPos)->row() * getSizeInventory() + itemAt(oldItemPos)->column();
                if (oldCellIndex == currentCellIndex)
                    return;
                if (map[oldCellIndex].type != map[currentCellIndex].type)
                    return;
                map[currentCellIndex].amount += map[oldCellIndex].amount;
                emit deleteCellInventory(map[oldCellIndex].index);
                map.remove(oldCellIndex);
                item->setText(QString::number(map[currentCellIndex].amount));
                delete itemAt(oldItemPos);
                emit updateCellInventory(map[currentCellIndex].amount, map[currentCellIndex].index);
            }
            else{ //если просто перемещаем на пустую ячейку
                int oldCellIndex;
                oldCellIndex = itemAt(oldItemPos)->row() * getSizeInventory() + itemAt(oldItemPos)->column();
                cell newCell = setCell(currentCellIndex, map[oldCellIndex].type, map[oldCellIndex].amount);
                map[newCell.index] = newCell;
                item->setText(QString::number(newCell.amount));
                emit deleteCellInventory(map[oldCellIndex].index);
                map.remove(oldCellIndex);
                delete itemAt(oldItemPos);
                emit addedNewInventory(map[currentCellIndex].type, map[currentCellIndex].amount, map[currentCellIndex].index, getSizeInventory());
            }
        }
        else {
            if (itemAt(event->pos())){ //если добавляем из исходного к существующему
                map[currentCellIndex].amount += 1;
                item->setText(QString::number(map[currentCellIndex].amount));
                emit updateCellInventory(map[currentCellIndex].amount, map[currentCellIndex].index);
            }
            else { //если добавляем новый
                cell newCell = setCell(rowAt(event->pos().y()) * getSizeInventory() + columnAt(event->pos().x()), myObjectType, amount);
                map[newCell.index] = newCell;
                item->setText(QString::number(newCell.amount));
                emit addedNewInventory(map[currentCellIndex].type, map[currentCellIndex].amount, map[currentCellIndex].index, getSizeInventory());
            }
        }
        item->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
        setItem(rowAt(event->pos().y()), columnAt(event->pos().x()), item);
        event->acceptProposedAction();
        event->accept();
    }
    else {
        event->accept();
    }
}

void inventory::mousePressEvent(QMouseEvent *event)
{
    if (itemAt(event->pos())) {
        dragStartPosition = event->pos();
        QTableWidgetItem *item = itemAt(event->pos());
        int currentCellIndex;
        currentCellIndex = item->row() * getSizeInventory() + item->column();
        item->setSelected(true);
        if (event->button() == Qt::LeftButton){
            QPixmap pixmap = item->icon().pixmap(QSize(100,100));
            int amount;
            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            QString myObjectType;
            myObjectType = map[currentCellIndex].type;
            amount = map[currentCellIndex].amount;
            dataStream << pixmap << amount << myObjectType << event->pos(); // передача иконки, кол-ва элементов в ячейке и позиция перетаскиваемой ячейки,
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
            mySound->play();
            if (map[currentCellIndex].amount > 1){
                item->setText(QString::number(map[currentCellIndex].amount - 1));
                map[currentCellIndex].amount -= 1;
            }
            else{
                map.remove(currentCellIndex);
                item->setSelected(false);
                delete item;
            }
        }
    }
}


/*void inventory::mouseMoveEvent(QMouseEvent *event)
{

}

void inventory::mouseReleaseEvent(QMouseEvent *event)
{

}*/

void inventory::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        event->acceptProposedAction();
        QAbstractItemView *d = reinterpret_cast<QAbstractItemView*>(this);
        QModelIndex index = indexAt(event->pos());
        highlightedRect = d->visualRect(index);

        viewport()->update();
    }
    else {
        event->accept();
    }
}

void inventory::paintEvent(QPaintEvent *event)
{
    QTableWidget::paintEvent(event);  //let it paint the default stuff!
    QPainter painter(this->viewport());  //you need to paint on the viewport
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::blue);
    painter.drawRect(highlightedRect);
}

