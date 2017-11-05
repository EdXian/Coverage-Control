#ifndef DENSITY_FUNC_H
#define DENSITY_FUNC_H
#include <voronoi.h>

struct ddot{
 dot coord;
 double density;
};
typedef std::vector<ddot> density_set;

class density_func
{
public:
    density_func(double x , double y ,double dq);
    //dot_set dot;
    //voronoi *vor;
    void update_func(dot_set data);
    void set_coeffi(double _sigma, double _k, double _u_x,double _u_y);
    void dot_update(density_set &data);
    void mi (density_set &data);
    void vor_partition(density_set& data);
    double dq;
    double x_range;
    double y_range;
private:

    double sigma;
    double k;
    double u_x;
    double u_y;
};

#endif // DENSITY_FUNC_H
