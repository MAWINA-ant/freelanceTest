#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QFrame>
#include <QtWidgets>

class myObject : public QFrame
{
    Q_OBJECT

public:
    enum objectType {  Apple , Pear }; // перечисление типов
    myObject(objectType type, QWidget *parent = 0);
    QString getObjectType(){return objectTypeString;}
    QString geticonPath(){return iconPath;}
    ~myObject();

private:
    void setType(objectType type) {myObjectType = type;}
    QLabel *objectIcon;
    QString iconPath; // путь к иконке
    QString objectTypeString; // тип строкой
    objectType myObjectType; // текущий тип
    QPoint dragStartPosition;

signals:
    //void addedNewObject(QString, QString);
    //void removeObject(QString);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event); // переопределяю метод для drug'n'drop и начинаю перетаскивание
    void mouseMoveEvent(QMouseEvent *event); // в методе определяется началось ли движение объекта
};

#endif // MYOBJECT_H
