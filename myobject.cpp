#include "myobject.h"

int const amount = 1;

myObject::myObject(QWidget *parent)
    : QFrame(parent)
{
    setMinimumSize(100, 100);
    setMaximumSize(100, 100);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);
    setType(Apple);
    switch (myObjectType) {
        case Apple:
            iconPath =":/images/apple.jpg";
            break;
        case Pear:
            iconPath =":/images/pear.jpg";
            break;
    }
    objectIcon = new QLabel(this);
    objectIcon->setPixmap(QPixmap(iconPath));
    objectIcon->show();
    objectIcon->setAttribute(Qt::WA_DeleteOnClose);
}
/*
void myObject::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton
         && geometry().contains(event->pos())) {

        dragStartPosition = event->pos();
        QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
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
        //drag->setPixmap(iconPixmap);
        Qt::DropAction dropAction = drag->exec();
    }
}
*/
void myObject::mouseMoveEvent(QMouseEvent *event)
 {
     if (!(event->buttons() & Qt::LeftButton))
         return;
     if ((event->pos() - dragStartPosition).manhattanLength()
          < QApplication::startDragDistance())
         return;

     //QDrag *drag = new QDrag(this);
     //QMimeData *mimeData = new QMimeData;

     //QByteArray itemData;
     //QDataStream dataStream(&itemData, QIODevice::WriteOnly);
     //dataStream << pixmap << QPoint(event->pos() - child->pos());

     //mimeData->setData("application/x-dnditemdata",);
     //drag->setMimeData(mimeData);

     //Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

 }

/*void myObject::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void myObject::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void myObject::dropEvent(QDropEvent *event)
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

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}
*/
void myObject::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){

        dragStartPosition = event->pos();
        QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
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
        child->setPixmap(pixmap);
    }
}
