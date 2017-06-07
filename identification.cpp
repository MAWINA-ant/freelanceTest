#include "identification.h"

identification::identification(QWidget *parent)
    : QDialog(parent)
{
    serverRole = new QPushButton("Сервер");
    clientRole = new QPushButton("Клиент");
    QLabel *ipAddressLabel = new QLabel("IP адресс:");
    QLabel *portLabel = new QLabel("Порт:");
    ipAddressLine = new QLineEdit("localhost");
    portLine = new QLineEdit("2323");
    QHBoxLayout *hLayout1 = new QHBoxLayout();
    QHBoxLayout *hLayout2 = new QHBoxLayout();
    QHBoxLayout *hLayout3 = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout1->addWidget(ipAddressLabel);
    hLayout1->addWidget(ipAddressLine);
    hLayout2->addWidget(portLabel);
    hLayout2->addWidget(portLine);
    hLayout3->addWidget(serverRole);
    hLayout3->addWidget(clientRole);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    setLayout(vLayout);
    connect(serverRole, SIGNAL(clicked(bool)), this, SLOT(serverRoleChosen()));
    connect(clientRole, SIGNAL(clicked(bool)), this, SLOT(clientRoleChosen()));
}

identification::~identification()
{

}

void identification::serverRoleChosen()
{
    currentRole = "Server"; // server
    ipAddress = ipAddressLine->text();
    port = portLine->text().toInt();
    close();
}

void identification::clientRoleChosen()
{
    currentRole = "Client"; // client
    ipAddress = ipAddressLine->text();
    port = portLine->text().toInt();
    close();
}

void identification::closeEvent(QCloseEvent *event)
{
    emit closeDialog();
    event->accept();
}
