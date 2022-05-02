//
// Created by pako on 31.03.22.
//
#include "GVNode.h"
#include "GraphUtilities.h"
#include <QColor>
#include <QPainter>
#include <iostream>


GVNode::GVNode(Agraph_t *graph, const QString &name):
m_graph{graph},
m_node{GraphUtilities::agnode(graph, name, 1)}
{
}

GVNode::~GVNode()
{
  agdelete(m_graph, m_node);
}

void GVNode::setattr(const QString &name, const QString &value)
{
  GraphUtilities::agset(m_node, name, value);
}

QString GVNode::getattr(const QString &name, const QString &value)
{
  return GraphUtilities::agget(m_node, name, value);
}

void GVNode::set_label(const QString &label)
{
  GraphUtilities::agset(m_node, "label", label);
}

QString GVNode::get_label()
{
  return GraphUtilities::agget(m_node, "label");
}

QRectF GVNode::boundingRect() const
{
  return m_path.boundingRect();
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void GVNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->save();

  painter->setPen(m_pen);
  painter->drawPath(m_path);

  if (get_label() != QString())
  {
    painter->save();
    painter->setFont(m_font);
    const QRectF rect = boundingRect().adjusted(2,2,-2,-2); //Margin
    painter->drawText(rect, Qt::AlignCenter , get_label());
    painter->restore();
  }
  painter->restore();
}
#pragma GCC diagnostic pop

void GVNode::apply_layout()
{
  // change to cluster parent, try find if I can do it here
  double graph_UR_y = GD_bb(m_graph).UR.y;

  double dpi = GraphUtilities::agget(m_graph, "dpi", "96.0").replace(",", ".").toDouble();
  double dpi_ratio = dpi/72.0;

  int height = ND_height(m_node) * dpi;
  int width = ND_width(m_node) * dpi;

  pointf coord = ND_coord(m_node);
  coord.y = (graph_UR_y - coord.y) * dpi_ratio;
  coord.x = coord.x * dpi_ratio;

  m_path.clear();
  m_path.addEllipse({coord.x,coord.y}, width/2.0, height/2.0);
  m_font.setPointSizeF(getattr("fontsize", "1").toDouble());
  m_font.setFamily(getattr("fontname", "Times-Roman"));
  m_pen.setWidth(1);
  m_pen.setColor(Qt::black);

}



