#ifndef BYTESCHUNK_H
#define BYTESCHUNK_H

#include <QList>
#include <QByteArray>

namespace thumbnailer
{

static bool explode_chunk_set(const QByteArray& data, QList<QByteArray>& chunks)
{
  int p = 0;
  while (p + 4 < data.size())
  {
    // read header
    quint32 sz =
            (data[p] & 0xff) << 24 |
            (data[p+1] & 0xff) << 16 |
            (data[p+2] & 0xff) << 8 |
            (data[p+3] & 0xff);
    p += 4;
    if (data.size() < int(p + sz))
      return false; // invalid chunk set
    // copy the chunk to follow
    chunks.push_back(data.mid(p, sz));
    p += sz;
  };
  return true;
}

static QByteArray make_chunk_header(const QByteArray& chunk)
{
  quint32 sz = chunk.size();
  QByteArray header(4, '\0');
  header[0] = (sz >> 24) & 0xff;
  header[1] = (sz >> 16) & 0xff;
  header[2] = (sz >> 8) & 0xff;
  header[3] = sz & 0xff;
  return header;
}

}

#endif /* BYTESCHUNK_H */

