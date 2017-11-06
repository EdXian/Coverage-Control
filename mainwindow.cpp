#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graph_id = 0;

    vor = new voronoi(10,10,0.2);
    den = new density_func(10,10,0.2);
//    timer = new QTimer(this);
//        connect(timer, SIGNAL(timeout()),this, SLOT(sub_loop()));
  dot_set vor_dot(6) ;
  vor_partition partition(6);
  QVector<double> vor_x(6),vor_y(6);
  QVector<double> vor_cx(6),vor_cy(6);

  vor_dot[0].x=0;vor_dot[0].y=0;
  vor_dot[1].x=1;vor_dot[1].y=1;
  vor_dot[2].x=0;vor_dot[2].y=1;
  vor_dot[3].x=3;vor_dot[3].y=1;
  vor_dot[4].x=-1;vor_dot[4].y=-1;
  vor_dot[5].x=4;vor_dot[5].y=-4;

  vor->set_vor_points(vor_dot);
  vor->q_define(q);
  vor->allocate(q);
  vor->set_density(q,20,0,0,0.3);
  vor->distribute(q,partition);
  vor->get_partition_masscenter(partition,c);

    for(unsigned int i=0;i<vor_dot.size();i++)
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

    for(unsigned int i=0;i<c.size();i++)
    {
        vor_cx.push_back(c[i].x);
        vor_cy.push_back(c[i].y);
    }

    for(int i=0;i<100;i++)
       {
          double dx=0;
          dx=0.2*i-10;
          dfx.push_back( dx);
          dfy.push_back( k*1/(sigma*2.50599)*exp(-1*(dx-u)*(dx-u)/(2*sigma*sigma)));
       }

    ui->customPlot->addGraph();
    ui->customPlot->graph(graph_id)->setPen(QPen(Qt::red));
    ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::LineStyle::lsNone);
    ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph(graph_id)->setData(vor_cx,vor_cy);
    graph_id++;

  //Pi i=1...n



    vor_points1.push_back(Point_2(vor_dot[0].x,vor_dot[0].y));
    vor_points1.push_back(Point_2(vor_dot[1].x,vor_dot[1].y));
    vor_points1.push_back(Point_2(vor_dot[2].x,vor_dot[2].y));
    vor_points1.push_back(Point_2(vor_dot[3].x,vor_dot[3].y));
    vor_points1.push_back(Point_2(vor_dot[4].x,vor_dot[4].y));
    vor_points1.push_back(Point_2(vor_dot[5].x,vor_dot[5].y));

     Delaunay_triangulation_2 dt2;
     Iso_rectangle_2 bbox(-10,-10,10,10);
     Cropped_voronoi_from_delaunay vor1(bbox);

     dt2.insert(vor_points1.begin(),vor_points1.end());
     dt2.draw_dual(vor1);

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
