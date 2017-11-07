#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_pushButton_clicked();
    void plot_loop();
private:
    Ui::MainWindow *ui;
    int graph_id;
    std::vector<Point_2> vor_points1;  // vor_points
    dot_set q;
    dot_set c;
    density_set density_dot;

    double k=25,u=0,sigma=2;
    QVector<double> dfx, dfy;
    //voronoi *vor;
    density_func *den;
     QTimer *timer;
     dot_set vor_dot;

};

#endif // MAINWINDOW_H
