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

#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <qglobal.h>
#include <QCoreApplication>
#include <QDebug>

#include <QDir>

class ConfigBasePrivate;

class ConfigBase
{
  public:
    class EnumFreedbLookupTransport
    {
      public:
      enum type { CDDBP, HTTP, COUNT };
    };

    ConfigBase( );
    virtual ~ConfigBase();

    void load();

    /**
      Set MusicBrainzLookupEnabled
    */
    void setMusicBrainzLookupEnabled( bool v );

    /**
      Get MusicBrainzLookupEnabled
    */
    bool musicBrainzLookupEnabled() const;

    /**
      Is MusicBrainzLookupEnabled Immutable
    */
    bool isMusicBrainzLookupEnabledImmutable() const;

    /**
      Set CacheLookupEnabled
    */
    void setCacheLookupEnabled( bool v );

    /**
      Get CacheLookupEnabled
    */
    bool cacheLookupEnabled() const;

    /**
      Is CacheLookupEnabled Immutable
    */
    bool isCacheLookupEnabledImmutable() const;

    /**
      Set cacheLocations
    */
    void setCacheLocations( const QStringList & v );

    /**
      Get cacheLocations
    */
    QStringList cacheLocations() const;

    /**
      Is cacheLocations Immutable
    */
    bool isCacheLocationsImmutable() const;

  protected:
  private:
    ConfigBasePrivate *d;
};

#endif /* CONFIGBASE_H */

