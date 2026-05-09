#ifndef KCMCDDB_H
#define KCMCDDB_H

#include <QWidget>
#include "libkcddb/config.h"

class KCMCDDB : public QWidget
{
  Q_OBJECT

  public:

    KCMCDDB(QWidget * parent = nullptr);

    KCDDB::Config * getConfig() const;

  public Q_SLOTS:

    void defaults();
    void save();
    void load();

  protected:
    void checkSettings() const;
};

#endif // KCMCDDB_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
