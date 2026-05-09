/*
 *      Copyright (C) 2019 Jean-Luc Barriere
 *
 *  This file is part of Noson-App
 *
 *  Noson is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Noson is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Noson.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include "abstractapi.h"

#include <QIODevice>
#include <QSize>
#include <QUrl>

#include <memory>

namespace thumbnailer
{

  class DiskCacheManager;
  class NetManager;
  class NetRequest;
  class AbstractAlbumInfo;

  class AlbumInfo final : public AbstractWorker
  {
    Q_OBJECT

  public:
    AlbumInfo(DiskCacheManager* cache,
            NetManager* nam,
            AbstractAPI* api,
            const QString& artist,
            const QString& album,
            const QSize& requestedSize,
            int fetchCount,
            bool cached,
            QObject* parent = nullptr);
    ~AlbumInfo() override;

    void run() override;

    ReplyStatus error() const override;

    int errorCode() const override;

    QString errorString() const override;

    const QList<QByteArray>& images() const override;

    bool isCached() const override;

  private Q_SLOTS:
    void queryInfo();
    void readInfo();
    void processInfo();
    void processImage();

  private:
    AbstractAPI::Parse_Status parseInfo();
    bool parseServerError();
    void queryImages();
    void queryImage(const QUrl& url);
    void fakeImage();

    DiskCacheManager* m_cache;
    NetManager* m_nam;
    AbstractAPI* m_api;
    QString m_artist;
    QString m_album;
    QSize m_requestedSize;
    int m_fetchCount;
    bool m_cached;
    QUrl m_cacheUrl;
    int m_size;

    QIODevice* m_cacheDev; // not owned by this
    std::unique_ptr<NetRequest> m_call;
    AbstractAPI::error_t m_error;
    QByteArray m_info;
    QList<QUrl> m_imagesToProcess;
    QList<QByteArray> m_images;
    QList<AbstractAlbumInfo::metadata_t> m_metas;

    AbstractAlbumInfo* m_p;
    int m_try;
  };

}
#endif /* ALBUMINFO_H */

