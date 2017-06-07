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
    //Q_ENUMS(clientServerOperation)

public:
    //enum clientServerOperation{ add, change, remove };
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    void createMenu();
    void sendToClient(QTcpSocket* pSocket, const QString& str);

    QPushButton *buttonMainMenu;
    inventory *tableInventory;
    mainMenu *mainMenuWidget;
    myObject *myObjectWidget;
    mydatabase *mydatabaseObject;
    identification *roleDialog;

    QTcpSocket *clientSocket;
    QTextEdit*  m_ptxt;

    QTcpServer *tcpServer;
    //QTcpSocket *serverSocket;
    QTextEdit*  m_ptxtServer;
    quint16     m_nNextBlockSize;

    //clientServerOperation operation;

signals:

    void addedNewObject(QString, QString);              // сигнал для добавления строки в таблицу object в БД
    void removeObject(QString);                         // сигнал для удаления строки в таблицу object в БД
    void clearTableInventory();                         // сигнал для очистки таблицы inventory в БД

    void addToSocket(QString, int, int , int, QIcon);
    void changeToSocket(int, int);
    void removeToSocket(int);

private slots:
    void exit();
    void newGame();
    void buttonMainMenuClicked();

public slots:
    //*********************************************
    //server slots
    void slotNewConnection();
    void slotReadClient();
    void slotSendAddedToClient(QString,int,int,int,QIcon);
    void slotSendUpdateToClient(int,int);
    void slotSendDeleteToClient(int);

    //***************************************************
    //client slots
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendAddedToServer(QString,int,int,int,QIcon);
    void slotSendUpdateToServer(int,int);
    void slotSendDeleteToServer(int);
    void slotConnected();

protected:
    void resizeEvent(QResizeEvent *event);


};

#endif // DIALOG_H
