#include "dialog.h"
#include <QDebug>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    roleDialog = new identification();
    roleDialog->exec();

    mainMenuWidget = new mainMenu();
    myObjectWidget = new myObject(myObject::Apple);
    buttonMainMenu = new QPushButton("Главное меню");
    tableInventory = new inventory();
    setAcceptDrops(true);
    createMenu();
    nextBlockSize = 0;
    //*********************************************************************
    // Server
    if (roleDialog->getRole() == "Server"){
        tcpServer = new QTcpServer(this);
        mydatabaseObject = new mydatabase();
        if (!tcpServer->listen(QHostAddress(roleDialog->getIpAddress()), roleDialog->getPort())) {
            QMessageBox::critical(0,
                                  "Server Error",
                                  "Unable to start the server:"
                                  + tcpServer->errorString()
                                     );
            tcpServer->close();
            return;
        }
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
        connect(this, SIGNAL(addedNewObject(QString,QString)), mydatabaseObject, SLOT(insertDataObject(QString,QString)));
        connect(this, SIGNAL(removeObject(QString)), mydatabaseObject, SLOT(removeDataObject(QString)));
        connect(tableInventory, SIGNAL(clearAllInventory(int)), mydatabaseObject, SLOT(clearDataBase(int)));
        connect(tableInventory, SIGNAL(addedNewInventory(QString,int,int,int)), mydatabaseObject, SLOT(insertDataInventory(QString,int,int,int)));
        connect(tableInventory, SIGNAL(updateCellInventory(int,int)), mydatabaseObject, SLOT(updateDataInventory(int,int)));
        connect(tableInventory, SIGNAL(deleteCellInventory(int)), mydatabaseObject, SLOT(removeDataInventory(int)));
        emit addedNewObject(myObjectWidget->getObjectType(), myObjectWidget->geticonPath());
    }
    //*********************************************************************
    // client

    else if (roleDialog->getRole() == "Client"){
        clientSocket = new QTcpSocket();
        clientSocket->connectToHost(roleDialog->getIpAddress(), roleDialog->getPort());
        connect(clientSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
        connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    }
    connect(this, SIGNAL(newGame()), this, SLOT(slotNewGame()));
    connect(this, SIGNAL(clearTableInventory()), tableInventory, SLOT(clearTable()));
    connect(this, SIGNAL(addToSocket(QString,int,int,int,QIcon)), tableInventory, SLOT(addFromSocket(QString,int,int,int,QIcon)));
    connect(this, SIGNAL(changeToSocket(int,int)), tableInventory, SLOT(changeFromSocket(int,int)));
    connect(this, SIGNAL(removeToSocket(int)), tableInventory, SLOT(removeFromSocket(int)));
    connect(mainMenuWidget, SIGNAL(newGame()), this, SLOT(slotNewGame()));
    connect(mainMenuWidget, SIGNAL(exit()), this, SLOT(exit()));
    connect(buttonMainMenu, SIGNAL(clicked(bool)), this, SLOT(buttonMainMenuClicked()));
}

Dialog::~Dialog()
{
    if (roleDialog->getRole() == "Server")
        emit removeObject(myObjectWidget->getObjectType());
}

void Dialog::createMenu(){
    setWindowFlags(Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint |
                   Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint |
                   Qt::WindowCloseButtonHint);
    buttonMainMenu->setEnabled(false);
    myObjectWidget->setEnabled(false);
    QHBoxLayout *hLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    QVBoxLayout *vRightLayout = new QVBoxLayout();
    vRightLayout->addWidget(myObjectWidget);
    vRightLayout->addWidget(buttonMainMenu);
    hLayout->addWidget(tableInventory);
    hLayout->addLayout(vRightLayout);
    vLayout->addWidget(mainMenuWidget);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
    show();
    buttonMainMenuClicked();
}

void Dialog::slotNewGame(){
    tableInventory->setEnabled(true);
    buttonMainMenu->setEnabled(true);
    myObjectWidget->setEnabled(true);
    mainMenuWidget->setEnabled(false);
    QPropertyAnimation *animation = new QPropertyAnimation(mainMenuWidget, "geometry");
    animation->setDuration(2500);
    animation->setStartValue(QRect(0, 0, size().width(), 50));
    animation->setEndValue(QRect(0, -70, size().width(), 50));
    animation->start();
    emit clearTableInventory();
}

void Dialog::exit(){
    this->close();
}

