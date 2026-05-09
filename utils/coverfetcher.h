/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef COVERFETCHER_HEADER
#define COVERFETCHER_HEADER

#include <QByteArray>
#include <QObject>

#include "thumbnailer/thumbnailer.h"

class CoverFetcher : public QObject
{
    Q_OBJECT
public:
    explicit CoverFetcher(thumbnailer::Thumbnailer& tbn, QObject *parent = nullptr);
    ~CoverFetcher() override;

    void startFetchThumbnails(const QString &artist, const QString &album, const int fetchNo);
    void stopFetchThumbnails();
    void startFetchCover(int no);

    inline int count()
    {
        return cover_names.count();
    }

    enum Status { NOS, SEARCHING, FETCHING_THUMBNAIL, FETCHING_COVER };

    inline Status status() const
    {
        return _status;
    }

Q_SIGNALS:
    void fetchedThumbnail(const QByteArray &thumbnail, const QString &caption, int no);
    void allCoverThumbnailsFetched();
    void fetchedCover(const QByteArray &cover);
    void nothingFetched();

    void statusChanged(CoverFetcher::Status status);

    void error(const QString &description, const QString &solution = QString());
    void warning(const QString &description);
    void info(const QString &description);

private Q_SLOTS:
    void finished_request();

private:
    QSharedPointer<thumbnailer::Request> _req;
    Status _status;
    QStringList cover_names;
    QList<QByteArray> cover_thumbnails;

    thumbnailer::Thumbnailer& _tbn;
};

#endif
