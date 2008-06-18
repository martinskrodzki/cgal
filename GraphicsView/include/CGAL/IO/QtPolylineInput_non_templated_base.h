
#ifndef CGAL_Q_POLYLINE_INPUT_2_NON_TEMPLATED_BASE_H
#define CGAL_Q_POLYLINE_INPUT_2_NON_TEMPLATED_BASE_H

#include <QPolygonF>
#include <QPointF>

#include <CGAL/IO/QtInput.h>

class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QGraphicsItem;
class QGraphicsPathItem;
class QKeyEvent;
class QEvent;
class QObject;

namespace CGAL {

class QtPolylineInput_non_templated_base : public QtInput
{
public:
  void setNumberOfVertices(int n)
  {
    n_ = n;
  }

protected:
  // protected constructor
  QtPolylineInput_non_templated_base(QGraphicsScene* s,
                                      int n = 0,
                                      bool closed = true);


  // mousePressEvent returns true iff the event is consummed
  bool mousePressEvent(QGraphicsSceneMouseEvent *event);

  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

  // keyPressEvent returns true iff the event is consummed
  bool keyPressEvent(QKeyEvent *event);
  
  bool eventFilter(QObject *obj, QEvent *event);
  
  void rubberbands(const QPointF& p);

  virtual void generate_polygon() = 0;

protected:
  QPolygonF polygon;

private:
  QGraphicsPathItem *path_item;
  QGraphicsLineItem *b, *e;
  bool closed_;
  int n_;
  QPointF sp;
  QGraphicsScene *scene_;
};

} // namespace CGAL

#endif // CGAL_Q_POLYLINE_INPUT_2_NON_TEMPLATED_BASE_H