void Dialog::buttonMainMenuClicked(){
    tableInventory->setEnabled(false);
    buttonMainMenu->setEnabled(false);
    myObjectWidget->setEnabled(false);
    mainMenuWidget->setEnabled(true);
    QPropertyAnimation *animation = new QPropertyAnimation(mainMenuWidget, "geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(0, -70, size().width(), 50));
    animation->setEndValue(QRect(0, 0, size().width(), 50));
    animation->start();
}
//****************
//server
void Dialog::slotNewConnection()
{
    serverSocket = tcpServer->nextPendingConnection();
    connect(serverSocket, SIGNAL(disconnected()), serverSocket, SLOT(deleteLater()));
    connect(serverSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    connect(mainMenuWidget, SIGNAL(newGame()), this, SLOT(slotSendNewGameToClient()));
    connect(tableInventory, SIGNAL(addedToSocket(QString,int,int,int,QIcon)), this, SLOT(slotSendAddedToClient(QString,int,int,int,QIcon)));
    connect(tableInventory, SIGNAL(updateToSocket(int,int)), this, SLOT(slotSendUpdateToClient(int,int)));
    connect(tableInventory, SIGNAL(deleteToSocket(int)), this, SLOT(slotSendDeleteToClient(int)));
}
//****************
//server
void Dialog::slotReadClient()
{
    QDataStream in(serverSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!nextBlockSize) {
            if (serverSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> nextBlockSize;
        }
        if (serverSocket->bytesAvailable() < nextBlockSize) {
            break;
        }
        QString operation;
        QString typeObject;
        int amount;
        int cellIndex;
        int size;
        QIcon icon;
        in >> operation;
        if (operation == "add"){
            in >> typeObject >> amount >> cellIndex >> size >> icon;
            emit addToSocket(typeObject, amount, cellIndex, size, icon);
        }
        else if (operation == "change"){
            in >> amount >> cellIndex;
            emit changeToSocket(amount, cellIndex);
        }
        else if (operation == "remove"){
            in >> cellIndex;
            emit removeToSocket(cellIndex);
        }
        else if (operation == "newGame"){
            emit newGame();
        }
        nextBlockSize = 0;
    }
}

void Dialog::slotSendNewGameToClient()
{
    QString operation = "newGame";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << operation;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    serverSocket->write(arrBlock);
}

void Dialog::slotSendAddedToClient(QString typeObject, int amount, int cellIndex, int size, QIcon icon)
{
    QString operation = "add";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << operation << typeObject << amount << cellIndex << size << icon;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    serverSocket->write(arrBlock);
}

void Dialog::slotSendUpdateToClient(int amount, int cellIndex)
{
    QString operation = "change";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << operation << amount << cellIndex;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    serverSocket->write(arrBlock);
}

void Dialog::slotSendDeleteToClient(int cellIndex)
{
    QString operation = "remove";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << operation << cellIndex;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    serverSocket->write(arrBlock);
}

void Dialog::slotReadyRead()
{
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!nextBlockSize) {
            if (clientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> nextBlockSize;
        }
        if (clientSocket->bytesAvailable() < nextBlockSize) {
            break;
        }
        QString operation;
        QString typeObject;
        int amount;
        int cellIndex;
        int size;
        QIcon icon;
        in >> operation;
        if (operation == "add"){
           in >> typeObject >> amount >> cellIndex >> size >> icon;
           emit addToSocket(typeObject, amount, cellIndex, size, icon);
        }
        else if (operation == "change"){
            in >> amount >> cellIndex;
            emit changeToSocket(amount, cellIndex);
        }
        else if (operation == "remove"){
            in >> cellIndex;
            emit removeToSocket(cellIndex);
        }
        else if (operation == "newGame"){
            emit newGame();
        }
        nextBlockSize = 0;
    }
}

void Dialog::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                         "The host was not found." :
                         err == QAbstractSocket::RemoteHostClosedError ?
                         "The remote host is closed." :
                         err == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused." :
                         QString(clientSocket->errorString())
                        );
    qDebug() << strError;
}

void Dialog::slotSendNewGameToServer()
{
    QString operation = "newGame";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << operation;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    clientSocket->write(arrBlock);
}

void Dialog::slotSendAddedToServer(QString typeObject, int amount, int cellIndex, int size, QIcon icon)
{
    QString operation = "add";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << operation << typeObject << amount << cellIndex << size << icon;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    clientSocket->write(arrBlock);
}

void Dialog::slotSendUpdateToServer(int amount, int cellIndex)
{
    QString operation = "change";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << operation << amount << cellIndex;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    clientSocket->write(arrBlock);
}

void Dialog::slotSendDeleteToServer(int cellIndex)
{
    QString operation = "remove";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << operation << cellIndex;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    clientSocket->write(arrBlock);
}

void Dialog::slotConnected()
{
    connect(mainMenuWidget, SIGNAL(newGame()), this, SLOT(slotSendNewGameToServer()));
    connect(tableInventory, SIGNAL(addedToSocket(QString,int,int,int,QIcon)), this, SLOT(slotSendAddedToServer(QString,int,int,int, QIcon)));
    connect(tableInventory, SIGNAL(updateToSocket(int,int)), this, SLOT(slotSendUpdateToServer(int,int)));
    connect(tableInventory, SIGNAL(deleteToSocket(int)), this, SLOT(slotSendDeleteToServer(int)));
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    if (mainMenuWidget->isEnabled())
        mainMenuWidget->setGeometry(0, 0, size().width(), 50);
    else
        mainMenuWidget->setGeometry(0, -70, size().width(), 50);
}


