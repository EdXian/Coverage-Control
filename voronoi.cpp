#include "voronoi.h"

voronoi::voronoi(double x , double y ,double dq)
{
 this->x_range = x;
 this->y_range = y;
 this->dq = dq;
}


void voronoi::q_define(dot_set& data)
{
    for(int i=0;i<(2*x_range/dq);i++)
    {
        for(int j=0;j<(2*y_range/dq);j++)
        {
            dot _data;
            _data.x=-1*x_range+(dq)*i;
            _data.y=-1*y_range+(dq)*j;
            data.push_back(_data);

        }
    }
}

void voronoi::set_vor_points(dot_set &data)
{
    for(dot_set::iterator it=data.begin();it != data.end();it++)
    {
        vor_points.push_back(*it);
    }
}

void voronoi::allocate(dot_set& data)
{

 for(dot_set::iterator it = data.begin();it!=data.end();it++)
 {
     double mini_dist=sqrt(x_range*x_range+y_range*y_range)+10;
     double dist ;

    for(unsigned int i=0;i<vor_points.size();i++)
    {
        dist = ((*it).x -vor_points[i].x)*((*it).x -vor_points[i].x)+
               ((*it).y -vor_points[i].y)*((*it).y -vor_points[i].y);
        dist = sqrt(dist);
        if(mini_dist>dist)
        {
          mini_dist=dist;
          (*it).partition = i;
        }
    }

    mini_dist=sqrt(x_range*x_range+y_range*y_range);
 }
}


void voronoi::distribute(dot_set& data , vor_partition& partition)
{
   // partition(vor_points.size());
  for(dot_set::iterator it = data.begin();it!=data.end();it++)  //  use iterator???
  {
    partition[(*it).partition].push_back(*(it));

  }
}

void voronoi::print(){

    std::cout<<"ok"<<std::endl;
}
