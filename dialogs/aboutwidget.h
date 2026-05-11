#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QDialog>

#include "ui_aboutwidgetUI.h"

class AboutWidget : public QDialog
{
  Q_OBJECT

public:
  explicit AboutWidget(QWidget *parent = nullptr);
  ~AboutWidget();

private:
  Ui::AboutWidgetUI ui;
};

#endif // ABOUTWIDGET_H
