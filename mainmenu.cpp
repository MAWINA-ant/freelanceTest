#include "mainmenu.h"
#include <QDebug>

mainMenu::mainMenu(QWidget *parent) : QWidget(parent)
{
    buttonNewGame = new QPushButton("Новая игра");
    buttonExit = new QPushButton("Выход");
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(buttonNewGame);
    hLayout->addWidget(buttonExit);
    qDebug() << "razmer knopok" << buttonNewGame->geometry() << buttonExit->geometry();
    setLayout(hLayout);
    connect(buttonNewGame, SIGNAL(clicked(bool)), this, SLOT(buttonNewGameClicked()));
    connect(buttonExit, SIGNAL(clicked(bool)), this, SLOT(buttonExitClicked()));
}

void mainMenu::buttonNewGameClicked(){
    emit newGame();
}

void mainMenu::buttonExitClicked(){
    emit exit();
}
