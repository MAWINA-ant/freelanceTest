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
    void buttonExitClicked();
    void buttonNewGameClicked();

private:
    QPushButton *buttonNewGame;
    QPushButton *buttonExit;
    QColor backGroundColor;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // MAINMENU_H
