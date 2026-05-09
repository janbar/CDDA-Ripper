/*
    SPDX-FileCopyrightText: 2007 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kcddb.h"

#include <QObject>

namespace KCDDB
{
    QString
  resultToString(Result r)
  {
    switch (r)
    {
      case Success:
        return QObject::tr("Success");
        break;

      case ServerError:
        return QObject::tr("Server error");
        break;

      case HostNotFound:
        return QObject::tr("Host not found");
        break;

      case NoResponse:
        return QObject::tr("No response");
        break;

      case NoRecordFound:
        return QObject::tr("No record found");
        break;

      case MultipleRecordFound:
        return QObject::tr("Multiple records found");
        break;

      case CannotSave:
        return QObject::tr("Cannot save");
        break;

      case InvalidCategory:
        return QObject::tr("Invalid category");
        break;

      default:
        return QObject::tr("Unknown error");
        break;
    }
  }
}
