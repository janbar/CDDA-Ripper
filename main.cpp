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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtGlobal>
#else
#include <QtEnvironmentVariables>
#include <QtSystemDetection>
#endif

#include "goodstyle.h"
#include "thumbnailer/netmanager.h"
#include "mainwindow.h"
#include "utils/tmpdir.h"

#define APP_DISPLAY_NAME  "CDDA-Ripper"
#define APP_DESKTOP_NAME  "io.github.janbar.cddaripper"
#define APP_ORG_NAME      "janbar"

void prepareTranslator(QGuiApplication& app, const QString& translationPath, const QString& translationPrefix, const QLocale& locale);
void setupApp(QGuiApplication& app);
void stylePalette(QPalette& palette, const QString& style);

int main(int argc, char* argv[])
{
  bool noStyle = false;
#ifdef Q_OS_LINUX
  const QByteArray currentDesktop = qgetenv("XDG_CURRENT_DESKTOP").toLower();
  const QByteArray sessionDesktop = qgetenv("XDG_SESSION_DESKTOP").toLower();
  const bool isKde = currentDesktop.contains("kde") || sessionDesktop.contains("kde");
  const bool isGnome = currentDesktop.contains("gnome") || sessionDesktop.contains("gnome");

  if (!qEnvironmentVariableIsSet("QT_QPA_PLATFORM"))
  {
    // Enforce XCB on Linux/Gnome, if the user didn't override via QT_QPA_PLATFORM
    // TODO: Reconsider when Qt/Wayland is reliably working on the supported distributions
    const bool hasWaylandDisplay = qEnvironmentVariableIsSet("WAYLAND_DISPLAY");
    const bool isWaylandSessionType = qgetenv("XDG_SESSION_TYPE") == "wayland";
    const bool isWayland = hasWaylandDisplay || isWaylandSessionType;
    if (isGnome && isWayland)
    {
        qInfo() << "Warning: Ignoring WAYLAND_DISPLAY on Gnome."
                << "Use QT_QPA_PLATFORM=wayland to run on Wayland anyway.";
        qputenv("QT_QPA_PLATFORM", "xcb");
    }
  }

  if (isKde)
  {
#ifndef ENABLE_NATIVE_STYLE
    qInfo() << "Using native style has been disabled at compile time.";
#else
    qInfo() << "Using native style on Kde.";
    noStyle = true;
#endif
  }
#endif

  QGuiApplication::setApplicationName(APP_DESKTOP_NAME);
  QGuiApplication::setDesktopFileName(APP_DESKTOP_NAME);
  QGuiApplication::setApplicationDisplayName(APP_DISPLAY_NAME);
  QGuiApplication::setOrganizationName(APP_ORG_NAME);

  QApplication app(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
  setupApp(app);

  if (!noStyle)
  {
    QApplication::setStyle(new GoodStyle);
    QSettings settings;
    QPalette palette = QGuiApplication::palette();
    stylePalette(palette, settings.value("stylePalette", QString("light")).toString());
    app.setPalette(palette);
  }

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
  QString i18Path(translationPath);
  i18Path.append("/").append(translationPrefix).append("_").append(locale.name().left(2)).append(".qm");
  QTranslator * translator = new QTranslator(&app);
  if (translator->load(locale, translationPrefix, QString("_"), translationPath))
  {
    qInfo("using file '%s' for translations.", i18Path.toUtf8().constData());
    app.installTranslator(translator);
  }
  else
  {
    qWarning("no file found for translations '%s' (using default).", i18Path.toUtf8().constData());
    delete translator;
  }
}

void setupApp(QGuiApplication& app)
{
  QLocale locale = QLocale::system();
  qInfo("User locale setting is %s", std::locale().name().c_str());
  // set translators
  prepareTranslator(app, QString(":/i18n"), QString("cddaripper"), locale);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QString qt_translationPath(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#else
  QString qt_translationPath(QLibraryInfo::path(QLibraryInfo::TranslationsPath));
#endif
  prepareTranslator(app, qt_translationPath, "qtbase", locale);

  app.setWindowIcon(QIcon(QPixmap(":/icons/cddaripper-128x128.png")));
}

void stylePalette(QPalette& palette, const QString& style)
{
  if (style == "dark")
  {
    palette.setColor(QPalette::Window, QColor(69,69,69));
    palette.setColor(QPalette::WindowText, QColor(200,200,200));
    palette.setColor(QPalette::Base, QColor(48,48,48));
    palette.setColor(QPalette::Shadow, QColor(255,255,255));
    palette.setColor(QPalette::AlternateBase, QColor(69,69,69));
    palette.setColor(QPalette::ToolTipBase, QColor(48,48,48));
    palette.setColor(QPalette::ToolTipText, QColor(220,220,220));
    palette.setColor(QPalette::Text, QColor(220,220,220));
    palette.setColor(QPalette::Button, QColor(69,69,69));
    palette.setColor(QPalette::ButtonText, QColor(240,240,240));
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(128,212,250));
    palette.setColor(QPalette::Highlight, QColor(200,200,200));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127,127,127));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127,127,127));
  }
}
