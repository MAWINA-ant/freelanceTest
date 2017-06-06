#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H

#include <QDialog>
#include <QtWidgets>

class identification : public QDialog
{
    Q_OBJECT

public:
    identification(QWidget *parent = 0);
    ~identification();
    QString getRole(){return currentRole;}

signals:
    void chosenRole();

private:
    QString currentRole;
    QPushButton *serverRole;
    QPushButton *clientRole;

private slots:
    void serverRoleChosen();
    void clientRoleChosen();
};

#endif // IDENTIFICATION_H
