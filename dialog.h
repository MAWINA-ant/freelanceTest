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
    quint16     nextBlockSize; // длина следующего блока, полученного от сокета

signals:

    void addedNewObject(QString, QString);              // сигнал для добавления строки в таблицу object в БД
    void removeObject(QString);                         // сигнал для удаления строки в таблицу object в БД
    void clearTableInventory();                         // сигнал для очистки таблицы inventory в БД

    //**********************************************************
    // сигналы для вызова слотов изменения объекта в другой роли
    void addToSocket(QString, int, int , int, QIcon);
    void changeToSocket(int, int);
    void removeToSocket(int);
    void newGame(); // новая игра в двух ролях
    //**********************************************************

public slots:

    void slotNewGame(); // слот при нажатии кнопки новая игра главного меню
    void exit();
    void buttonMainMenuClicked(); // вызывает меню используя анимацию
    //*********************************************
    //server слоты
    void slotNewConnection(); // определение нового соединения с клиентом
    void slotReadClient(); // для прочтения данных от клиента
    void slotSendNewGameToClient();
    void slotSendAddedToClient(QString,int,int,int,QIcon);
    void slotSendUpdateToClient(int,int);
    void slotSendDeleteToClient(int);
    //*************************************************

    //***************************************************
    //client слоты
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendNewGameToServer();
    void slotSendAddedToServer(QString,int,int,int,QIcon);
    void slotSendUpdateToServer(int,int);
    void slotSendDeleteToServer(int);
    void slotConnected();
    //******************************************************

protected:
    void resizeEvent(QResizeEvent *event); // по изменению размера меняется геометрия главного меню
};

#endif // DIALOG_H
