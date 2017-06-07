#include "dialog.h"
#include <QDebug>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    roleDialog = new identification();
    roleDialog->exec();


    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);
    m_ptxtServer = new QTextEdit;
    m_ptxtServer->setReadOnly(true);

    mainMenuWidget = new mainMenu();
    myObjectWidget = new myObject(myObject::Apple);
    buttonMainMenu = new QPushButton("Главное меню");
    tableInventory = new inventory();
    setAcceptDrops(true);
    createMenu();
    //*********************************************************************

    // Server
    if (roleDialog->getRole() == "Server"){
        m_nNextBlockSize = 0;
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
        connect(this, SIGNAL(clearTableInventory()), tableInventory, SLOT(clearTable()));
        connect(tableInventory, SIGNAL(clearAllInventory(int)), mydatabaseObject, SLOT(clearDataBase(int)));
        //connect(getDatafromDB, SIGNAL(clicked(bool)), mydatabaseObject, SLOT(receivedData()));

        connect(tableInventory, SIGNAL(addedNewInventory(QString,int,int,int)), mydatabaseObject, SLOT(insertDataInventory(QString,int,int,int)));
        connect(tableInventory, SIGNAL(updateCellInventory(int,int)), mydatabaseObject, SLOT(updateDataInventory(int,int)));
        connect(tableInventory, SIGNAL(deleteCellInventory(int)), mydatabaseObject, SLOT(removeDataInventory(int)));

        //connect(tableInventory, SIGNAL(addedToSocket(QString,int,int,int,QIcon)), this, SLOT(slotSendAddedToClient(QString,int,int,int,QIcon)));
        //connect(tableInventory, SIGNAL(updateCellInventory(int,int)), this, SLOT(slotSendUpdateToClient(int,int)));
        //connect(tableInventory, SIGNAL(deleteCellInventory(int)), this, SLOT(slotSendDeleteToClient(int)));


        emit addedNewObject(myObjectWidget->getObjectType(), myObjectWidget->geticonPath());
    }
    //*********************************************************************

    // client

    else if (roleDialog->getRole() == "Client"){
        clientSocket = new QTcpSocket();
        clientSocket->connectToHost(roleDialog->getIpAddress(), roleDialog->getPort());
        connect(clientSocket, SIGNAL(connected()), SLOT(slotConnected()));
        connect(clientSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
        connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

        connect(this, SIGNAL(addToSocket(QString,int,int,int,QIcon)), this, SLOT(slotSendToServer(QString,int,int,int, QIcon)));
        /*connect(this, SIGNAL(updateToSocket(int,int)), this, SLOT(slotSendToServer(int,int)));
        connect(this, SIGNAL(deleteToSocket(int)), this, SLOT(slotSendToServer(int)));*/

        connect(tableInventory, SIGNAL(addedToSocket(QString,int,int,int,QIcon)), this, SLOT(slotSendAddedToServer(QString,int,int,int, QIcon)));
        connect(tableInventory, SIGNAL(updateCellInventory(int,int)), this, SLOT(slotSendUpdateToServer(int,int)));
        connect(tableInventory, SIGNAL(deleteCellInventory(int)), this, SLOT(slotSendDeleteToServer(int)));

    }

    connect(this, SIGNAL(addToSocket(QString,int,int,int,QIcon)), tableInventory, SLOT(addFromSocket(QString,int,int,int,QIcon)));
    connect(this, SIGNAL(changeToSocket(int,int)), tableInventory, SLOT(changeFromSocket(int,int)));
    connect(this, SIGNAL(removeToSocket(int)), tableInventory, SLOT(removeFromSocket(int)));

    connect(mainMenuWidget, SIGNAL(newGame()), this, SLOT(newGame()));
    connect(mainMenuWidget, SIGNAL(exit()), this, SLOT(exit()));
    connect(buttonMainMenu, SIGNAL(clicked(bool)), this, SLOT(buttonMainMenuClicked()));


}

Dialog::~Dialog()
{
    if (roleDialog->getRole() == "Server")
        emit removeObject(myObjectWidget->getObjectType());
}

