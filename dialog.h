#ifndef DIALOG_H
#define DIALOG_H

#include "mainmenu.h"

#include <QDialog>
//#include <QPushButton>
//#include <QLayout>
//#include <QTableWidget>
//#include <QHeaderView>
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

private slots:
    void exit();
    void newGame();
    void buttonMainMenuClicked();
};

#endif // DIALOG_H
