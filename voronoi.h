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
    double vx;
    double vy;
    int    partition;
    double density;

};
typedef std::vector<dot> dot_set;
typedef std::vector<dot_set> vor_partition;

class voronoi
{
public:
    voronoi(double x , double y ,double dq);

    dot_set q;
    dot_set vor_points;
    //partition vor_partition;
    //vor_partition partition;
    void q_define(dot_set& data);
    void set_vor_points(dot_set &data);
    void allocate(dot_set& data);
    void distribute(dot_set& data , vor_partition& partition);
    void set_density(dot_set& data,double k,double u_x,double u_y,double sigma);
    void get_partition_masscenter(vor_partition &data, dot_set &m);

    double dq;
    double x_range;
    double y_range;
    void print();
    double sigma;
    double k;
    double u_x;
    double u_y;
private:


};

#endif // VORONOI_H
