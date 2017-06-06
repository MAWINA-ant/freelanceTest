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
    void sendToClient(QTcpSocket* pSocket, const QString& str);

    QPushButton *buttonMainMenu;
    inventory *tableInventory;
    mainMenu *mainMenuWidget;
    myObject *myObjectWidget;
    mydatabase *mydatabaseObject;
    QPushButton *getDatafromDB;
    identification *roleDialog;

    QTcpSocket *clientSocket;
    QLineEdit*  m_ptxtInput;
    QPushButton* pcmd;
    QTextEdit*  m_ptxt;

    QTcpServer *tcpServer;
    QTextEdit*  m_ptxtServer;
    quint16     m_nNextBlockSize;

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

public slots:
    //*********************************************
    //server slots
    void slotNewConnection();
    void slotReadClient();

    //***************************************************
    //client slots
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotSendToServer(QString,int,int,int);
    void slotSendToServer(int,int);
    void slotSendToServer(int);
    void slotConnected();

protected:
    void resizeEvent(QResizeEvent *event);


};

#endif // DIALOG_H
