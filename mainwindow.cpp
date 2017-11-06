#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vor_dot(6)
{
    ui->setupUi(this);
    vor = new voronoi(10,10,0.2);
    den = new density_func(10,10,0.2);
    vor_dot[0].x=0;vor_dot[0].y=0;
    vor_dot[1].x=1;vor_dot[1].y=1;
    vor_dot[2].x=0;vor_dot[2].y=1;
    vor_dot[3].x=3;vor_dot[3].y=1;
    vor_dot[4].x=-1;vor_dot[4].y=-1;
    vor_dot[5].x=4;vor_dot[5].y=-4;


    for(dot_set::iterator it=vor_dot.begin();it!=vor_dot.end();it++)
    {
       (*it).vx = 0.1;
       (*it).vy = 0.2;

        std::cout<<"x = "<<(*it).vx<<"  y= "<<(*it).vy<<std::endl;
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
    Iso_rectangle_2 bbox(-10.001,-10.001,10.001,10.001);
    Cropped_voronoi_from_delaunay vor1(bbox);
    graph_id = 0;
    dt2.clear();

    dt2.insert(vor_points1.begin(),vor_points1.end());
    dt2.draw_dual(vor1);

    ui->customPlot->clearGraphs();
    vor_points1.clear();


    for(dot_set::iterator it=vor_dot.begin();it!=vor_dot.end();it++)
    {
         vor_points1.push_back(Point_2((*it).x,(*it).y));
    }
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


    for(dot_set::iterator it=vor_dot.begin();it!=vor_dot.end();it++)
    {
        if(((*it).x>8)||((*it).x<-8))
        {
            (*it).vx *=-1;
        }
        if(((*it).y>8)||((*it).y<-8))
        {
            (*it).vy *=-1;
        }
        (*it).x += (*it).vx;
        (*it).y += (*it).vy;
    }

    ui->customPlot->replot();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

}
