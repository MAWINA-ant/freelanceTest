#include "mainmenu.h"
#include <QDebug>
#include <QPainter>

mainMenu::mainMenu(QWidget *parent) : QWidget(parent)
{
    buttonNewGame = new QPushButton("Новая игра", this);
    buttonExit = new QPushButton("Выход", this);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(buttonNewGame);
    hLayout->addWidget(buttonExit);
    setLayout(hLayout);
    connect(buttonNewGame, SIGNAL(clicked(bool)), this, SLOT(buttonNewGameClicked()));
    connect(buttonExit, SIGNAL(clicked(bool)), this, SLOT(buttonExitClicked()));
}

void mainMenu::buttonNewGameClicked(){
    emit newGame();
}

void mainMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::yellow);
    painter.drawRect(rect());
}

void mainMenu::buttonExitClicked(){
    emit exit();
}
