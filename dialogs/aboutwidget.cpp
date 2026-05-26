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

#include "aboutwidget.h"
#include "ui_aboutwidgetUI.h"
#include "config.h"

#include <QDialogButtonBox>

AboutWidget::AboutWidget(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
  ui.formLayout->addWidget(buttonBox);
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

  ui.label_icon1->setPixmap(QPixmap(":/icons/cddaripper-64x64.png").scaledToWidth(48, Qt::SmoothTransformation));
  ui.label_icon2->setPixmap(QPixmap(":/icons/paranoia.png").scaledToWidth(48, Qt::SmoothTransformation));
  ui.label_icon3->setPixmap(QPixmap(":/icons/musicbrainz.png").scaledToWidth(48, Qt::SmoothTransformation));
  ui.label_icon4->setPixmap(QPixmap(":/icons/deezer.png").scaledToWidth(48, Qt::SmoothTransformation));

  ui.text_about1->setText(tr("<html>An easy to use audio CD ripping application."
          "<br><br><b>Version %1</b>, (c) Jean-Luc Barrière"
          "<br><br>CDDA-Ripper was forked from Audex-0.95, and reworked outside of the KDE frameworks."
          " MusicBrainz and Deezer are used to fetch metadata and cover arts."
          " CDParanoia is the main backend to extract audio track."
          "<br>Note that you should install your chosen encoders separately."
          " FLAC, LAME, FAAC, OGG, and OPUS encoders are currently supported."
          "<br><br><b>Credits&thinsp;:</b>"
          "<br>Audex-0.95 (Marco Nelles)"
          "<br>Solid (kde.org)"
          "<br>Kcddb (kde.org)"
          "<br>Thumbnailer (Jean-Luc Barrière)"
          "<br>GoodStyle (Juergen Skrotzky)"
          "</html>")
          .arg(CDDARIPPER_VERSION));
}

AboutWidget::~AboutWidget()
{
}
