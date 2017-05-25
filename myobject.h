#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QFrame>
#include <QtWidgets>

class myObject : public QFrame
{
public:
    enum objectType { Pear , Apple };
    myObject(QWidget *parent = 0);

private:
    void setType(objectType type) {myObjectType = type;}
    QLabel *objectIcon;
    QString iconPath;
    objectType myObjectType;

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // MYOBJECT_H
