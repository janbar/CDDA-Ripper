/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CDDAHEADERWIDGET_H
#define CDDAHEADERWIDGET_H

#include <QApplication>
#include <QDesktopServices>
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <QMap>
#include <QAction>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "dialogs/coverbrowserdialog.h"
#include "dialogs/errordialog.h"
#include "models/cddamodel.h"
#include "utils/cachedimage.h"
#include "utils/tmpdir.h"
#include "thumbnailer/thumbnailer.h"

class CDDAHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDDAHeaderWidget(thumbnailer::Thumbnailer& tbn, CDDAModel *cddaModel, QWidget *parent = nullptr, const int coverSize = 192, const int padding = 20);
    ~CDDAHeaderWidget() override;
    QSize sizeHint() const override;
    void setCover(CachedImage *cover);

    bool isEnabled() const;

public Q_SLOTS:
    void setEnabled(bool enabled);

    void fetchCover();

Q_SIGNALS:
    void headerDataChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void fetchCoverFinished(bool showDialog);

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private Q_SLOTS:

    void update();

    void thumbnailer();
    void load();
    void save();
    void view_cover();
    void remove();

    void edit_data();

    void set_cover(const QByteArray &cover);
    void fetch_first_cover();
    void fetch_cover_failed();
    void auto_fetch_cover_failed();

    void context_menu(const QPoint &point);

private:
    CDDAModel *cdda_model;
    QMap<QString, QAction*> action_collection;
    int cover_size;
    int padding;

    quint32 i_cover_checksum;
    QImage i_cover;

    qreal scale_factor;
    qreal opacity_factor;

    QRect cover_rect;
    bool cursor_on_cover;

    QRect link1_rect;
    bool cursor_on_link1;

    void setup_actions();

    bool enabled;

    bool fetching_cover_in_progress;
    CoverBrowserDialog *cover_browser_dialog;

    TmpDir *tmp_dir;
    thumbnailer::Thumbnailer& _tbn;
};

#endif