void Dialog::createMenu(){
    setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint);

    buttonMainMenu->setEnabled(false);
    myObjectWidget->setEnabled(false);
    QHBoxLayout *hLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    QVBoxLayout *vRightLayout = new QVBoxLayout();
    vRightLayout->addWidget(myObjectWidget);
    vRightLayout->addWidget(buttonMainMenu);

    vRightLayout->addWidget(m_ptxtServer);
    vRightLayout->addWidget(m_ptxt);

    hLayout->addWidget(tableInventory);
    hLayout->addLayout(vRightLayout);
    vLayout->addWidget(mainMenuWidget);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
    show();
    buttonMainMenuClicked();
}
//****************
//server
void Dialog::sendToClient(QTcpSocket *pSocket, const QString &str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}

void Dialog::newGame(){
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
    QTcpSocket *pClientSocket = tcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    sendToClient(pClientSocket, "Server Response: Connected!");
}
//****************
//server
void Dialog::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }
        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QString operation;
        QString typeObject;
        int amount;
        int cellIndex;
        int size;
        QIcon icon;
        QString strMessage;
        in >> operation;
        if (operation == "add"){
            in >> typeObject >> amount >> cellIndex >> size >> icon;
            strMessage = "Client has added - " + typeObject + QString::number(amount);
            emit addToSocket(typeObject, amount, cellIndex, size, icon);
        }
        else if (operation == "change"){
            in >> amount >> cellIndex;
            strMessage = "Client has change - " + QString::number(amount) + QString::number(cellIndex);
            emit changeToSocket(amount, cellIndex);
        }
        else if (operation == "remove"){
            in >> cellIndex;
            strMessage = "Client has remove - " + QString::number(cellIndex);
            emit removeToSocket(cellIndex);
        }
        m_ptxtServer->append(strMessage);
        m_nNextBlockSize = 0;
        sendToClient(pClientSocket, "Server Response: Received \"" + strMessage + "\"");
    }
}

void Dialog::slotSendAddedToClient(QString typeObject, int amount, int cellIndex, int size, QIcon icon)
{
    //QTcpSocket *pClientSocket = tcpServer->nextPendingConnection();
    QString operation = "add";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0)<< operation << typeObject << amount << cellIndex << size << icon;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    //pClientSocket->write(arrBlock);
}

void Dialog::slotSendUpdateToClient(int, int)
{

}

void Dialog::slotSendDeleteToClient(int)
{

}

void Dialog::slotReadyRead()
{
    QDataStream in(clientSocket);
        in.setVersion(QDataStream::Qt_4_2);
        for (;;) {
            if (!m_nNextBlockSize) {
                if (clientSocket->bytesAvailable() < sizeof(quint16)) {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (clientSocket->bytesAvailable() < m_nNextBlockSize) {
                break;
            }
            QTime   time;
            QString str;
            in >> time >> str;

            m_ptxt->append(time.toString() + " " + str);
            m_nNextBlockSize = 0;
        }
        /*QString operation;
        QString typeObject;
        int amount;
        int cellIndex;
        int size;
        QIcon icon;
        QString strMessage;
        in >> operation;

        if (operation == "add"){
            in >> typeObject >> amount >> cellIndex >> size >> icon;
            strMessage = "Client has added - " + typeObject + QString::number(amount);
            emit addToSocket(typeObject, amount, cellIndex, size, icon);
        }
        else if (operation == "change"){
            in >> amount >> cellIndex;
            strMessage = "Client has change - " + QString::number(amount) + QString::number(cellIndex);
            emit changeToSocket(amount, cellIndex);
        }
        else if (operation == "remove"){
            in >> cellIndex;
            strMessage = "Client has remove - " + QString::number(cellIndex);
            emit removeToSocket(cellIndex);
        }
        m_ptxt->append(strMessage);*/

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
    m_ptxt->append(strError);
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
    m_ptxt->append("Received the connected() signal");
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    if (mainMenuWidget->isEnabled())
        mainMenuWidget->setGeometry(0, 0, size().width(), 50);
    else
        mainMenuWidget->setGeometry(0, -70, size().width(), 50);
}


