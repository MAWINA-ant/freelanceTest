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
    currentRole = 0; // server
    emit chosenRole();
    close();
}

void identification::clientRoleChosen()
{
    currentRole = 1; // client
    emit chosenRole();
    close();
}
