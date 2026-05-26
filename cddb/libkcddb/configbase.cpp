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

#include "configbase.h"
#include "config-musicbrainz.h"

class ConfigBasePrivate
{
  public:

    // Lookup
    bool musicBrainzLookupEnabled;
    bool cacheLookupEnabled;
    QStringList cacheLocations;

    // Submit
    QString emailAddress;
};

ConfigBase::ConfigBase( )
{
  d = new ConfigBasePrivate;
  d->musicBrainzLookupEnabled = false;
  d->cacheLookupEnabled = false;
}

void ConfigBase::load()
{
#ifdef HAVE_MUSICBRAINZ5
  setMusicBrainzLookupEnabled(true);
#endif
}

void ConfigBase::setMusicBrainzLookupEnabled( bool v )
{
    if (!ConfigBase::isMusicBrainzLookupEnabledImmutable())
      d->musicBrainzLookupEnabled = v;
}

bool ConfigBase::musicBrainzLookupEnabled() const
{
  return d->musicBrainzLookupEnabled;
}

bool  ConfigBase::isMusicBrainzLookupEnabledImmutable() const
{
    return false;
}

void ConfigBase::setCacheLookupEnabled( bool v )
{
    if (!ConfigBase::isCacheLookupEnabledImmutable())
      d->cacheLookupEnabled = v;
}

bool ConfigBase::cacheLookupEnabled() const
{
  return d->cacheLookupEnabled;
}

bool  ConfigBase::isCacheLookupEnabledImmutable() const
{
    return false;
}

void ConfigBase::setCacheLocations( const QStringList & v )
{
    if (!ConfigBase::isCacheLocationsImmutable())
      d->cacheLocations = v;
}

QStringList ConfigBase::cacheLocations() const
{
  return d->cacheLocations;
}

bool  ConfigBase::isCacheLocationsImmutable() const
{
    return false;
}

ConfigBase::~ConfigBase()
{
  delete d;
}

