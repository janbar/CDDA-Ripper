/*
###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (C) 2017 by Juergen Skrotzky (JorgenVikingGod@gmail.com)          #
#               >> https://github.com/Jorgen-VikingGod                        #
#                                                                             #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################
*/

#include "goodstyle.h"

GoodStyle::GoodStyle() : GoodStyle(styleBase()) {}

GoodStyle::GoodStyle(QStyle *style) : QProxyStyle(style) {}

QStyle *GoodStyle::styleBase(QStyle *style) const {
  static QStyle *base =
      !style ? QStyleFactory::create(QStringLiteral("Fusion")) : style;
  return base;
}

QStyle *GoodStyle::baseStyle() const { return styleBase(); }

void GoodStyle::polish(QPalette &palette) {
  palette.setColor(QPalette::Window, QColor(240, 240, 240));
  palette.setColor(QPalette::WindowText, QColor(0, 0, 0));
  palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(120, 120, 120));
  palette.setColor(QPalette::Base, QColor(255, 255, 255));
  palette.setColor(QPalette::AlternateBase, QColor(233, 231, 227));
  palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
  palette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));
  palette.setColor(QPalette::Text, QColor(0, 0, 0));
  palette.setColor(QPalette::Disabled, QPalette::Text, QColor(120, 120, 120));
  palette.setColor(QPalette::Dark, QColor(160, 160, 160));
  palette.setColor(QPalette::Shadow, QColor(105, 105, 105));
  palette.setColor(QPalette::Button, QColor(240, 240, 240));
  palette.setColor(QPalette::ButtonText, QColor(0, 0, 0));
  palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120, 120, 120));
  palette.setColor(QPalette::BrightText, QColor(50, 150, 207));
  palette.setColor(QPalette::Link, QColor(51, 153, 255));
  palette.setColor(QPalette::Highlight, QColor(50, 150, 207));
  palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(51, 153, 255));
  palette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
  palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(255, 255, 255));
}

void GoodStyle::polish(QApplication *app) {
  if (!app) return;

  // loadstylesheet
  QFile qfDarkstyle(QStringLiteral(":/goodstyle/style.qss"));
  if (qfDarkstyle.open(QIODevice::ReadOnly | QIODevice::Text)) {
    // set stylesheet
    QString qsStylesheet = QString::fromLatin1(qfDarkstyle.readAll());
    app->setStyleSheet(qsStylesheet);
    qfDarkstyle.close();
  }
}
