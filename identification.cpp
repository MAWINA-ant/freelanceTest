#include "identification.h"

identification::identification(QWidget *parent)
    : QDialog(parent)
{
    serverRole = new QPushButton("Сервер");
    clientRole = new QPushButton("Клиент");
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(serverRole);
    hLayout->addWidget(clientRole);
    setLayout(hLayout);
    connect(serverRole, SIGNAL(clicked(bool)), this, SLOT(serverRoleChosen()));
    connect(clientRole, SIGNAL(clicked(bool)), this, SLOT(clientRoleChosen()));
}

identification::~identification()
{

}

void identification::serverRoleChosen()
{
    currentRole = "Server"; // server
    close();
}

void identification::clientRoleChosen()
{
    currentRole = "Client"; // client
    close();
}
