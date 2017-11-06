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


void voronoi::set_density(dot_set& data,double k,double u_x,double u_y,double sigma)
{
   for(dot_set::iterator it = data.begin();it!=data.end();it++)
   {
       (*it).density = k/(sigma*2.50599)*exp(-0.1*(((*it).x-u_x)*((*it).x-u_x)+((*it).y-u_y)*((*it).y-u_y))/(2*sigma*sigma));
   }
}

void voronoi::get_partition_masscenter(vor_partition &data,dot_set &m)
{
    for(vor_partition::iterator it_i = data.begin();it_i!=data.end();it_i++)
    {
       double sum=0.0;
       double sum_x=0.0,sum_y=0.0;
       dot c;
       dot_set &_data =*it_i;
       for(dot_set::iterator it_j=_data.begin();it_j!=_data.end();it_j++)
       {
          sum+=(*it_j).density;
          sum_x+=(*it_j).x*(*it_j).density;
          sum_y+=(*it_j).y*(*it_j).density;
       }
       c.x = sum_x/sum;
       c.y = sum_y/sum;
       m.push_back(c);
       sum=0;
    }

}

void voronoi::print(){

    std::cout<<"ok"<<std::endl;
}
