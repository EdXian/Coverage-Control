#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "stdio.h"
#include "iostream"
#include "math.h"

#include <voronoi.h>
#include <density_func.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iterator>



using namespace Eigen;
using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Iso_rectangle_2 Iso_rectangle_2;
typedef K::Segment_2 Segment_2;
typedef K::Ray_2 Ray_2;
typedef K::Line_2 Line_2;
typedef CGAL::Delaunay_triangulation_2<K>  Delaunay_triangulation_2;

//Handling voronoi_from_delaunay
struct Cropped_voronoi_from_delaunay{
  std::list<Segment_2> m_cropped_vd;
  Iso_rectangle_2 m_bbox;

  Cropped_voronoi_from_delaunay(const Iso_rectangle_2& bbox):m_bbox(bbox){}

  template <class RSL>
  void crop_and_extract_segment(const RSL& rsl){
    CGAL::Object obj = CGAL::intersection(rsl,m_bbox);
    const Segment_2* s=CGAL::object_cast<Segment_2>(&obj);
    if (s) m_cropped_vd.push_back(*s);
  }

  void operator<<(const Ray_2& ray)    { crop_and_extract_segment(ray); }
  void operator<<(const Line_2& line)  { crop_and_extract_segment(line); }
  void operator<<(const Segment_2& seg){ crop_and_extract_segment(seg); }
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


  int graph_id=0;
  std::vector<Point_2> vor_points1;  // vor_points
  QVector<double> dfx(100) , dfy(100);
  voronoi vor(10,10,0.2);
  dot_set q;
  dot_set vor_dot(6) ;
  vor_partition partition(6);
  QVector<double> vor_x(6),vor_y(6);
  QVector<double> vor_cx(6),vor_cy(6);
  std::vector<double> m;
  dot_set c;

  density_set density_dot;
  density_func den(10,10,0.2);
  den.set_coeffi(0.3,20,0,0);  //double sigma=2,c=25 , u=0;
  double k=25,u=0,sigma=2;
  den.dot_update(density_dot);
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
          sum+=(*it_j).density;
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
          sum_x+=(*it_j).x*(*it_j).density;
          sum_y+=(*it_j).y*(*it_j).density;
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


    for(int i=0;i<vor_dot.size();i++)
    {
        vor_x.push_back(vor_dot[i].x);
        vor_y.push_back(vor_dot[i].y);
    }
     ui->customPlot->addGraph();
    ui->customPlot->graph(graph_id)->setPen(QPen(Qt::black));
    ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::LineStyle::lsNone);
    ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph(graph_id)->setData(vor_x,vor_y);
    graph_id++;

    for(int i=0;i<c.size();i++)
    {
        vor_cx.push_back(c[i].x);
        vor_cy.push_back(c[i].y);
    }

    for(int i=0;i<100;i++)
       {
           double dx=0;
           dx=0.2*i-10;
          dfx[i] = dx;
          dfy[i] = k*1/(sigma*2.50599)*exp(-1*(dx-u)*(dx-u)/(2*sigma*sigma));
       }
    ui->customPlot->addGraph();
    ui->customPlot->graph(graph_id)->setPen(QPen(Qt::red));
    ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::LineStyle::lsNone);
    ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph(graph_id)->setData(vor_cx,vor_cy);
    graph_id++;

  //Pi i=1...n
    vor_points1.push_back(Point_2(0,0));
    vor_points1.push_back(Point_2(1,1));
    vor_points1.push_back(Point_2(0,1));
    vor_points1.push_back(Point_2(3,1));
    vor_points1.push_back(Point_2(-1,-1));
    vor_points1.push_back(Point_2(4,-4));

   // std::cout <<"the number of voronoi points is : "<<vor_points.size()<<std::endl;



     Delaunay_triangulation_2 dt2; //create Delaunay_triangulation Object

     dt2.insert(vor_points1.begin(),vor_points1.end()); //insert my Voronoi points

     Iso_rectangle_2 bbox(-10,-10,10,10);            //create boundary for Voronoi diagram
     Cropped_voronoi_from_delaunay vor1(bbox);    //extract the cropped Voronoi diagram

     dt2.draw_dual(vor1);        //calculate

     //read and plot voronoi edge from vector
     for(std::list<Segment_2>::iterator it=vor1.m_cropped_vd.begin();it!=vor1.m_cropped_vd.end();it++)
     {
        Segment_2 &data=*it;
        Point_2 min,max;
        min=data.min();
        max=data.max();
        QVector<double> x(2),y(2);
        x[0]=min.hx();
        x[1]=max.hx();
        y[0]=min.hy();
        y[1]=max.hy();
        ui->customPlot->addGraph();

        ui->customPlot->graph(graph_id)->setData(x,y);
        graph_id++;
     }

    //density function - normal distribution


    ui->customPlot->xAxis->setRange(-10,10);
    ui->customPlot->yAxis->setRange(-10,10);
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    ui->density_function->addGraph();
    ui->density_function->xAxis->setRange(-10,10);
    ui->density_function->yAxis->setRange(0,5);
    ui->density_function->xAxis->setLabel("x");
    ui->density_function->yAxis->setLabel("y");
    ui->density_function->graph(0)->setData(dfx,dfy);

    ui->customPlot->replot();
    ui->density_function->replot();

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

}
