#include "aboutwidget.h"
#include "ui_aboutwidgetUI.h"
#include "config.h"

AboutWidget::AboutWidget(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);

  ui.label_icon1->setPixmap(QPixmap(":/icons/cddaripper-64x64.png").scaledToWidth(48, Qt::SmoothTransformation));
  ui.label_icon2->setPixmap(QPixmap(":/icons/musicbrainz.png").scaledToWidth(48, Qt::SmoothTransformation));
  ui.label_icon3->setPixmap(QPixmap(":/icons/deezer.png").scaledToWidth(48, Qt::SmoothTransformation));

  ui.label_about->setText(tr(
          "<html>An easy to use audio CD ripping application.<br>"
          "<br><b>Version %1</b>, (c) Jean-Luc Barrière<br>"
          "<br>CDDA-Ripper was forked from Audex-0.95, and reworked outside of the KDE frameworks.</html>")
          .arg(CDDARIPPER_VERSION).arg(QT_VERSION_MAJOR).arg(QT_VERSION_MINOR).arg(QT_VERSION_PATCH));

  connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(close()));
}

AboutWidget::~AboutWidget()
{
}
