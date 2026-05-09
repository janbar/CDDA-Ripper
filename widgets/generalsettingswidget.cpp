/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "generalsettingswidget.h"

#include <QStandardPaths>

generalSettingsWidget::generalSettingsWidget(QWidget *parent)
    : generalSettingsWidgetUI(parent)
{
    kcfg_basePath->setInputMethodHints(Qt::InputMethodHint::ImhUrlCharactersOnly);
    kcfg_basePath->setObjectName("kcfg_basePath");
}

generalSettingsWidget::~generalSettingsWidget()
{
}
