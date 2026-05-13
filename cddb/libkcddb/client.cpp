/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002-2005 Benjamin C. Meyer <ben at meyerhome dot net>
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "client.h"

#include "cache.h"
#include "logging.h"
#include "lookup.h"

#include "config-musicbrainz.h"
#ifdef HAVE_MUSICBRAINZ5
#include "musicbrainz/musicbrainzlookup.h"
#include "musicbrainz/asyncmusicbrainzlookup.h"
#endif

namespace KCDDB
{
  class Client::Private
  {
    public:

      Private()
        : cdInfoLookup(nullptr),
          block( true )
      {}

      ~Private()
      {
        delete cdInfoLookup;
        qDeleteAll(pendingLookups);
      }

      Lookup * cdInfoLookup;

      Config config;
      CDInfoList cdInfoList;
      TrackOffsetList trackOffsetList;
      QList<Lookup *> pendingLookups;
      bool block;
  };

  Client::Client()
    : d(new Private)
  {
    d->config.load();
  }

  Client::~Client()
  {
    delete d;
  }

    Config &
  Client::config() const
  {
    return d->config;
  }

    void
  Client::setBlockingMode( bool enable )
  {
    d->block = enable;
  }

    bool
  Client::blockingMode() const
  {
    return d->block;
  }

    CDInfoList
  Client::lookupResponse() const
  {
    return d->cdInfoList;
  }

    Result
  Client::lookup(const TrackOffsetList & trackOffsetList)
  {
    d->cdInfoList.clear();
    d->trackOffsetList = trackOffsetList;

    if ( trackOffsetList.count() <= 1 )
    {
      qCDebug(LIBKCDDB) << "Lookup called with empty offset list";
      return NoRecordFound;
    }

    if ( d->config.cacheLookupEnabled() )
    {
      d->cdInfoList = Cache::lookup( trackOffsetList, config() );

      qCDebug(LIBKCDDB) << "Found " << d->cdInfoList.count() << " hit(s)";

      if ( !d->cdInfoList.isEmpty() )
      {
        if ( !blockingMode() )
          Q_EMIT finished( Success );

        return Success;
      }
    }

    Result r = NoRecordFound;

    // just in case we have an info lookup hanging around, prevent mem leakage
    delete d->cdInfoLookup;
    d->cdInfoLookup = nullptr;
    qDeleteAll(d->pendingLookups);
    d->pendingLookups.clear();

    if ( blockingMode() )
    {
#ifdef HAVE_MUSICBRAINZ5
      if ( d->config.musicBrainzLookupEnabled() )
      {
        d->cdInfoLookup = new MusicBrainzLookup();

        r = d->cdInfoLookup->lookup( trackOffsetList );

        if ( Success == r )
        {
          d->cdInfoList = d->cdInfoLookup->lookupResponse();
          Cache::store( d->trackOffsetList, d->cdInfoList, config() );

          return r;
        }

        delete d->cdInfoLookup;
        d->cdInfoLookup = nullptr;
      }
#endif

      return r;
    }
    else
    {
#ifdef HAVE_MUSICBRAINZ5
      if ( d->config.musicBrainzLookupEnabled() )
      {
        AsyncMusicBrainzLookup* lookup = new AsyncMusicBrainzLookup();

        connect( lookup, &AsyncMusicBrainzLookup::finished,
                 this, &Client::slotFinished );
        d->pendingLookups.append( lookup );
      }
#endif

      return runPendingLookups();
    }
  }

    void
  Client::slotFinished( Result r )
  {
    if ( d->cdInfoLookup && Success == r )
    {
      d->cdInfoList = d->cdInfoLookup->lookupResponse();
      Cache::store( d->trackOffsetList, d->cdInfoList, config() );
    }
    else
      d->cdInfoList.clear();

    if ( d->cdInfoLookup ) // in case someone called lookup() while finished() was being processed, and deleted cdInfoLookup.
    {
      d->cdInfoLookup->deleteLater();
      d->cdInfoLookup = nullptr;
    }

    if ( Success == r )
    {
      Q_EMIT finished( r );
      qDeleteAll( d->pendingLookups );
      d->pendingLookups.clear();
    }
    else
    {
      runPendingLookups();
    }
  }

    Result
  Client::runPendingLookups()
  {
    if (!d->pendingLookups.empty())
    {
      d->cdInfoLookup = d->pendingLookups.takeFirst();

      Result r = d->cdInfoLookup->lookup( d->trackOffsetList );

      if ( Success != r )
      {
        delete d->cdInfoLookup;
        d->cdInfoLookup = nullptr;
      }

      return r;
    }
    else
    {
      Q_EMIT finished( NoRecordFound );
      return NoRecordFound;
    }
  }

    void
  Client::store(const CDInfo &cdInfo, const TrackOffsetList& offsetList)
  {
    Cache::store(offsetList, cdInfo, config());
  }
}

#include "moc_client.cpp"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
