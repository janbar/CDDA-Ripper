CDDA-Ripper
===========

CDDA-Ripper is a fork of Audex 0.95, an audio grabber tool for CD-ROM drives.
It was reworked outside of the KDE frameworks, initially required by Audex.

Some extra features have been removed, and other added or fixed. The main goal
is to provide an even easier-to-use tool. Data are fetched from MusicBrainz,
and covers are downloaded from the DEEZER services. Alternatively, you can drag
and drop the cover of your choice.

### Ubuntu package

You can install CDDA-Ripper on your Ubuntu desktop by the following:

```
sudo add-apt-repository ppa:jlbarriere68/cddaripper
sudo apt-get update
sudo apt-get install cddaripper
```

### Prerequisites

* cdparanoia
* MusicBrainz library version 5.x
* udev
* udisk2

### Prerequisites (optional)

* Ogg Vorbis (ogg files)
* Ogg Opus (opus files)
* FLAC (flac files)
* LAME (mp3 files)
* FAAC (mp4/aac files)
* eyeD3 (mp3 tagging)

### Compile with Qt 6.9
```
mkdir build
cd build
cmake -DQT_VERSION_PREFERRED=6 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
make
(sudo) make install
```

### Compile with Qt 5.15
```
mkdir build
cd build
cmake -DQT_VERSION_PREFERRED=5 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
make
(sudo) make install
```

