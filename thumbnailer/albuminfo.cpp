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

#include "albuminfo.h"
#include "diskcachemanager.h"
#include "netrequest.h"
#include "byteschunk.h"

#include <QDebug>
#include <QUrlQuery>

#define IMAGE_SIZE_SMALL      1
#define IMAGE_SIZE_MEDIUM     2
#define IMAGE_SIZE_LARGE      3
#define IMAGE_SIZE_EXTRALARGE 4
#define DEFAULT_SIZE IMAGE_SIZE_EXTRALARGE
#define EXPIRE_DAYS_SUCCEEDED 360
#define EXPIRE_DAYS_FAILED    8
#define ERRMSG_INVALID        "Invalid response"
#define ERRMSG_NOT_FOUND      "No image found"
#define ERRMSG_API_UNDEF      "API is undefined"
#define ERRMSG_QUOTA_EXCEEDED "Quota limit exceeded"

using namespace thumbnailer;

AlbumInfo::AlbumInfo(DiskCacheManager* cache, NetManager* nam, AbstractAPI* api, const QString& artist, const QString& album, const QSize& requestedSize, int fetchCount, bool cached, QObject* parent)
: AbstractWorker(parent)
, m_cache(cache)
, m_nam(nam)
, m_api(api)
, m_artist(artist)
, m_album(album)
, m_requestedSize(requestedSize)
, m_fetchCount(fetchCount)
, m_cached(cached)
, m_size(0)
, m_cacheDev(nullptr)
, m_call(nullptr)
, m_p(nullptr)
, m_try(0)
{
  m_error.status = ReplyInvalid;
  m_error.errorCode = 0;

  if (requestedSize.width() <= 0 && requestedSize.height() <= 0)
    m_requestedSize = QSize(DEFAULT_SIZE, DEFAULT_SIZE);

  if (m_requestedSize.width() <= 34 && m_requestedSize.height() <=  34)
    m_size = IMAGE_SIZE_SMALL;
  else if (m_requestedSize.width() <= 64 && m_requestedSize.height() <=  64)
    m_size = IMAGE_SIZE_MEDIUM;
  else if (m_requestedSize.width() <= 174 && m_requestedSize.height() <=  174)
    m_size = IMAGE_SIZE_LARGE;
  else
    m_size = IMAGE_SIZE_EXTRALARGE;

  QString str("image://albuminfo/?");
  QUrlQuery qry;
  qry.addQueryItem("artist", m_artist);
  qry.addQueryItem("album", m_album);
  qry.addQueryItem("size", QString::number(m_size));
  str.append(qry.toString());
  m_cacheUrl.setUrl(str);
}

AlbumInfo::~AlbumInfo()
{
  if (m_p)
    delete m_p;
}

void AlbumInfo::run()
{
  QIODevice * cacheDev = m_cache->queryData(m_cacheUrl, m_cached);
  if (cacheDev)
  {
    if (!explode_chunk_set(cacheDev->readAll(), m_images) || m_images.size() == 0)
    {
      m_error.status = ReplyServerError;
      m_error.errorCode = 0;
      m_error.errorString = ERRMSG_NOT_FOUND;
    }
    else
    {
      m_error.status = ReplySuccess;
      m_error.errorCode = 0;
      m_error.errorString.clear();
      m_cached = true; // hold data is cached
    }
    cacheDev->close();
    delete cacheDev;
    Q_EMIT finished();
    return;
  }
  // using cache only
  if (m_cached)
  {
    m_error.status = ReplyServerError;
    m_error.errorCode = 0;
    m_error.errorString = ERRMSG_NOT_FOUND;
    m_cached = false;
    Q_EMIT finished();
    return;
  }

  if (!m_api)
  {
    m_error.status = ReplyFatalError;
    m_error.errorCode = -1;
    m_error.errorString = ERRMSG_API_UNDEF;
    Q_EMIT finished();
    return;
  }
  m_p = m_api->newAlbumInfo(m_artist, m_album, m_fetchCount);
  queryInfo();
}

ReplyStatus AlbumInfo::error() const
{
  return m_error.status;
}

int AlbumInfo::errorCode() const
{
  return m_error.errorCode;
}

QString AlbumInfo::errorString() const
{
  return m_error.errorString;
}

const QList<QByteArray>& AlbumInfo::images() const
{
  return m_images;
}

bool AlbumInfo::isCached() const
{
  return m_cached;
}

void AlbumInfo::queryInfo()
{
  ++m_try;
  m_error.status = ReplySuccess;
  m_error.errorCode = 0;
  m_error.errorString.clear();
  m_info.clear();
  m_call.reset(new NetRequest());
  connect(m_call.get(), SIGNAL(finished(thumbnailer::NetRequest*)), this, SLOT(processInfo()));
  m_p->queryInfo(m_call.get());
  m_call->launch(m_nam);
}

void AlbumInfo::readInfo()
{
  m_info.append(m_call->readData());
}

