#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
#include "stdio.h"
#include "iostream"
#include "math.h"


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

struct q_dot{
    double x;
    double y;
    int party;
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

  double dq=0.2;
  int graph_id=0;

  QVector<q_dot> q;   //q set belongs to D
  QVector<QVector<q_dot>> party;  //pi
  QVector<QVector<QVector<q_dot>>> party_list;//Pi

  QVector<double> dfx(101) , dfy(101);
  double sigma=2, u=0,c=25;

  std::vector<Point_2> vor_points;  // vor_points


  for(int i=0;i<(20/dq);i++)
  {
      for(int j=0;j<(20/dq);j++)
      {

        q_dot data;
        data.x=-10+i*0.2;
        data.y=-10+j*0.2;
        q.push_back(data);
      }
  }


  //Pi i=1...n
    vor_points.push_back(Point_2(0,0));
    vor_points.push_back(Point_2(1,1));
    vor_points.push_back(Point_2(0,1));
    vor_points.push_back(Point_2(3,1));
    vor_points.push_back(Point_2(-1,-1));
    vor_points.push_back(Point_2(4,-4));

    std::cout <<"the number of voronoi points is : "<<vor_points.size()<<std::endl;


  for(QVector<q_dot>::iterator it=q.begin();it!=q.end();it++)
  {
      int minima_index;
      q_dot &data = *it;

      //{q belongs to D |  ||q-pi|| < ||q-pj|| }   q belongs to Pi
        for(std::vector<Point_2>::iterator it_i=vor_points.begin();it_i!=vor_points.end();it_i++)
        {
           for(std::vector<Point_2>::iterator it_j=(it_i);it_j!=vor_points.end();it_j++)
           {
              Point_2 &vor_pi=*it_i;  double distance_i=0; //Voronoi Points pi
              Point_2 &vor_pj=*it_j;  double distance_j=0; //Voronoi Points pi

               distance_i = (data.x-vor_pi.hx())*(data.x-vor_pi.hx())+(data.y-vor_pi.hy())*(data.y-vor_pi.hy());
               distance_j = (data.x-vor_pj.hx())*(data.x-vor_pj.hx())+(data.y-vor_pj.hy())*(data.y-vor_pj.hy());
                //taking square root is not neccessary
               if(distance_i < distance_j)
               {
                auto index=std::distance(vor_points.begin(),it_i);

                minima_index = index;
                 //todo
               }

           }
        }

  }
    int co=0;
    for(QVector<q_dot>::iterator it=q.begin();it!=q.end();it++)
    {
        std::cout<<"count = "<<co<<"x = "<<it->x <<"  y = "<<it->y<< "  party = " <<it->party<<std::endl;
        co++;

    }
   ui->customPlot->xAxis->setRange(-10,10);
   ui->customPlot->yAxis->setRange(-10,10);
   ui->customPlot->xAxis->setLabel("x");
   ui->customPlot->yAxis->setLabel("y");




    //read and plot voronoi points
    for(std::vector<Point_2>::iterator it=vor_points.begin();it!=vor_points.end();it++)
    {
       Point_2 &data = *it;
        QVector<double> x(1) , y(1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(graph_id)->setPen(QPen(Qt::red));
        ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::LineStyle::lsNone);
        ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssCircle);
        x[0]=data.hx();
        y[0]=data.hy();
        ui->customPlot->graph(graph_id)->setData( x,y);
        graph_id++;
    }

     Delaunay_triangulation_2 dt2; //create Delaunay_triangulation Object

     dt2.insert(vor_points.begin(),vor_points.end()); //insert my Voronoi points

     Iso_rectangle_2 bbox(-8,-8,8,8);            //create boundary for Voronoi diagram
     Cropped_voronoi_from_delaunay vor(bbox);    //extract the cropped Voronoi diagram

     dt2.draw_dual(vor);        //calculate

     //read and plot voronoi edge from vector
     for(std::list<Segment_2>::iterator it=vor.m_cropped_vd.begin();it!=vor.m_cropped_vd.end();it++)
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

    for(int i=0;i<=100;i++)
    {
        double dx=0;
        dx=0.2*i-10;
       dfx[i] = dx;
       dfy[i] = c*1/(sigma*2.50599)*exp(-1*(dx-u)*(dx-u)/(2*sigma*sigma));
    }


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
