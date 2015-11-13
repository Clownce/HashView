#include "hashview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HashView w;
    w.show();

    return a.exec();
}
