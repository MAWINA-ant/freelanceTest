#ifndef DIALOG_H
#define DIALOG_H

#include "mainmenu.h"
#include "myobject.h"
#include "inventory.h"
#include "mydatabase.h"
#include "identification.h"

#include <QDialog>
#include <QtWidgets>
#include <QTcpSocket>
#include <QTcpServer>

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
    identification *roleDialog;

    QTcpSocket *clientSocket;

    QTcpServer *tcpServer;
    QTcpSocket *serverSocket;
    quint16     nextBlockSize;

signals:
    void newGame();

    void addedNewObject(QString, QString);              // сигнал для добавления строки в таблицу object в БД
    void removeObject(QString);                         // сигнал для удаления строки в таблицу object в БД
    void clearTableInventory();                         // сигнал для очистки таблицы inventory в БД

    void addToSocket(QString, int, int , int, QIcon);
    void changeToSocket(int, int);
    void removeToSocket(int);

private slots:
    void exit();    
    void buttonMainMenuClicked();

public slots:

    void slotNewGame();
    //*********************************************
    //server slots
    void slotNewConnection();
    void slotReadClient();
    void slotSendNewGameToClient();
    void slotSendAddedToClient(QString,int,int,int,QIcon);
    void slotSendUpdateToClient(int,int);
    void slotSendDeleteToClient(int);

    //***************************************************
    //client slots
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendNewGameToServer();
    void slotSendAddedToServer(QString,int,int,int,QIcon);
    void slotSendUpdateToServer(int,int);
    void slotSendDeleteToServer(int);
    void slotConnected();

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // DIALOG_H
