/*
    SPDX-FileCopyrightText: 2005 Shaheed Haque <srhaque@iee.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "genres.h"

#include <QObject>

class KCDDB::Genres::Private
{
public:
    QStringList cddb;
    QStringList i18n;
};

KCDDB::Genres::Genres()
    : d(new Private)
{
    // The Genre is completely arbitrary. But we follow kaudiocreator's cue
    // and make life easy for people.
    //
    // To cope with preexisting records which don't match an entry, we will
    // add one later if needed.
    d->cddb << QLatin1String( "Unknown" ) << QLatin1String( "A Cappella" ) << QLatin1String( "Acid Jazz" ) <<
            QLatin1String( "Acid Punk" ) << QLatin1String( "Acid" ) << QLatin1String( "Acoustic" ) << QLatin1String( "Alternative" ) <<
            QLatin1String( "Alt. Rock" ) << QLatin1String( "Ambient" ) << QLatin1String( "Anime" ) << QLatin1String( "Avantgarde" ) <<
            QLatin1String( "Ballad" ) << QLatin1String( "Bass" ) << QLatin1String( "Beat" ) << QLatin1String( "Bebop" ) <<
            QLatin1String( "Big Band" ) << QLatin1String( "Black Metal" ) << QLatin1String( "Bluegrass" ) << QLatin1String( "Blues" ) <<
            QLatin1String( "Booty Bass" ) << QLatin1String( "BritPop" ) << QLatin1String( "Cabaret" ) << QLatin1String( "Celtic" ) <<
            QLatin1String( "Chamber Music" ) << QLatin1String( "Chanson" ) << QLatin1String( "Chorus" ) << QLatin1String( "Christian Gangsta Rap" ) <<
            QLatin1String( "Christian Rap" ) << QLatin1String( "Christian Rock" ) << QLatin1String( "Classical" ) << QLatin1String( "Classic Rock" ) <<
            QLatin1String( "Club-house" ) << QLatin1String( "Club" ) << QLatin1String( "Comedy" ) << QLatin1String( "Contemporary Christian" ) <<
            QLatin1String( "Country" ) << QLatin1String( "Crossover" ) << QLatin1String( "Cult" ) << QLatin1String( "Dance Hall" ) <<
            QLatin1String( "Dance" ) << QLatin1String( "Darkwave" ) << QLatin1String( "Death Metal" ) << QLatin1String( "Disco" ) <<
            QLatin1String( "Dream" ) << QLatin1String( "Drum & Bass" ) << QLatin1String( "Drum Solo" ) << QLatin1String( "Duet" ) <<
            QLatin1String( "Easy Listening" ) << QLatin1String( "Electronic" ) << QLatin1String( "Ethnic" ) << QLatin1String( "Eurodance" ) <<
            QLatin1String( "Euro-House" ) << QLatin1String( "Euro-Techno" ) << QLatin1String( "Fast-Fusion" ) << QLatin1String( "Folklore" ) <<
            QLatin1String( "Folk/Rock" ) << QLatin1String( "Folk" ) << QLatin1String( "Freestyle" ) << QLatin1String( "Funk" ) <<
            QLatin1String( "Fusion" ) << QLatin1String( "Game" ) << QLatin1String( "Gangsta Rap" ) << QLatin1String( "Goa" ) <<
            QLatin1String( "Gospel" ) << QLatin1String( "Gothic Rock" ) << QLatin1String( "Gothic" ) << QLatin1String( "Grunge" ) <<
            QLatin1String( "Hardcore" ) << QLatin1String( "Hard Rock" ) << QLatin1String( "Heavy Metal" ) << QLatin1String( "Hip-Hop" ) <<
            QLatin1String( "House" ) << QLatin1String( "Humor" ) << QLatin1String( "Indie" ) << QLatin1String( "Industrial" ) <<
            QLatin1String( "Instrumental Pop" ) << QLatin1String( "Instrumental Rock" ) << QLatin1String( "Instrumental" ) << QLatin1String( "Jazz+Funk" ) <<
            QLatin1String( "Jazz" ) << QLatin1String( "JPop" ) << QLatin1String( "Jungle" ) << QLatin1String( "Latin" ) << QLatin1String( "Lo-Fi" ) <<
            QLatin1String( "Meditative" ) << QLatin1String( "Merengue" ) << QLatin1String( "Metal" ) << QLatin1String( "Musical" ) <<
            QLatin1String( "National Folk" ) << QLatin1String( "Native American" ) << QLatin1String( "Negerpunk" ) << QLatin1String( "New Age" ) <<
            QLatin1String( "New Wave" ) << QLatin1String( "Noise" ) << QLatin1String( "Oldies" ) << QLatin1String( "Opera" ) <<
            QLatin1String( "Other" ) << QLatin1String( "Polka" ) << QLatin1String( "Polsk Punk" ) << QLatin1String( "Pop-Funk" ) <<
            QLatin1String( "Pop/Funk" ) << QLatin1String( "Pop" ) << QLatin1String( "Porn Groove" ) << QLatin1String( "Power Ballad" ) <<
            QLatin1String( "Pranks" ) << QLatin1String( "Primus" ) << QLatin1String( "Progressive Rock" ) << QLatin1String( "Psychedelic Rock" ) <<
            QLatin1String( "Psychedelic" )<< QLatin1String( "Punk Rock" ) << QLatin1String( "Punk" ) << QLatin1String( "R&B" ) <<
            QLatin1String( "Rap" ) << QLatin1String( "Rave" ) << QLatin1String( "Reggae" ) << QLatin1String( "Retro" ) <<
            QLatin1String( "Revival" ) << QLatin1String( "Rhythmic Soul" ) << QLatin1String( "Rock & Roll" ) << QLatin1String( "Rock" ) <<
            QLatin1String( "Salsa" ) << QLatin1String( "Samba" ) << QLatin1String( "Satire" ) << QLatin1String( "Showtunes" ) <<
            QLatin1String( "Ska" ) << QLatin1String( "Slow Jam" ) << QLatin1String( "Slow Rock" ) << QLatin1String( "Sonata" ) <<
            QLatin1String( "Soul" ) << QLatin1String( "Sound Clip" ) << QLatin1String( "Soundtrack" ) << QLatin1String( "Southern Rock" ) <<
            QLatin1String( "Space" ) << QLatin1String( "Speech" ) << QLatin1String( "Swing" ) << QLatin1String( "Symphonic Rock" ) <<
            QLatin1String( "Symphony" ) << QLatin1String( "Synthpop" ) << QLatin1String( "Tango" ) << QLatin1String( "Techno-Industrial" ) <<
            QLatin1String( "Techno" ) << QLatin1String( "Terror" ) << QLatin1String( "Thrash Metal" ) << QLatin1String( "Top 40" ) <<
            QLatin1String( "Trailer" ) << QLatin1String( "Trance" ) << QLatin1String( "Tribal" ) << QLatin1String( "Trip-Hop" ) <<
            QLatin1String( "Vocal" );
    d->i18n << QObject::tr("Unknown") << QObject::tr("A Cappella") << QObject::tr("Acid Jazz") <<
            QObject::tr("Acid Punk") << QObject::tr("Acid") << QObject::tr("Acoustic") << QObject::tr("Alternative") <<
            QObject::tr("Alt. Rock") << QObject::tr("Ambient") << QObject::tr("Anime") << QObject::tr("Avantgarde") <<
            QObject::tr("Ballad") << QObject::tr("Bass") << QObject::tr("Beat") << QObject::tr("Bebop") <<
            QObject::tr("Big Band") << QObject::tr("Black Metal") << QObject::tr("Bluegrass") << QObject::tr("Blues") <<
            QObject::tr("Booty Bass") << QObject::tr("BritPop") << QObject::tr("Cabaret") << QObject::tr("Celtic") <<
            QObject::tr("Chamber Music") << QObject::tr("Chanson") << QObject::tr("Chorus") << QObject::tr("Christian Gangsta Rap") <<
            QObject::tr("Christian Rap") << QObject::tr("Christian Rock") << QObject::tr("Classical") << QObject::tr("Classic Rock") <<
            QObject::tr("Club-house") << QObject::tr("Club") << QObject::tr("Comedy") << QObject::tr("Contemporary Christian") <<
            QObject::tr("Country") << QObject::tr("Crossover") << QObject::tr("Cult") << QObject::tr("Dance Hall") <<
            QObject::tr("Dance") << QObject::tr("Darkwave") << QObject::tr("Death Metal") << QObject::tr("Disco") <<
            QObject::tr("Dream") << QObject::tr("Drum & Bass") << QObject::tr("Drum Solo") << QObject::tr("Duet") <<
            QObject::tr("Easy Listening") << QObject::tr("Electronic") << QObject::tr("Ethnic") << QObject::tr("Eurodance") <<
            QObject::tr("Euro-House") << QObject::tr("Euro-Techno") << QObject::tr("Fast-Fusion") << QObject::tr("Folklore") <<
            QObject::tr("Folk/Rock") << QObject::tr("Folk") << QObject::tr("Freestyle") << QObject::tr("Funk") <<
            QObject::tr("Fusion") << QObject::tr("Game") << QObject::tr("Gangsta Rap") << QObject::tr("Goa") <<
            QObject::tr("Gospel") << QObject::tr("Gothic Rock") << QObject::tr("Gothic") << QObject::tr("Grunge") <<
            QObject::tr("Hardcore") << QObject::tr("Hard Rock") << QObject::tr("Heavy Metal") << QObject::tr("Hip-Hop") <<
            QObject::tr("House") << QObject::tr("Humor") << QObject::tr("Indie") << QObject::tr("Industrial") <<
            QObject::tr("Instrumental Pop") << QObject::tr("Instrumental Rock") << QObject::tr("Instrumental") << QObject::tr("Jazz+Funk") <<
            QObject::tr("Jazz") << QObject::tr("JPop") << QObject::tr("Jungle") << QObject::tr("Latin") << QObject::tr("Lo-Fi") <<
            QObject::tr("Meditative") << QObject::tr("Merengue") << QObject::tr("Metal") << QObject::tr("Musical") <<
            QObject::tr("National Folk") << QObject::tr("Native American") << QObject::tr("Negerpunk") << QObject::tr("New Age") <<
            QObject::tr("New Wave") << QObject::tr("Noise") << QObject::tr("Oldies") << QObject::tr("Opera") <<
            QObject::tr("Other") << QObject::tr("Polka") << QObject::tr("Polsk Punk") << QObject::tr("Pop-Funk") <<
            QObject::tr("Pop/Funk") << QObject::tr("Pop") << QObject::tr("Porn Groove") << QObject::tr("Power Ballad") <<
            QObject::tr("Pranks") << QObject::tr("Primus") << QObject::tr("Progressive Rock") << QObject::tr("Psychedelic Rock") <<
            QObject::tr("Psychedelic") << QObject::tr("Punk Rock") << QObject::tr("Punk") << QObject::tr("R&B") <<
            QObject::tr("Rap") << QObject::tr("Rave") << QObject::tr("Reggae") << QObject::tr("Retro") <<
            QObject::tr("Revival") << QObject::tr("Rhythmic Soul") << QObject::tr("Rock & Roll") << QObject::tr("Rock") <<
            QObject::tr("Salsa") << QObject::tr("Samba") << QObject::tr("Satire") << QObject::tr("Showtunes") <<
            QObject::tr("Ska") << QObject::tr("Slow Jam") << QObject::tr("Slow Rock") << QObject::tr("Sonata") <<
            QObject::tr("Soul") << QObject::tr("Sound Clip") << QObject::tr("Soundtrack") << QObject::tr("Southern Rock") <<
            QObject::tr("Space") << QObject::tr("Speech") << QObject::tr("Swing") << QObject::tr("Symphonic Rock") <<
            QObject::tr("Symphony") << QObject::tr("Synthpop") << QObject::tr("Tango") << QObject::tr("Techno-Industrial") <<
            QObject::tr("Techno") << QObject::tr("Terror") << QObject::tr("Thrash Metal") << QObject::tr("Top 40") <<
            QObject::tr("Trailer") << QObject::tr("Trance") << QObject::tr("Tribal") << QObject::tr("Trip-Hop") <<
            QObject::tr("Vocal");
}

KCDDB::Genres::~Genres()
{
    delete d;
}

KCDDB::Genres::Genres(const Genres& other)
    :d(new Private)
{
    *d = *other.d;
}

KCDDB::Genres& KCDDB::Genres::operator=(const Genres& other)
{
    *d = *other.d;
    return *this;
}

const QStringList &KCDDB::Genres::cddbList() const
{
    return d->cddb;
}

const QStringList &KCDDB::Genres::i18nList() const
{
    return d->i18n;
}

const QString KCDDB::Genres::cddb2i18n(const QString &genre) const
{
    QString userDefinedGenre = genre.trimmed();
    int index = d->cddb.indexOf(userDefinedGenre);
    if (index != -1)
    {
        return d->i18n[index];
    }
    else
    {
        return userDefinedGenre;
    }
}

const QString KCDDB::Genres::i18n2cddb(const QString &genre) const
{
    QString userDefinedGenre = genre.trimmed();
    int index = d->i18n.indexOf(userDefinedGenre);
    if (index != -1)
    {
        return d->cddb[index];
    }
    else
    {
        return userDefinedGenre;
    }
}