void AlbumInfo::processInfo()
{
  if (m_call->error())
  {
    // network failure
    if (m_call->errorCode() < 200)
    {
      m_error.status = ReplyNetworkError;
      m_error.errorCode = m_call->errorCode();
      m_error.errorString = m_call->errorString();
      Q_EMIT finished();
      return;
    }

    if (!m_call->atEnd())
      readInfo();

    if (!parseServerError())
    {
      m_error.status = ReplyServerError;
      m_error.errorCode = m_call->errorCode();
      m_error.errorString = m_call->errorString();
      Q_EMIT finished();
      return;
    }
    switch(m_error.status)
    {
    case ReplyFatalError:
    case ReplyQuotaExceeded:
      break;
    default:
      fakeImage();
      break;
    }
    Q_EMIT finished();
    return;
  }

  if (!m_call->atEnd())
    readInfo();

  switch (parseInfo())
  {
  case AbstractAPI::Parse_Failed:
    m_error.status = ReplyInvalid;
    m_error.errorCode = 0;
    m_error.errorString = ERRMSG_INVALID;

    // API could return the error details in the content of response 200
    if (parseServerError())
    {
      switch(m_error.status)
      {
      case ReplyFatalError:
        break;
      case ReplyQuotaExceeded:
        if (m_try <= m_api->maxRetry())
          break;
        m_error.status = ReplyServerError;
        m_error.errorCode = 0;
        m_error.errorString = ERRMSG_QUOTA_EXCEEDED;
        fakeImage();
        break;
      default:
        fakeImage();
        break;
      }
    }
    Q_EMIT finished();
    return;

  case AbstractAPI::Parse_RetryQuery:
    if (m_try <= m_api->maxRetry())
    {
      queryInfo();
      return;
    }
    fakeImage();
    Q_EMIT finished();
    return;

  default:
  {
    int found = m_metas.size();
    for (const AbstractAlbumInfo::metadata_t& meta : std::as_const(m_metas))
    {
      if (!meta.image_small.isEmpty() && m_size < 2)
        m_imagesToProcess.push_back(QUrl(meta.image_small));
      else if (!meta.image_medium.isEmpty() && m_size < 3)
        m_imagesToProcess.push_back(QUrl(meta.image_medium));
      else if (!meta.image_large.isEmpty() && m_size < 4)
        m_imagesToProcess.push_back(QUrl(meta.image_large));
      else if (!meta.image_extralarge.isEmpty())
        m_imagesToProcess.push_back(QUrl(meta.image_extralarge));
      else
        --found;
    }
    if (found > 0)
    {
      // create cache entry
      int days = EXPIRE_DAYS_SUCCEEDED - (EXPIRE_DAYS_SUCCEEDED/2) + (std::rand() % EXPIRE_DAYS_SUCCEEDED);
      m_cacheDev = m_cache->createData(m_cacheUrl, QDateTime::currentDateTime().addDays(days));
      queryImages();
    }
    else
    {
      m_error.status = ReplyNoDataFound;
      m_error.errorCode = 0;
      m_error.errorString = QStringLiteral(ERRMSG_NOT_FOUND " for album=[%1] artist=[%2] size=%3").arg(m_album).arg(m_artist).arg(m_size);
      fakeImage();
      Q_EMIT finished();
    }
  }
  }
}

AbstractAPI::Parse_Status AlbumInfo::parseInfo()
{
  return m_p->parseInfo(m_info, m_metas);
}

bool AlbumInfo::parseServerError()
{
  return m_p->parseServerError(m_call->httpStatusCode(), m_info, m_error);
}

void AlbumInfo::queryImages()
{
  if (m_imagesToProcess.isEmpty())
  {
    m_cache->insertData(m_cacheDev);
    Q_EMIT finished();
  }
  else
  {
    QUrl url = m_imagesToProcess.front();
    m_imagesToProcess.pop_front();
    queryImage(url);
  }
}

void AlbumInfo::queryImage(const QUrl& url)
{
  m_call.reset(new NetRequest());
  connect(m_call.get(), SIGNAL(finished(thumbnailer::NetRequest*)), this, SLOT(processImage()));
  m_call->setUrl(url);
  m_call->launch(m_nam);
}

void AlbumInfo::fakeImage()
{
  // insert cache fake
  if (m_cacheDev == nullptr)
  {
    int days = EXPIRE_DAYS_FAILED - (EXPIRE_DAYS_FAILED/2) + (std::rand() % EXPIRE_DAYS_FAILED);
    m_cacheDev = m_cache->createData(m_cacheUrl, QDateTime::currentDateTime().addDays(days));
    m_cache->insertData(m_cacheDev);
  }
}

void AlbumInfo::processImage()
{
  if (m_call->error())
  {
    // network failure
    if (m_call->errorCode() < 200)
    {
      m_error.status = ReplyNetworkError;
      m_error.errorCode = m_call->errorCode();
      m_error.errorString = m_call->errorString();
      queryImages();
      return;
    }
    // server failure
    m_error.status = ReplyServerError;
    m_error.errorCode = m_call->errorCode();
    m_error.errorString = m_call->errorString();
    fakeImage();
    queryImages();
    return;
  }
  if (!m_call->atEnd())
  {
    m_images.push_back(m_call->readData());
    m_cacheDev->write(make_chunk_header(m_images.back()));
    m_cacheDev->write(m_images.back());
  }
  queryImages();
}
