#include "myobject.h"

int const amount = 1;

myObject::myObject(objectType type,QWidget *parent)
    : QFrame(parent)
{
    myObjectType = type;
    setMinimumSize(100, 100);
    setMaximumSize(100, 100);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);
    switch (myObjectType) {
        case Apple:
            iconPath =":/images/apple.jpg";
            objectTypeString = "Apple";
            break;
        case Pear:
            iconPath =":/images/pear.jpg";
            objectTypeString = "Pear";
            break;
    }
    objectIcon = new QLabel(this);
    objectIcon->setPixmap(QPixmap(iconPath));
    objectIcon->show();
    objectIcon->setAttribute(Qt::WA_DeleteOnClose);
}

myObject::~myObject()
{

}

void myObject::mouseMoveEvent(QMouseEvent *event)
{
     if (!(event->buttons() & Qt::LeftButton))
         return;
     if ((event->pos() - dragStartPosition).manhattanLength()
          < QApplication::startDragDistance())
         return;
}

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
        dataStream << pixmap << amount << objectTypeString << QPoint(2000,2000);

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
