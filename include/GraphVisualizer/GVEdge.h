//
// Created by pako on 26.03.22.
//

#ifndef GRAPHVIZUALIZER_GVEDGE_H
#define GRAPHVIZUALIZER_GVEDGE_H

#include <QString>
#include <QPainterPath>

#include <QPen>
#include <QBrush>
#include <QFont>

#include <graphviz/gvc.h>
#include <QGraphicsItem>


class GVEdge : public QGraphicsItem
{
public:
  GVEdge(Agnode_t *source, Agnode_t *target, const QString &name, Agraph_t *graph);
  ~GVEdge();


  void set_label(const QString &label);
  QString get_label();

  void setattr(const QString &name, const QString &value);
  QString getattr(const QString &name, const QString &value);

  void invalidate_edge();

  QRectF boundingRect() const override;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

  void apply_layout();

protected:
  QString m_source;
  QString m_target;

  QPen m_pen;
  QBrush m_brush;
  QFont m_font;
  Agedge_t *m_edge;
  Agraph_t *m_graph;

  textlabel_t *m_label{nullptr};
  QRectF m_label_rect;

  QPainterPath m_path;
};


#endif //GRAPHVIZUALIZER_GVEDGE_H
