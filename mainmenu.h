#ifndef MAINMENU_H
#define MAINMENU_H

#include <QPushButton>
#include <QLayout>

class mainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit mainMenu(QWidget *parent = 0);
    void setBackGroundColor(QColor color){ backGroundColor = color; }

signals:
    void newGame();
    void exit();

public slots:

private slots:
    void buttonExitClicked(); //вызов сигнала exit()
    void buttonNewGameClicked(); //вызов сигнала newGame()

private:
    QPushButton *buttonNewGame;
    QPushButton *buttonExit;
    QColor backGroundColor;

protected:
    void paintEvent(QPaintEvent *event); // перерисовка объекта с фоном другого цвета
};

#endif // MAINMENU_H
