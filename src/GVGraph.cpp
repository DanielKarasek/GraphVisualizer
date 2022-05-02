//
// Created by pako on 25.03.22.
//

#include "GVGraph.h"
#include <QGraphicsEllipseItem>
#include <QRectF>
#include <QColor>

#include <graphviz/gvc.h>
#include <iostream>
#include "GraphUtilities.h"


GVGraph::GVGraph(QString name, QFont font, double node_size):
  m_context(gvContext()),
  m_graph(GraphUtilities::agopen(name, Agstrictdirected))
{
  GraphUtilities::agset(m_graph, "overlap", "prism");
  GraphUtilities::agset(m_graph, "splines", "true");
//  GraphUtilities::agset(m_graph, "rankdir", "LR");
  GraphUtilities::agset(m_graph, "pad", "0.9");
  GraphUtilities::agset(m_graph, "dpi", "96,0");
  GraphUtilities::agset(m_graph, "nodesep", "1.5");

  GraphUtilities::agnodeattr(m_graph, "fixedsize", "true");
  GraphUtilities::agnodeattr(m_graph, "label", "");
  GraphUtilities::agnodeattr(m_graph, "regular", "true");
  QString tmp("%1");
  QString nodePtsWidth = tmp.arg(node_size/GraphUtilities::agget(m_graph, "dpi", "96,0").toDouble());
  GraphUtilities::agnodeattr(m_graph, "width", nodePtsWidth.replace(".", ","));
  GraphUtilities::agedgeattr(m_graph, "label", "");

  setFont(font);

}

GVGraph::~GVGraph()
{

}

void GVGraph::addNode(const QString &name)
{
  if (m_nodes.contains(name))
    m_nodes.remove(name);
  m_nodes.insert(name, std::make_shared<GVNode>(m_graph, name));
  addItem(&(*m_nodes[name]));
}

void GVGraph::addNodes(const QStringList &names)
{
  for(const auto &name: names)
    addNode(name);
}
//
//void GVGraph::removeNode(const QString &name)
//{
//  if (!m_nodes.contains(name)) return;
//
//  agdelete(m_graph, m_nodes[name]);
//  m_nodes.remove(name);
//
//  auto keys = m_edges.keys();
//  for (const auto &key : keys)
//    if (key.first_node == name || key.second_node==name)
//      removeEdge(key);
//}

void GVGraph::clear()
{
  auto keys_nodes = m_nodes.keys();
  for (auto const &key: keys_nodes)
    m_nodes.remove(key);

  auto keys_edges = m_edges.keys();
  for (auto const &key: keys_edges)
  {
    m_edges[key]->invalidate_edge();
    m_edges.remove(key);
  }

//  QGraphicsScene::clear();
}

void GVGraph::setRootNode(const QString &name)
{
  if(m_nodes.contains(name))
    GraphUtilities::agset(m_graph, "root", name);
}
void GVGraph::addEdge(const QString &source, const QString &target, const QString &name)
{
  if (!m_nodes.contains(source) || !m_nodes.contains(target)) return;

  auto edge = std::make_shared<GVEdge>(m_nodes[source]->node(), m_nodes[target]->node(), name, m_graph);

  if (!m_edges.contains(name))
    m_edges.insert(name, edge);
  addItem(&(*m_edges[name]));
}

//void GVGraph::removeEdge(const EdgeID &edge_id)
//{
//  if (!m_edges.contains(edge_id)) return;
//
//  agdelete(m_graph, m_edges[edge_id]);
//  m_edges.remove(edge_id);
//}

void GVGraph::setFont(QFont font)
{
  std::cout << font.family().toStdString() << std::endl;
  m_font=font;

  GraphUtilities::agset(m_graph, "fontname", font.family());
  GraphUtilities::agset(m_graph, "fontsize", QString("%1").arg(font.pointSizeF()));

  GraphUtilities::agnodeattr(m_graph, "fontname", font.family());
  GraphUtilities::agnodeattr(m_graph, "fontsize", QString("%1").arg(font.pointSizeF()));

  GraphUtilities::agedgeattr(m_graph, "fontname", font.family());
  GraphUtilities::agedgeattr(m_graph, "fontsize", QString("%1").arg(font.pointSizeF()));
}

void GVGraph::applyLayout()
{
  GraphUtilities::gvLayout(m_context, m_graph, "dot");
  for (const auto &node : m_nodes)
    node->apply_layout();
  for (const auto &edge : m_edges)
    edge->apply_layout();
}

QRectF GVGraph::boundingRect()
{

  double dpi = GraphUtilities::agget(m_graph, "dpi", "96,0").replace(",", ".").toDouble();
  auto corner_positions = GD_bb(m_graph);
  return QRectF{corner_positions.LL.x * (dpi/DotDefaultDPI), corner_positions.LL.y * (dpi/DotDefaultDPI),
                corner_positions.UR.y * (dpi/DotDefaultDPI), corner_positions.UR.y * (dpi/DotDefaultDPI)};
}

bool operator<(const GVGraph::EdgeID &first, const GVGraph::EdgeID &second)
{
  if (first.first_node < second.first_node)
    return true;
  return false;
}

void GVGraph::setattr(const QString &obj_name, const QString &name, const QString &value)
{
  if (m_nodes.contains(obj_name)) m_nodes[obj_name]->setattr(name, value);
  if (m_edges.contains(obj_name)) m_edges[obj_name]->setattr(name, value);
}

QString GVGraph::getattr(const QString &obj_name, const QString &name, const QString &value)
{
  if (m_nodes.contains(obj_name)) return m_nodes[obj_name]->getattr(name, value);
  if (m_edges.contains(obj_name)) return m_edges[obj_name]->getattr(name, value);
  return {};
}
