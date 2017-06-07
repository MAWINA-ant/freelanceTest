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
    QString getIpAddress(){return ipAddress;}
    int getPort(){return port;}

signals:
    void chosenRole();
    void closeDialog();

private:
    QString currentRole;
    QString ipAddress;
    int port;
    QPushButton *serverRole;
    QPushButton *clientRole;
    QLineEdit *ipAddressLine;
    QLineEdit *portLine;

private slots:
    void serverRoleChosen();
    void clientRoleChosen();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // IDENTIFICATION_H
