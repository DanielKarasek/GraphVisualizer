//
// Created by pako on 25.03.22.
//

#ifndef GRAPHVIZUALIZER_GVGRAPH_H
#define GRAPHVIZUALIZER_GVGRAPH_H

#include "GVNode.h"
#include "GVEdge.h"
#include <QGraphicsScene>

#include <QString>
#include <QFont>
#include <QMap>
#include <graphviz/gvc.h>


class GVGraph : public QGraphicsScene
{
public:
  struct EdgeID
  {
    QString first_node;
    QString second_node;
    QString name;
    friend bool operator<(const EdgeID &first, const EdgeID &second);
  };
  GVGraph(QString name, QFont font=QFont(), double node_size=50);
  ~GVGraph();

  void addNode(const QString &name);
  void addNodes(const QStringList &name);

  // remove node, edge
  void addEdge(const QString &source, const QString &target, const QString &name);

  void setattr(const QString &obj_name, const QString &name, const QString &value);
  QString getattr(const QString &obj_name, const QString &name, const QString &value);


  void clear();

  void setFont(QFont font);
  void setRootNode(const QString &name);

  void applyLayout();

  QRectF boundingRect();

  Agraph_t *get_graph(){return m_graph;}

protected:
  const double DotDefaultDPI{72.0};

  GVC_t *m_context;
  Agraph_t *m_graph;
  QFont m_font;
  QMap<QString, std::shared_ptr<GVNode>> m_nodes{};
  QMap<QString, std::shared_ptr<GVEdge>> m_edges{};
};


#endif //GRAPHVIZUALIZER_GVGRAPH_H

