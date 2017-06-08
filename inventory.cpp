#include "inventory.h"

inventory::inventory(QWidget *parent) : QTableWidget(parent)
{
    setAcceptDrops(true);
    setSettings();
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

void inventory::setSettings(){
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
    setIconSize(QSize(75,80));
    setFont(QFont("Times", 6));
}

void inventory::clearTable()
{
    map.clear();
    clear();
    emit clearAllInventory(getSizeInventory());
}

void inventory::addFromSocket(QString type, int count, int cellIndex, int size, QIcon icon)
{
    cell newCell = setCell(cellIndex, type, count);
    map[newCell.index] = newCell;
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setIcon(icon);
    item->setText(QString::number(map[cellIndex].amount));
    item->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
    setItem(cellIndex / getSizeInventory(), cellIndex % getSizeInventory(), item);
    emit addedNewInventory(type, count, cellIndex, size);
}

void inventory::changeFromSocket(int count, int cellIndex)
{
    map[cellIndex].amount = count;
    QTableWidgetItem *item = this->item(cellIndex / getSizeInventory(), cellIndex % getSizeInventory());
    item->setText(QString::number(map[cellIndex].amount));
    item->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
    emit updateCellInventory(count, cellIndex);
}

void inventory::removeFromSocket(int cellIndex)
{
    map.remove(cellIndex);
    QTableWidgetItem *item = this->item(cellIndex / getSizeInventory(), cellIndex % getSizeInventory());
    delete item;
    emit deleteCellInventory(cellIndex);
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
                emit deleteToSocket(map[oldCellIndex].index);
                map.remove(oldCellIndex);
                item->setText(QString::number(map[currentCellIndex].amount));
                delete itemAt(oldItemPos);
                emit updateCellInventory(map[currentCellIndex].amount, map[currentCellIndex].index);
                emit updateToSocket(map[currentCellIndex].amount, map[currentCellIndex].index);
            }
            else{ //если просто перемещаем на пустую ячейку
                int oldCellIndex;
                oldCellIndex = itemAt(oldItemPos)->row() * getSizeInventory() + itemAt(oldItemPos)->column();
                cell newCell = setCell(currentCellIndex, map[oldCellIndex].type, map[oldCellIndex].amount);
                map[newCell.index] = newCell;
                item->setText(QString::number(newCell.amount));
                emit deleteCellInventory(map[oldCellIndex].index);
                emit deleteToSocket(map[oldCellIndex].index);
                map.remove(oldCellIndex);
                delete itemAt(oldItemPos);
                emit addedNewInventory(map[currentCellIndex].type, map[currentCellIndex].amount, map[currentCellIndex].index, getSizeInventory());
                emit addedToSocket(map[currentCellIndex].type, map[currentCellIndex].amount, map[currentCellIndex].index, getSizeInventory(), item->icon());
            }
        }
        else {
            if (itemAt(event->pos())){ //если добавляем из исходного к существующему
                map[currentCellIndex].amount += 1;
                item->setText(QString::number(map[currentCellIndex].amount));
                emit updateToSocket(map[currentCellIndex].amount, map[currentCellIndex].index);
                emit updateCellInventory(map[currentCellIndex].amount, map[currentCellIndex].index);
            }
            else { //если добавляем новый
                cell newCell = setCell(rowAt(event->pos().y()) * getSizeInventory() + columnAt(event->pos().x()), myObjectType, amount);
                map[newCell.index] = newCell;
                item->setText(QString::number(newCell.amount));
                emit addedNewInventory(map[currentCellIndex].type, map[currentCellIndex].amount, map[currentCellIndex].index, getSizeInventory());
                emit addedToSocket(map[currentCellIndex].type, map[currentCellIndex].amount, map[currentCellIndex].index, getSizeInventory(), item->icon());
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
    //**************************************************
    // создаю свой тип mimeData и начинаю перетаскивание,
    // если нажатие левой кнопкой мыши
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
        //***********************************************************************
        // по нажатию правой кнопкой происходит удаление или уменьшение
        // текущего кол-ва элементов со звуком
        else if (event->button() == Qt::RightButton){
            mySound->play();
            if (map[currentCellIndex].amount > 1){
                item->setText(QString::number(map[currentCellIndex].amount - 1));
                map[currentCellIndex].amount -= 1;
                emit updateToSocket(map[currentCellIndex].amount, map[currentCellIndex].index);
                emit updateCellInventory(map[currentCellIndex].amount, map[currentCellIndex].index);
            }
            else{
                emit deleteCellInventory(map[currentCellIndex].index);
                emit deleteToSocket(map[currentCellIndex].index);
                map.remove(currentCellIndex);
                item->setSelected(false);
                delete item;
            }
        }
    }
}

void inventory::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        event->acceptProposedAction();
        QAbstractItemView *d = reinterpret_cast<QAbstractItemView*>(this);
        QModelIndex index = indexAt(event->pos());
        highlightedRect = d->visualRect(index); // получаю текущий квадрат

        viewport()->update();
    }
    else {
        event->accept();
    }
}

void inventory::paintEvent(QPaintEvent *event)
{
    QTableWidget::paintEvent(event);  // перерисовка QTableWidget
    QPainter painter(this->viewport());  // перерисовка на viewport();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::blue);
    painter.drawRect(highlightedRect);
}

