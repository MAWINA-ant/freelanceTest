#include "dialog.h"
#include "identification.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    return a.exec();
}
