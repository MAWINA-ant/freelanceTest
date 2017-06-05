#ifndef DIALOG_H
#define DIALOG_H

#include "mainmenu.h"
#include "myobject.h"
#include "inventory.h"
#include "mydatabase.h"

#include <QDialog>
#include <QtWidgets>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    void createMenu();

    QPushButton *buttonMainMenu;
    inventory *tableInventory;
    mainMenu *mainMenuWidget;
    myObject *myObjectWidget;
    mydatabase *mydatabaseObject;
    QPushButton *getDatafromDB;

signals:
    void addedNewInventory(QString, int, int, int);     // сигнал для добавления строки в таблицу inventory в БД
    void updateCellInventory(int, int);                 // сигнал для обновления строки в таблицу inventory в БД
    void deleteCellInventory(int);                      // сигнал для удаления строки в таблицу inventory в БД
    void addedNewObject(QString, QString);              // сигнал для добавления строки в таблицу object в БД
    void removeObject(QString);                         // сигнал для удаления строки в таблицу object в БД
    void clearTableInventory();                         // сигнал для очистки таблицы inventory в БД

private slots:
    void exit();
    void newGame();
    void buttonMainMenuClicked();

protected:
    void resizeEvent(QResizeEvent *event);


};

#endif // DIALOG_H
