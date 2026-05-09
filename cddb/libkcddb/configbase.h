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

