#include "mainwindow.h"
#include <QApplication>
#include <voronoi.h>


//A class to recover Voronoi diagram from stream.
//Rays, lines and segments are cropped to a rectangle
//so that only segments are stored


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    voronoi vor(10,10,0.2);
    dot_set q;
    vor.q_define(q);

    dot_set vor_dot(6) ;

    vor_dot[0].x=0;vor_dot[0].y=0;vor_dot[0].partition=0;
    vor_dot[1].x=1;vor_dot[1].y=1;vor_dot[1].partition=0;
    vor_dot[2].x=0;vor_dot[2].y=1;vor_dot[2].partition=0;
    vor_dot[3].x=3;vor_dot[3].y=1;vor_dot[3].partition=0;
    vor_dot[4].x=-1;vor_dot[4].y=-1;vor_dot[4].partition=0;
    vor_dot[5].x=4;vor_dot[5].y=-4;vor_dot[5].partition=0;
    vor.set_vor_points(vor_dot);
    vor.allocate(q);

    for(dot_set::iterator it = vor.vor_points.begin();it!=vor.vor_points.end();it++)
    {
        std::cout<<(*it).x<<"  "<<(*it).y<<"  "<<(*it).partition<<std::endl;
    }


    for(dot_set::iterator it=q.begin();it!=q.end();it++)
    {

        std::cout<<(*it).partition;
        if(((co+1)%100)==0)
        {
            std::cout << std::endl;
        }

    }

     MainWindow w;
    w.show();

    return a.exec();
}
