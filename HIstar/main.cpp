#include "HIstar.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HIstar w;
    w.show();
    return a.exec();
}
