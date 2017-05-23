#include "dialog.h"
#include <QDebug>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    createMenu();
    connect(mainMenuWidget, SIGNAL(newGame()), this, SLOT(newGame()));
    connect(mainMenuWidget, SIGNAL(exit()), this, SLOT(exit()));
    connect(buttonMainMenu, SIGNAL(clicked(bool)), this, SLOT(buttonMainMenuClicked()));
}

Dialog::~Dialog()
{

}

void Dialog::createMenu(){

    mainMenuWidget = new mainMenu();
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    qDebug() << mainMenuWidget->geometry();
    buttonMainMenu = new QPushButton("Главное меню");
    tableInventory = new QTableWidget(3,3);
    tableInventory->setMaximumHeight(302);
    tableInventory->setMinimumHeight(302);
    tableInventory->setMaximumWidth(302);
    tableInventory->setMinimumWidth(302);
    for (int i=0; i<3; i++){
        tableInventory->setRowHeight(i,100);
        tableInventory->setColumnWidth(i,100);
    }
    tableInventory->verticalHeader()->hide();
    tableInventory->horizontalHeader()->hide();
    tableInventory->setEnabled(false);
    buttonMainMenu->setEnabled(false);
    QHBoxLayout *hLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    QVBoxLayout *vRightLayout = new QVBoxLayout();
    vRightLayout->addWidget(buttonMainMenu);
    hLayout->addWidget(tableInventory);
    hLayout->addLayout(vRightLayout);
    vLayout->addWidget(mainMenuWidget);
    vLayout->addWidget(splitter);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
}

void Dialog::newGame(){
    tableInventory->setEnabled(true);
    buttonMainMenu->setEnabled(true);
    mainMenuWidget->setEnabled(false);


    QPropertyAnimation *animation = new QPropertyAnimation(mainMenuWidget, "geometry");
    animation->setDuration(2500);
    animation->setStartValue(QRect(0, -40, 200, 130));
    animation->setEndValue(QRect(0, -70, 200, 130));

    animation->start();
}

void Dialog::exit(){
    this->close();
}

void Dialog::buttonMainMenuClicked(){
    tableInventory->setEnabled(false);
    buttonMainMenu->setEnabled(false);
    mainMenuWidget->setEnabled(true);
    QPropertyAnimation *animation = new QPropertyAnimation(mainMenuWidget, "geometry");
    animation->setDuration(2500);
    animation->setStartValue(QRect(0, -70, 200, 130));
    animation->setEndValue(QRect(0, -40, 200, 130));

    animation->start();
}
