//
// Created by pako on 01.04.22.
//
#include <QString>
#include "GVEdge.h"
#include "GraphUtilities.h"
#include <QPainter>
#include <QColor>
#include <iostream>
#include <graphviz/gvc.h>

GVEdge::GVEdge(Agnode_t *source, Agnode_t *target, const QString &name, Agraph_t *graph)
{
  m_edge = agedge(graph, source, target, name.toLocal8Bit().data(), 1);
  m_graph = graph;
  m_source = agnameof(source);
  m_target = agnameof(target);
}

GVEdge::~GVEdge()
{
  if (m_edge != nullptr)
    agdelete(m_graph, m_edge);
}

void GVEdge::set_label(const QString &label)
{
  GraphUtilities::agset(m_edge, "xlabel", label);
}

QString GVEdge::get_label()
{
  return GraphUtilities::agget(m_edge, "xlabel", "");
}

void GVEdge::setattr(const QString &name, const QString &value)
{
  GraphUtilities::agset(m_edge, name, value);
}

QString GVEdge::getattr(const QString &name, const QString &value)
{
  return GraphUtilities::agget(m_edge, name, value);
}
void GVEdge::invalidate_edge()
{
  m_edge = nullptr;
}

QRectF GVEdge::boundingRect() const
{
  return m_path.boundingRect();
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void GVEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->save();
  painter->setPen(m_pen);
//  painter->setBrush(m_brush);
  painter->drawPath(m_path);
  if (m_label)
  {
    painter->save();
    painter->setFont(m_font);

    painter->fillRect(m_label_rect, {255,255,255});
    painter->drawText(m_label_rect, Qt::AlignCenter, m_label->text);

    painter->restore();
  }
  painter->restore();
}
#pragma GCC diagnostic pop

void GVEdge::apply_layout()
{
  double dpi_ratio = GraphUtilities::agget(m_graph, "dpi", "96.0").replace(",", ".").toDouble() / 72.0;
  int graph_upper_right_y = GD_bb(m_graph).UR.y;

  bezier *edge_spline_list = ED_spl(m_edge)->list;
  m_path.clear();
  if ((edge_spline_list != nullptr) && (edge_spline_list->size % 3 == 1))
  {
    //If there is a starting point, draw a line from it to the first curve point
    if (edge_spline_list->sflag)
    {
      m_path.moveTo(edge_spline_list->sp.x * (dpi_ratio),
                    (graph_upper_right_y - edge_spline_list->sp.y) * (dpi_ratio));
      m_path.lineTo(edge_spline_list->list[0].x * (dpi_ratio),
                    (graph_upper_right_y - edge_spline_list->list[0].y) * (dpi_ratio));
    } else
      m_path.moveTo(edge_spline_list->list[0].x * (dpi_ratio),
                    (graph_upper_right_y - edge_spline_list->list[0].y) * (dpi_ratio));

    //Loop over the curve points
    for (int i = 1; i < edge_spline_list->size; i += 3)
      m_path.cubicTo(edge_spline_list->list[i].x * (dpi_ratio),
                     (graph_upper_right_y - edge_spline_list->list[i].y) * (dpi_ratio),
                     edge_spline_list->list[i + 1].x * (dpi_ratio),
                     (graph_upper_right_y - edge_spline_list->list[i + 1].y) * (dpi_ratio),
                     edge_spline_list->list[i + 2].x * (dpi_ratio),
                     (graph_upper_right_y - edge_spline_list->list[i + 2].y) * (dpi_ratio));

    //If there is an ending point, draw a line to it
    if (edge_spline_list->eflag)
      m_path.lineTo(edge_spline_list->ep.x * (dpi_ratio),
                    (graph_upper_right_y - edge_spline_list->ep.y) * (dpi_ratio));
  }
  if (get_label() != QString())
  {
    m_label = ED_xlabel(m_edge);
    m_label_rect.setSize({m_label->dimen.x*dpi_ratio, m_label->dimen.y * dpi_ratio});
    m_label_rect.moveTopLeft({m_label->pos.x*dpi_ratio, (graph_upper_right_y - m_label->pos.y)*dpi_ratio});
  }
  bool ok {true};
  m_pen.setWidth(getattr("width", "1").toInt());

  m_font.setPointSizeF(getattr("fontsize", "1").toDouble());
  m_font.setFamily(getattr("fontname", "Times-Roman"));

  QString color = getattr("color", "#000000");
  int color_value = color.mid(1).toLong(&ok, 16);
  int r {static_cast<int>((color_value>>16) & 0xff)};
  int g {static_cast<int>((color_value>>8) & 0xff)};
  int b {static_cast<int>(color_value & 0xff)};
  m_pen.setColor(QColor(r,g,b));
}
