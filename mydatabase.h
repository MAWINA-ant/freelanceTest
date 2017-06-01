#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>

class mydatabase : public QObject
{
    Q_OBJECT
public:
    explicit mydatabase(QObject *parent = 0);

signals:

public slots:
};

#endif // MYDATABASE_H