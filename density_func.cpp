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

void density_func::dot_define(density_set &data)
{
    for(int i=0;i<(2*x_range/dq);i++)
    {
        for(int j=0;j<(2*y_range/dq);j++)
        {

            ddot _data;
            _data.coord.x=-1*x_range+(dq)*i;
            _data.coord.y=-1*y_range+(dq)*j;
            _data.density = k*1/(sigma*2.50599)*exp(-1*(_data.coord.x-u)*(_data.coord.x-u)/(2*sigma*sigma));
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

void density_func::set_coeffi(double _sigma, double _k, double _u)
{
    sigma = _sigma;
    k=_k;
    u=_u;
}
