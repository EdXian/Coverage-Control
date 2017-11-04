#include "mainwindow.h"
#include <QApplication>
#include <voronoi.h>
#include <density_func.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    double dq=0.2;
    voronoi vor(10,10,0.2);
    dot_set q;
    dot_set vor_dot(6) ;
    vor_partition partition(6);

    std::vector<double> m;
    dot_set c;

    density_set density_dot;
    density_func den(10,10,0.2);
    den.set_coeffi(2,25,0);  //double sigma=2, u=0,c=25;
    den.dot_define(density_dot);
    vor.q_define(q);

    vor_dot[0].x=0;vor_dot[0].y=0;vor_dot[0].partition=0;
    vor_dot[1].x=1;vor_dot[1].y=1;vor_dot[1].partition=0;
    vor_dot[2].x=0;vor_dot[2].y=1;vor_dot[2].partition=0;
    vor_dot[3].x=3;vor_dot[3].y=1;vor_dot[3].partition=0;
    vor_dot[4].x=-1;vor_dot[4].y=-1;vor_dot[4].partition=0;
    vor_dot[5].x=4;vor_dot[5].y=-4;vor_dot[5].partition=0;
    vor.set_vor_points(vor_dot);
    vor.allocate(q);

    for(int i=0;i<q.size();i++)
    {
        q[i].density =   density_dot[i].density;
    }
    vor.distribute(q,partition);

      for(vor_partition::iterator it_i =partition.begin();it_i!=partition.end();it_i++)
      {
          double sum=0.0;
          dot_set &data =*it_i;
         for(dot_set::iterator it_j=data.begin();it_j!=data.end();it_j++)
         {
            sum+=dq*dq*(*it_j).density;
         }

         m.push_back(sum);
         sum=0;
      }


      for(vor_partition::iterator it_i =partition.begin();it_i!=partition.end();it_i++)
      {
        double sum_x=0.0,sum_y=0.0;
        dot_set &data =*it_i;
        dot c_;
        for(dot_set::iterator it_j=data.begin();it_j!=data.end();it_j++)
        {
            sum_x+=dq*dq*(*it_j).x*(*it_j).density;
            sum_y+=dq*dq*(*it_j).y*(*it_j).density;
        }
        c_.x = sum_x;
        c_.y = sum_y;
        c.push_back(c_);
        sum_x=0.0;
        sum_y=0.0;
      }
    for(int i=0;i<c.size();i++)
    {
        c[i].x/=m[i];
        c[i].y/=m[i];
        std::cout<<"x = "<< c[i].x<<"  y = "<< c[i].y<<std::endl;
    }



     MainWindow w;

    w.show();

    return a.exec();
}
