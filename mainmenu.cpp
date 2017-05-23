#include "mainmenu.h"

mainMenu::mainMenu(QWidget *parent) : QWidget(parent)
{
    buttonNewGame = new QPushButton("Новая игра");
    buttonExit = new QPushButton("Выход");
    QHBoxLayout *hLayout = new QHBoxLayout();
    //QSplitter *splitter = new QSplitter(Qt::Horizontal);
    hLayout->addWidget(buttonNewGame);
    //hLayout->addWidget(splitter);
    hLayout->addWidget(buttonExit);
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
