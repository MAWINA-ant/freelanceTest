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
    QString getRole(){return currentRole;} // возвращает роль для дальнейшего создания приложения
                                           // в зависимости от неё
    QString getIpAddress(){return ipAddress;} // записывается ip-адресс для создания сервера или
                                              // коннекта клиента
    int getPort(){return port;} // тоже что и для ip, только для порта

signals:
    void chosenRole();

private:
    QString currentRole;
    QString ipAddress;
    int port;
    QPushButton *serverRole;
    QPushButton *clientRole;
    QLineEdit *ipAddressLine;
    QLineEdit *portLine;

private slots:
    void serverRoleChosen(); // устанавливает значения в приватные поля для сервера
    void clientRoleChosen(); // устанавливает значения в приватные поля для клиента

};

#endif // IDENTIFICATION_H
