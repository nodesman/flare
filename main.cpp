#include <QtGui/QApplication>
#include "flare.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    flare w;
    w.show();
    return a.exec();
}
