#include "cloudfile.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cloudFile w;
    w.show();

    return a.exec();
}
