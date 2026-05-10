/*
 *      Copyright (C) 2019 Jean-Luc Barriere
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

#include <QApplication>
#include <QGuiApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QTranslator>
#include <QLibraryInfo>
#include <QPalette>

#include "goodstyle.h"
#include "thumbnailer/netmanager.h"
#include "mainwindow.h"
#include "utils/tmpdir.h"

void prepareTranslator(QGuiApplication& app, const QString& translationPath, const QString& translationPrefix, const QLocale& locale);
void setupApp(QGuiApplication& app);

int main(int argc, char* argv[])
{
  QGuiApplication::setApplicationName("cddaripper");
  QGuiApplication::setApplicationDisplayName("CDDA-Ripper");
  QGuiApplication::setOrganizationName("io.github.janbar");

  QApplication app(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
  QApplication::setStyle(new GoodStyle);
  setupApp(app);

//  QPalette darkPalette;
//  darkPalette.setColor(QPalette::Window, QColor(69,69,69));
//  darkPalette.setColor(QPalette::WindowText, QColor(200,200,200));
//  darkPalette.setColor(QPalette::Base, QColor(48,48,48));
//  darkPalette.setColor(QPalette::Shadow, QColor(255,255,255));
//  darkPalette.setColor(QPalette::AlternateBase, QColor(69,69,69));
//  darkPalette.setColor(QPalette::ToolTipBase, QColor(48,48,48));
//  darkPalette.setColor(QPalette::ToolTipText, QColor(220,220,220));
//  darkPalette.setColor(QPalette::Text, QColor(220,220,220));
//  darkPalette.setColor(QPalette::Button, QColor(69,69,69));
//  darkPalette.setColor(QPalette::ButtonText, QColor(240,240,240));
//  darkPalette.setColor(QPalette::BrightText, Qt::red);
//  darkPalette.setColor(QPalette::Link, QColor(128,212,250));
//  darkPalette.setColor(QPalette::Highlight, QColor(200,200,200));
//  darkPalette.setColor(QPalette::HighlightedText, Qt::black);
//  darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127,127,127));
//  darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127,127,127));
//  app.setPalette(darkPalette);

  // init SSL configuration
  thumbnailer::NetManager::initSSLDefaultConfiguration();

  auto* window = new MainWindow();
  window->show();

  int ret = app.exec();

  TmpDir::purge();
  return ret;
}

void prepareTranslator(QGuiApplication& app, const QString& translationPath, const QString& translationPrefix, const QLocale& locale)
{
  // load app translations
  QString i18Path(translationPath);
  i18Path.append("/").append(translationPrefix).append("_").append(locale.name().left(2)).append(".qm");
  QTranslator * translator = new QTranslator(&app);
  if (translator->load(locale, translationPrefix, QString("_"), translationPath))
  {
    qInfo("using file '%s' for translations.", i18Path.toUtf8().constData());
    app.installTranslator(translator);

    // try to load qt base translations
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QString qt_translationPath(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#else
    QString qt_translationPath(QLibraryInfo::path(QLibraryInfo::TranslationsPath));
#endif
    QTranslator * qt_translator = new QTranslator(&app);
    if (qt_translator->load(locale, "qtbase", "_", qt_translationPath))
    {
      qInfo("using file '%s' for translations.", qt_translationPath.toUtf8().constData());
      app.installTranslator(qt_translator);
    }
  }
  else
  {
    qWarning("no file found for translations '%s' (using default).", i18Path.toUtf8().constData());
  }
}

void setupApp(QGuiApplication& app)
{
  QLocale locale = QLocale::system();
  qInfo("User locale setting is %s", std::locale().name().c_str());
  // set translators
  prepareTranslator(app, QString(":/i18n"), QString("cddaripper"), locale);
  app.setWindowIcon(QIcon(QPixmap(":/icons/cddaripper-128x128.png")));
}
