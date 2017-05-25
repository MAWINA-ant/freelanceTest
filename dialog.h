#ifndef DIALOG_H
#define DIALOG_H

#include "mainmenu.h"
#include "myobject.h"

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
    QTableWidget *tableInventory;
    mainMenu *mainMenuWidget;
    myObject *myObjectWidget;

private slots:
    void exit();
    void newGame();
    void buttonMainMenuClicked();

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // DIALOG_H
