/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/* Some of this code is inspired by amarok 1.4.7
 * SPDX-FileCopyrightText: 2004 Mark Kretschmann <markey@web.de>
 * SPDX-FileCopyrightText: 2004 Stefan Bogner <bochi@online.ms>
 * SPDX-FileCopyrightText: 2004 Max Howell
 */

#include "coverfetcher.h"

#include <QDebug>
#include <QSize>
#include <QTimer>
#include <algorithm>

CoverFetcher::CoverFetcher(thumbnailer::Thumbnailer& tbn, QObject *parent)
    : QObject(parent), _status(NOS), _tbn(tbn)
{
    Q_UNUSED(parent);
    if (!_tbn.isValid())
      _tbn.configure("DEEZER", ""); // choose provider DEEZER
}

CoverFetcher::~CoverFetcher()
{
  if (_req)
    stopFetchThumbnails();
}

void CoverFetcher::startFetchThumbnails(const QString &artist, const QString &album, const int fetchNo)
{
    qDebug() << "Fetch Thumbs ...";
    if (_status != NOS || fetchNo == 0) {
        Q_EMIT nothingFetched();
        return;
    }

    _req = _tbn.getAlbumArt(artist, album, QSize(512, 512), fetchNo);
    if (_req)
    {
        connect(_req.data(), SIGNAL(finished()), this, SLOT(finished_request()));
        _status = SEARCHING;
        Q_EMIT statusChanged(NOS);
    }
    else
        Q_EMIT error("Fetching thumbnails failed");
}

void CoverFetcher::stopFetchThumbnails()
{
    if ((_status != FETCHING_THUMBNAIL) && (_status != SEARCHING))
        return;

    Q_ASSERT(_req);
    _req->cancel();
}

void CoverFetcher::startFetchCover(int no)
{
  qDebug("Fetching cover %d", no);
  if (_status != NOS)
    return;

  if (cover_names.isEmpty() || no < 0 || (no >= cover_names.count()))
      Q_EMIT nothingFetched();
  else
      QTimer::singleShot(100, this, [this, no]() { Q_EMIT fetchedCover(cover_thumbnails[no]); });
}

void CoverFetcher::finished_request()
{
    Q_ASSERT(_req);
    disconnect(_req.data(), SIGNAL(finished()), this, SLOT(finished_request()));
    qDebug() << "searching finished.";
    _status = NOS;
    Q_EMIT statusChanged(NOS);

    if (_req->images().isEmpty())
        Q_EMIT nothingFetched();
    else
    {
        _status = FETCHING_THUMBNAIL;
        Q_EMIT statusChanged(FETCHING_THUMBNAIL);
        cover_thumbnails.clear();
        cover_names.clear();
        int i = 0;
        for (const QByteArray& raw : _req->images())
        {
            cover_thumbnails.push_back(raw);
            cover_names.push_back(QString::number(++i));
            Q_EMIT fetchedThumbnail(cover_thumbnails.back(), cover_names.back(), i);
        }
        _status = NOS;
        Q_EMIT statusChanged(NOS);
        Q_EMIT allCoverThumbnailsFetched();
    }
}
