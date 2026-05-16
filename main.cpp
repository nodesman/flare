#include <QApplication>
#include "flare.h"

int main(int argc, char *argv[])
{
    qunsetenv("QT_PLUGIN_PATH");
    qunsetenv("QT_PREFIX");

    QApplication a(argc, argv);
    flare w;
    w.show();
    return a.exec();
}
