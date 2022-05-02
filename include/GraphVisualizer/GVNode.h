//
// Created by pako on 26.03.22.
//

#ifndef GRAPHVIZUALIZER_GVNODE_H
#define GRAPHVIZUALIZER_GVNODE_H
#include <QString>

#include <QGraphicsItem>
#include <QPainterPath>

#include <QPen>
#include <QBrush>
#include <QFont>

#include <graphviz/gvc.h>

class GVNode : public QGraphicsItem
{
public:
  GVNode(Agraph_t *graph, const QString &name);
  ~GVNode();

  void set_label(const QString &label);
  QString get_label();

  void setattr(const QString &name, const QString &value);
  QString getattr(const QString &name, const QString &value);

  QRectF boundingRect() const override;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

  void apply_layout();
  Agnode_t *node(){return m_node;}


protected:
  Agraph_t  *m_graph;
  Agnode_t  *m_node;

  QFont m_font;

  QPainterPath m_path;

  QPen m_pen;
  QBrush m_brush;

};
#endif //GRAPHVIZUALIZER_GVNODE_H
