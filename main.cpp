#include "dialog.h"
#include "identification.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    identification role;
    role.show();
    Dialog w;
    /*if (role.getRole())
        Dialog w;
    else
        Dialog w;*/
    QObject::connect(&role, SIGNAL(chosenRole()), &w, SLOT(show()));
    return a.exec();
}
