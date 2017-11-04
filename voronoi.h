#ifndef VORONOI_H
#define VORONOI_H
#include <stdio.h>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;


struct dot{
    double x;
    double y;
    int    partition;
};
typedef std::vector<dot> dot_set;
typedef std::vector<dot_set> partition;

class voronoi
{
public:
    voronoi(double x , double y ,double dq);

    dot_set q;
    dot_set vor_points;
    //partition vor_partition;
    void q_define(dot_set& q);
    void set_vor_points(dot_set &data);
    void allocate(dot_set& data);
    double dq;
    double x_range;
    double y_range;
    void print();

private:


};

#endif // VORONOI_H
