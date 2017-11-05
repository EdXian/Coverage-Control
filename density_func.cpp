#include "density_func.h"

density_func::density_func(double x , double y ,double dq)
{
    this->x_range = x;
    this->y_range = y;
    this->dq = dq;
  // vor = new voronoi(x,y,dq);

}

void density_func::update_func(dot_set data)
{

}


void density_func::vor_partition( density_set &data)
{


}

void density_func::dot_update(density_set &data)
{
    for(int i=0;i<(2*x_range/dq);i++)
    {
        for(int j=0;j<(2*y_range/dq);j++)
        {

            ddot _data;
            _data.coord.x=-1*x_range+(dq)*i;
            _data.coord.y=-1*y_range+(dq)*j;
            _data.density = k*1/(sigma*2.50599)*exp(-0.5*((_data.coord.x-u_x)*(_data.coord.x-u_x)+(_data.coord.y-u_y)*(_data.coord.y-u_y))/(2*sigma*sigma));
            data.push_back(_data);
        }
    }
}
void density_func::mi(density_set &data)
{
    for(density_set::iterator it=data.begin();it!=data.end();it++)
    {

    }
}

void density_func::set_coeffi(double _sigma, double _k, double _u_x,double _u_y)
{
    sigma = _sigma;
    k=_k;
    u_x=_u_x;
    u_y = _u_y;
}
