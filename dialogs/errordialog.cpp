// SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
// SPDX-License-Identifier: GPL-3.0-or-later

#include "errordialog.h"

#include <QMessageBox>

namespace ErrorDialog
{
void show(QWidget *parent, const QString &message, const QString &details, const QString &caption)
{
    if (details.isEmpty()) {
        QMessageBox::critical(parent, caption, message);
    } else {
        QMessageBox::critical(parent, caption, "<p>" + message + "</p><p>" + details + "</p>");
    }
}

}
