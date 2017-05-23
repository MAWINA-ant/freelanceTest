#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QSplitter>

class mainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit mainMenu(QWidget *parent = 0);

signals:
    void newGame();
    void exit();

public slots:

private slots:
    void buttonExitClicked();
    void buttonNewGameClicked();

private:
    QPushButton *buttonNewGame;
    QPushButton *buttonExit;
};

#endif // MAINMENU_H
