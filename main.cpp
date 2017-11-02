#include "mainwindow.h"
#include <QApplication>



//A class to recover Voronoi diagram from stream.
//Rays, lines and segments are cropped to a rectangle
//so that only segments are stored


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


     MainWindow w;
    w.show();

    return a.exec();
}
