#include "minesweeper10.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    minesweeper10 w;
    w.show();
    return a.exec();
}
