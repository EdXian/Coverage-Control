#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vor_dot(6)
{
    ui->setupUi(this);
    //vor = new voronoi(10,10,0.2);
    den = new density_func(10,10,0.2);

    vor_dot[0].x=0;vor_dot[0].y=0;
    vor_dot[1].x=1;vor_dot[1].y=1;
    vor_dot[2].x=0;vor_dot[2].y=1;
    vor_dot[3].x=3;vor_dot[3].y=1;
    vor_dot[4].x=-1;vor_dot[4].y=-1;
    vor_dot[5].x=4;vor_dot[5].y=-4;
    for(dot_set::iterator it=vor_dot.begin();it!=vor_dot.end();it++)
        {
//           (*it).vx = 0.1;
//           (*it).vy = 0.2;

            //std::cout<<"x = "<<(*it).vx<<"  y= "<<(*it).vy<<std::endl;
        }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(plot_loop()));
    timer->start(50);
    ui->customPlot->xAxis->setRange(-10,10);
    ui->customPlot->yAxis->setRange(-10,10);
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
}

void MainWindow::plot_loop()
{
    Delaunay_triangulation_2 dt2;
    Iso_rectangle_2 bbox(-10,-10,10,10);
    Cropped_voronoi_from_delaunay vor1(bbox);
    voronoi vor(10,10,0.2);
    vor_partition partition(6);

    //initialize
    c.clear();
    q.clear();
    graph_id = 0;
    dt2.clear();

    ui->customPlot->clearGraphs();
    vor_points1.clear();

    vor.q_define(q);

    vor.set_vor_points(vor_dot);

    vor.set_density(q,k,u_x,u_y,sigma);
    vor.allocate(q);
    vor.distribute(q,partition);
    vor.get_partition_masscenter(partition,c);

    //plot voronoi diagram




    for(dot_set::iterator it=vor_dot.begin();it!=vor_dot.end();it++)
    {
         vor_points1.push_back(Point_2((*it).x,(*it).y));
    }
    dt2.insert(vor_points1.begin(),vor_points1.end());
    dt2.draw_dual(vor1);

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


    dfx.clear();
    dfy.clear();
    for(unsigned int i=0;i<vor_dot.size();i++)
    {
        dfx.push_back(vor_dot[i].x);
        dfy.push_back(vor_dot[i].y);
        ui->customPlot->addGraph();
        ui->customPlot->graph(graph_id)->setData(dfx,dfy);
        ui->customPlot->graph(graph_id)->setLineStyle(QCPGraph::LineStyle::lsNone);
        ui->customPlot->graph(graph_id)->setScatterStyle(QCPScatterStyle::ssCircle);
        ui->customPlot->graph(graph_id)->setPen(QPen(Qt::red));
        graph_id++;
    }
    ui->customPlot->replot();
    for(unsigned int i=0;i<vor_dot.size();i++)
    {
        vor_dot[i].x += -0.1*(vor_dot[i].x-c[i].x);
        vor_dot[i].y += -0.1*(vor_dot[i].y-c[i].y);
    }

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
        u_x= ui->u_x_value->text().toDouble();
        u_y= ui->u_y_value->text().toDouble();
        k=ui->k_value->text().toDouble();
        sigma=ui->sigma_value->text().toDouble();
}
