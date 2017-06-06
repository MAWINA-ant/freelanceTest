#include "dialog.h"
#include <QDebug>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    roleDialog = new identification();
    roleDialog->exec();
    if (roleDialog->getRole() == "Server"){
        tcpServer = new QTcpServer(this);
        if (!tcpServer->listen(QHostAddress::Any, 2323)) {
            QMessageBox::critical(0,
                                  "Server Error",
                                  "Unable to start the server:"
                                  + tcpServer->errorString()
                                     );
            tcpServer->close();
            return;
        }
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    }
    else if (roleDialog->getRole() == "Client"){


        clientSocket = new QTcpSocket(this);
        clientSocket->connectToHost("localhost", 2323);
        connect(clientSocket, SIGNAL(connected()), SLOT(slotConnected()));
        connect(clientSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
        connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));












    }
    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);
    m_ptxtInput = new QLineEdit;
    connect(m_ptxtInput, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
    pcmd = new QPushButton("&Send");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));


    mydatabaseObject = new mydatabase();
    setAcceptDrops(true);
    createMenu();
    connect(mainMenuWidget, SIGNAL(newGame()), this, SLOT(newGame()));
    connect(mainMenuWidget, SIGNAL(exit()), this, SLOT(exit()));
    connect(buttonMainMenu, SIGNAL(clicked(bool)), this, SLOT(buttonMainMenuClicked()));
    connect(tableInventory, SIGNAL(addedNewInventory(QString,int,int,int)), this, SIGNAL(addedNewInventory(QString,int,int,int)));
    connect(this, SIGNAL(addedNewInventory(QString,int,int,int)), mydatabaseObject, SLOT(insertDataInventory(QString,int,int,int)));
    connect(tableInventory, SIGNAL(updateCellInventory(int,int)), this, SIGNAL(updateCellInventory(int,int)));
    connect(this, SIGNAL(updateCellInventory(int,int)), mydatabaseObject, SLOT(updateDataInventory(int,int)));
    connect(tableInventory, SIGNAL(deleteCellInventory(int)), this, SIGNAL(deleteCellInventory(int)));
    connect(this, SIGNAL(deleteCellInventory(int)), mydatabaseObject, SLOT(removeDataInventory(int)));
    connect(this, SIGNAL(addedNewObject(QString,QString)), mydatabaseObject, SLOT(insertDataObject(QString,QString)));
    connect(this, SIGNAL(removeObject(QString)), mydatabaseObject, SLOT(removeDataObject(QString)));
    connect(myObjectWidget, SIGNAL(addedNewObject(QString,QString)), mydatabaseObject, SLOT(insertDataObject(QString,QString)));
    connect(myObjectWidget, SIGNAL(removeObject(QString)), mydatabaseObject, SLOT(removeDataObject(QString)));
    connect(this, SIGNAL(clearTableInventory()), tableInventory, SLOT(clearTable()));
    connect(tableInventory, SIGNAL(clearInventory(int)), mydatabaseObject, SLOT(clearDataBase(int)));
    connect(getDatafromDB, SIGNAL(clicked(bool)), mydatabaseObject, SLOT(receivedData()));
    connect(mydatabaseObject, SIGNAL(sendData(QString,int,int,QString)), tableInventory, SLOT(updateFromDataBase(QString,int,int,QString)));
    myObjectWidget->addedNewObject(myObjectWidget->getObjectType(), myObjectWidget->geticonPath());
}

Dialog::~Dialog()
{
    myObjectWidget->removeObject(myObjectWidget->getObjectType());
}

void Dialog::createMenu(){
    setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint);
    mainMenuWidget = new mainMenu();
    myObjectWidget = new myObject(myObject::Apple);
    buttonMainMenu = new QPushButton("Главное меню");
    getDatafromDB = new QPushButton("Data");
    tableInventory = new inventory();
    buttonMainMenu->setEnabled(false);
    myObjectWidget->setEnabled(false);
    QHBoxLayout *hLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    QVBoxLayout *vRightLayout = new QVBoxLayout();
    vRightLayout->addWidget(myObjectWidget);
    vRightLayout->addWidget(buttonMainMenu);
    vRightLayout->addWidget(getDatafromDB);


    vRightLayout->addWidget(m_ptxt);
    vRightLayout->addWidget(m_ptxtInput);
    vRightLayout->addWidget(pcmd);


    hLayout->addWidget(tableInventory);
    hLayout->addLayout(vRightLayout);
    vLayout->addWidget(mainMenuWidget);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
    show();
    buttonMainMenuClicked();
}

void Dialog::sendToClient(QTcpSocket *pSocket, const QString &str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_4_2);
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

void Dialog::slotNewConnection()
{
    QTcpSocket* pClientSocket = tcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    sendToClient(pClientSocket, "Server Response: Connected!");
}

void Dialog::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    //in.setVersion(QDataStream::Qt_4_2);
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
        QTime   time;
        QString str;
        in >> time >> str;
        QString strMessage = time.toString() + " " + "Client has sended - " + str;
        m_ptxt->append(strMessage);
        m_nNextBlockSize = 0;
        sendToClient(pClientSocket, "Server Response: Received \"" + str + "\"");
    }
}

void Dialog::slotReadyRead()
{
    QDataStream in(clientSocket);
        //in.setVersion(QDataStream::Qt_4_2);
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

void Dialog::slotSendToServer()
{
    QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        //out.setVersion(QDataStream::Qt_4_2);
        out << quint16(0) << QTime::currentTime() << m_ptxtInput->text();

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        clientSocket->write(arrBlock);
        m_ptxtInput->setText("");
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


