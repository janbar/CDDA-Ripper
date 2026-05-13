
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

