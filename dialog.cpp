#include "dialog.h"
#include <QDebug>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
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
    tableInventory = new inventory();
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

void Dialog::resizeEvent(QResizeEvent *event)
{
    if (mainMenuWidget->isEnabled())
        mainMenuWidget->setGeometry(0, 0, size().width(), 50);
    else
        mainMenuWidget->setGeometry(0, -70, size().width(), 50);
}


