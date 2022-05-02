//
// Created by pako on 31.03.22.
//

#include "GraphUtilities.h"

namespace GraphUtilities
{
  Agraph_t *agopen(const QString &name, Agdesc_t kind)
  {
    return ::agopen(const_cast<char *>(qPrintable(name)), kind, 0);
  }

  // agget with QString + added alternative return if needed
  QString agget(void *object, const QString &attr, const QString &alt)
  {
    QString str =  ::agget(object, const_cast<char *>(qPrintable(attr)));
    if (str == QString())
      return alt;
    return str;
  }

  int agset(void *obj, const QString &name, const QString &value)
  {
    return ::agsafeset(obj, const_cast<char *>(qPrintable(name)),
                       const_cast<char *>(qPrintable(value)),
                       const_cast<char *>(qPrintable(value)));
  }

  Agsym_t *agedgeattr(Agraph_t *g, const QString &name, const QString &value)
  {
    return ::agattr(g, AGEDGE,const_cast<char *>(qPrintable(name)),
                    const_cast<char *>(qPrintable(value)));
  }

  Agsym_t *agnodeattr(Agraph_t *g, const QString &name, const QString &value)
  {
    return ::agattr(g, AGNODE,const_cast<char *>(qPrintable(name)),
                    const_cast<char *>(qPrintable(value)));
  }
  Agsym_t *aggraphattr(Agraph_t *g, const QString &name, const QString &value)
  {
    return ::agattr(g, AGRAPH,const_cast<char *>(qPrintable(name)),
                    const_cast<char *>(qPrintable(value)));
  }

  Agnode_t *agnode(Agraph_t *g, const QString &name, int create_flags)
  {
    return ::agnode(g, const_cast<char *>(qPrintable(name)), create_flags);
  }

  int gvLayout(GVC_t *gvc, Agraph_t *g, const QString &engine)
  {
    return ::gvLayout(gvc, g, const_cast<char *>(qPrintable(engine)));
  }
}