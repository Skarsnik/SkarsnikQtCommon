#include "logreader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogReader w;
    w.show();

    return a.exec();
}
