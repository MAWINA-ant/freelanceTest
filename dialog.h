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

signals:
    void addedNewInventory(QString, int, int, int);
    void updateCellInventory(int, int);
    void deleteCellInventory(int);
    void addedNewObject(QString, QString);
    void removeObject(QString);
    void clearTableInventory();

private slots:
    void exit();
    void newGame();
    void buttonMainMenuClicked();

protected:
    void resizeEvent(QResizeEvent *event);


};

#endif // DIALOG_H
