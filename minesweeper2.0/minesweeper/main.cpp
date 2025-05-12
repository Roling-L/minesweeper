#include "minesweeper.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    minesweeper w;
    w.show();
    return a.exec();
}
