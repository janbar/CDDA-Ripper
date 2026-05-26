#include "kcmcddb.h"

#include "libkcddb/cache.h"

#include <QCheckBox>
#include <QVBoxLayout>

KCMCDDB::KCMCDDB(QWidget *parent)
  : QWidget(parent)
{
  load();
}

  void
KCMCDDB::defaults()
{
  KCDDB::Config config;
}

  void
KCMCDDB::checkSettings() const
{
  KCDDB::Config config;
  config.load();
}

  void
KCMCDDB::save()
{
  checkSettings();
}

  void
KCMCDDB::load()
{
  KCDDB::Config config;
  config.load();
}

  KCDDB::Config*
KCMCDDB::getConfig() const
{
  return new KCDDB::Config();
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
