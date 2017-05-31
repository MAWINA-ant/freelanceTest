#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QFrame>
#include <QtWidgets>

class myObject : public QFrame
{
public:
    enum objectType {  Apple , Pear }; // перечисление типов
    myObject(objectType type, QWidget *parent = 0);

private:
    void setType(objectType type) {myObjectType = type;}
    QLabel *objectIcon;
    QString iconPath; // путь к иконке
    QString objectTypeString; // тип строкой
    objectType myObjectType; // текущий тип
    QPoint dragStartPosition;


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
};

#endif // MYOBJECT_H
