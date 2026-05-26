/*
 *      Copyright (C) 2026 Jean-Luc Barriere
 *
 *  This file is part of CDDA-Ripper
 *
 *  CDDA-Ripper is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  CDDA-Ripper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Noson.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
