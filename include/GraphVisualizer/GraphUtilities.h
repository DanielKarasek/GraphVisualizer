//
// Created by pako on 31.03.22.
//

#ifndef GRAPHVIZUALIZER_GRAPHUTILITIES_H
#define GRAPHVIZUALIZER_GRAPHUTILITIES_H

#include <graphviz/gvc.h>
#include <QString>

namespace GraphUtilities
{
  Agraph_t *agopen(const QString &name, Agdesc_t kind);

  Agnode_t *agnode(Agraph_t *g, const QString &name, int create_flags);

  // agget with QString + added alternative return if needed
  QString agget(void *object, const QString &attr, const QString &alt = QString());
  int agset(void *obj, const QString &name, const QString &value);

  Agsym_t *agedgeattr(Agraph_t *g, const QString &name, const QString &value);
  Agsym_t *agnodeattr(Agraph_t *g, const QString &name, const QString &value);
  Agsym_t *aggraphattr(Agraph_t *g, const QString &name, const QString &value);

  int gvLayout(GVC_t *gvc, Agraph_t *g, const QString &engine);
}

#endif //GRAPHVIZUALIZER_GRAPHUTILITIES_H
