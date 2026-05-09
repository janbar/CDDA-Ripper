/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tmpdir.h"

#include <QDebug>
#include <QStandardPaths>

bool TmpDir::g_init = false;
QSharedPointer<QString> TmpDir::g_tmp_path_base(nullptr);
QSharedPointer<QString> TmpDir::g_tmp_path_app(nullptr);

TmpDir::TmpDir(const QString &appName, const QString &sub)
    : QObject()
{
    // setup static config
    if (!g_init) {
        QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
        g_tmp_path_base = QSharedPointer<QString>(new QString());
        g_tmp_path_base->append((dirs.size() ? dirs[0] : "/var/tmp/"));
        qDebug("Found temporary path %s", g_tmp_path_base->toUtf8().constData());
        PID pid;
        g_tmp_path_app = QSharedPointer<QString>(new QString());
        g_tmp_path_app->append(*g_tmp_path_base);
        if (g_tmp_path_app->right(1) != '/')
            g_tmp_path_app->append('/');
        g_tmp_path_app->append(appName).append('.').append(QString::number(pid.getPID())).append('/');
        g_init = true;
    }

    Q_ASSERT(!g_tmp_path_app->isEmpty());
    p_error = false;
    if (!sub.isEmpty())
        p_tmp_path.append(*g_tmp_path_app).append(sub).append('/');
    else
        p_tmp_path.append(*g_tmp_path_app);

    qDebug() << "Temporary folder in use:" << p_tmp_path;
    p_error = !p_create_dir(p_tmp_path);
}

TmpDir::~TmpDir()
{
    // do we have a sub component in the path?
    QSharedPointer<QString> tmp = g_tmp_path_app;
    if (g_init && p_tmp_path.compare(*tmp) != 0) {
        if (p_remove_dir(p_tmp_path)) {
            qDebug("Deleting temporary folder \"%s\"", p_tmp_path.toUtf8().constData());
        } else {
            qDebug("Deleting temporary folder \"%s\" failed", p_tmp_path.toUtf8().constData());
        }
    }
}

void TmpDir::purge()
{
  if (!g_init)
    return;
  QDir dir(*g_tmp_path_app);
  if ((dir.exists()) && (!dir.removeRecursively())) {
      qDebug("Temporary folder \"%s\" not removed.", g_tmp_path_app->toUtf8().constData());
  }
  g_init = false;
}

const QString TmpDir::tmpPath()
{
    p_error = !p_create_dir(p_tmp_path);
    return p_tmp_path;
}

quint64 TmpDir::freeSpace() const
{
    QStorageInfo diskfreespace(p_tmp_path);
    return diskfreespace.bytesAvailable();
}

bool TmpDir::p_create_dir(const QString &dirName)
{
    QDir *dir = new QDir(dirName);
    if (!dir->exists()) {
        if (!dir->mkpath(dirName)) {
            return false;
        }
    }

    return true;
}

bool TmpDir::p_remove_dir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = p_remove_dir(info.absoluteFilePath());
            } else {
                result = QFile::remove(info.absoluteFilePath());
            }
            if (!result) {
                return result;
            }
        }

        result = dir.rmdir(dirName);
    }

    return result;
}
