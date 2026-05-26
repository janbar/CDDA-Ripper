/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "cddaheaderwidget.h"
#include "dialogs/cddaheaderdatadialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMimeData>
#include <QScreen>
#include <QStandardPaths>

CDDAHeaderWidget ::CDDAHeaderWidget(thumbnailer::Thumbnailer& tbn, CDDAModel *cddaModel, QWidget *parent, const int coverSize, const int padding)
    : QWidget(parent), _tbn(tbn)
{
    cdda_model = cddaModel;
    if (!cdda_model) {
        qDebug() << "CDDAModel is NULL!";
        return;
    }
    connect(cdda_model, SIGNAL(modelReset()), this, SLOT(update()));

    setup_actions();

    qDebug() << "coverSize:" << coverSize;
    this->cover_size = coverSize;

    this->i_cover_checksum = 1;

    this->padding = padding;

    scale_factor = 1.0;
    opacity_factor = 1.0;

    setMouseTracking(true);
    cursor_on_cover = false;
    cursor_on_link1 = false;

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(context_menu(const QPoint &)));

    cover_browser_dialog = nullptr;
    fetching_cover_in_progress = false;

    setContextMenuPolicy(Qt::CustomContextMenu);

    setMinimumSize(QSize(cover_size + (padding * 2), cover_size + (padding * 2)));

    tmp_dir = new TmpDir(QGuiApplication::applicationName(), "cover");

    update();

    setAcceptDrops(true);
}

CDDAHeaderWidget::~CDDAHeaderWidget()
{
    for (QAction *action : std::as_const(action_collection)) {
      if (action)
        delete action;
    }

    action_collection.clear();

    delete tmp_dir;
}

QSize CDDAHeaderWidget::sizeHint() const
{
    return QSize((int)(cover_size * 1.5) + (padding * 2), (int)(cover_size * 1.0) + (padding * 2));
}

void CDDAHeaderWidget::setCover(CachedImage *cover)
{
    if (cover) {
        i_cover_checksum = cover->checksum();
    } else {
        i_cover_checksum = 0;
    }

    if (i_cover.isNull()) {
        if (cover) {
            i_cover = cover->coverImage().convertToFormat(QImage::Format_ARGB32_Premultiplied);
        } else {
            QImage image = QImage(":/images/nocover.png");
            i_cover = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        }
        scale_factor = 1.0;
        opacity_factor = 0.0;
    } else {
        if (cover) {
            i_cover = cover->coverImage().convertToFormat(QImage::Format_ARGB32_Premultiplied);
        } else {
            QImage image = QImage(":/images/nocover.png");
            i_cover = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        }
        scale_factor = 1.0;
        opacity_factor = 1.0;
    }

    repaint();
}

bool CDDAHeaderWidget::isEnabled() const
{
    return enabled;
}

void CDDAHeaderWidget::setEnabled(bool enabled)
{
    this->enabled = enabled;
    repaint();
}

void CDDAHeaderWidget::fetchCover()
{
    qDebug() << "AUTO cover fetch";

    if ((cdda_model->empty()) || (fetching_cover_in_progress))
        return;

    QApplication::restoreOverrideCursor();
    cursor_on_cover = false;
    fetching_cover_in_progress = true;
    action_collection["fetch"]->setEnabled(false);

    cover_browser_dialog = new CoverBrowserDialog(_tbn, this);

    connect(cover_browser_dialog, SIGNAL(allCoverThumbnailsFetched()), this, SLOT(fetch_first_cover()));
    connect(cover_browser_dialog, SIGNAL(nothingFetched()), this, SLOT(auto_fetch_cover_failed()));

    QString artist = cdda_model->artist();
    QString title = cdda_model->title();
    int lastColonPos = title.lastIndexOf(':');
    while (lastColonPos > 0) {
        title = title.left(lastColonPos);
        lastColonPos = title.lastIndexOf(':');
    }
    cover_browser_dialog->fetchThumbnails(artist, title);
}

void CDDAHeaderWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p;

    p.begin(this);

    if (enabled) {
        bool vertical = rect().height() > rect().width() && rect().width() < ((cover_size + padding) * 2);
        int xOffset = vertical ? padding : (padding * 2) + cover_size, yOffset = vertical ? (padding * 2) + cover_size : padding;

        QImage scaled_cover = i_cover.scaled((int)(scale_factor * cover_size), (int)(scale_factor * cover_size), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        p.drawImage((cover_size / 2 - (scaled_cover.width() / 2)) + padding, (cover_size / 2 - (scaled_cover.height() / 2)) + padding, scaled_cover);
        cover_rect = QRect(padding, padding, scaled_cover.width(), scaled_cover.height());

        p.setBrush(palette().text());

        QFont font(QApplication::font());
        int pixelSize = font.pixelSize();
        if (pixelSize == -1) {
            QScreen *screen = QApplication::screens().at(0);
            pixelSize = (font.pointSize() * (qreal)screen->logicalDotsPerInch() + 36) / 72;
        }
        int width = rect().width() - (xOffset + 1) - padding;
        font.setPixelSize((int)((((double)pixelSize) * 1.5) + 0.5));
        font.setBold(true);
        p.setFont(font);
        yOffset += p.fontMetrics().lineSpacing() * 1.2;
        p.drawText(xOffset, yOffset, p.fontMetrics().elidedText(cdda_model->artist(), Qt::ElideRight, width));

        font.setPixelSize(pixelSize);
        font.setBold(true);
        font.setItalic(true);
        p.setFont(font);
        yOffset += pixelSize;
        p.drawText(xOffset, yOffset, p.fontMetrics().elidedText(cdda_model->title(), Qt::ElideRight, width));

        yOffset += p.fontMetrics().lineSpacing() * 1.5;
        font.setBold(false);
        font.setItalic(false);
        p.setFont(font);

        QFontMetrics fm(font);
        QString yearLabel(tr("Released: ")), genreLabel(tr("Genre: ")), cdNoLabel(tr("CD Number: "));
        int maxWidth(fm.horizontalAdvance(yearLabel));

        if ((width = fm.horizontalAdvance(genreLabel)) > maxWidth)
            maxWidth = width;
        if (cdda_model->isMultiCD() && (width = fm.horizontalAdvance(cdNoLabel)))
            maxWidth = width;

        width = rect().width() - (xOffset + 1);

        if (!cdda_model->year().isEmpty()) {
            p.drawText(xOffset, yOffset, yearLabel);
            p.drawText(xOffset + maxWidth, yOffset, fm.elidedText(cdda_model->year(), Qt::ElideRight, width - maxWidth));
            yOffset += fm.lineSpacing();
        }
        if (!cdda_model->genre().isEmpty()) {
            p.drawText(xOffset, yOffset, genreLabel);
            p.drawText(xOffset + maxWidth, yOffset, fm.elidedText(cdda_model->genre(), Qt::ElideRight, width - maxWidth));
            yOffset += fm.lineSpacing();
        }
        if (cdda_model->isMultiCD()) {
            p.drawText(xOffset, yOffset, cdNoLabel);
            p.drawText(xOffset + maxWidth, yOffset, QString().setNum(cdda_model->cdNum()));
            yOffset += fm.lineSpacing();
        }

        font.setUnderline(true);
        p.setFont(font);

        // links

        fm = QFontMetrics(font);
        QString link1 = tr("Edit Data");
        p.setPen(QApplication::palette().color(QPalette::Active, QPalette::Link));

        link1_rect = fm.boundingRect(link1);

        yOffset = vertical ? yOffset + fm.lineSpacing() : (yOffset > (padding + cover_size) ? yOffset : (padding + cover_size));
        p.drawText(xOffset, yOffset, link1);
        link1_rect = QRect(xOffset, yOffset + link1_rect.y(), link1_rect.width(), link1_rect.height());

    } else { // disabled

        QFont font(QApplication::font());
        if (-1 == font.pixelSize()) {
            font.setPointSizeF(font.pointSizeF() * 1.5);
        } else {
            font.setPixelSize(font.pixelSize() * 1.5);
        }
        font.setBold(true);
        font.setItalic(true);
        p.setFont(font);
        p.drawText(rect(), Qt::AlignCenter | Qt::AlignVCenter, tr("No audio CD detected"));
    }

    p.end();
}

void CDDAHeaderWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (cover_rect.contains(event->pos())) {
        if (!cursor_on_cover) {
            QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
            cursor_on_cover = true;
        }

    } else if (link1_rect.contains(event->pos())) {
        if (!cursor_on_link1) {
            QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
            cursor_on_link1 = true;
        }

    } else {
        if (cursor_on_cover) {
            QApplication::restoreOverrideCursor();
            cursor_on_cover = false;
        } else if (cursor_on_link1) {
            QApplication::restoreOverrideCursor();
            cursor_on_link1 = false;
        }
    }
}

void CDDAHeaderWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if ((cursor_on_cover) && (!fetching_cover_in_progress)) {
            if (cdda_model->isCoverEmpty()) {
                if (cdda_model->empty()) {
                    load();
                } else {
                    thumbnailer();
                }
            } else {
                view_cover();
            }
        }
        if (cursor_on_link1)
            edit_data();
    }
}

void CDDAHeaderWidget::update()
{
    action_collection["fetch"]->setEnabled(!cdda_model->empty());

    bool activate = false;
    if (cdda_model->isCoverEmpty()) {
        if (i_cover_checksum)
            setCover(nullptr);
    } else {
        qDebug() << "current cover checksum:" << i_cover_checksum;
        qDebug() << "new cover checksum:" << cdda_model->coverChecksum();
        if (i_cover_checksum != cdda_model->coverChecksum())
            setCover(cdda_model->cover());
        activate = true;
    }

    action_collection["save"]->setEnabled(activate);
    action_collection["view"]->setEnabled(activate);
    action_collection["remove"]->setEnabled(activate);

    repaint();
}

void CDDAHeaderWidget::thumbnailer()
{
    qDebug() << "Cover fetch";

    if ((cdda_model->empty()) || (fetching_cover_in_progress))
        return;

    QApplication::restoreOverrideCursor();
    cursor_on_cover = false;
    fetching_cover_in_progress = true;
    action_collection["fetch"]->setEnabled(false);

    cover_browser_dialog = new CoverBrowserDialog(_tbn, this);

    connect(cover_browser_dialog, SIGNAL(coverFetched(const QByteArray &)), this, SLOT(set_cover(const QByteArray &)));
    connect(cover_browser_dialog, SIGNAL(nothingFetched()), this, SLOT(fetch_cover_failed()));

    QString artist = cdda_model->artist();
    QString title = cdda_model->title();
    int lastColonPos = title.lastIndexOf(':');
    while (lastColonPos > 0) {
        title = title.left(lastColonPos);
        lastColonPos = title.lastIndexOf(':');
    }
    cover_browser_dialog->fetchThumbnails(artist, title);

    if (cover_browser_dialog->exec() != QDialog::Accepted) {
        fetching_cover_in_progress = false;
        delete cover_browser_dialog;
        cover_browser_dialog = nullptr;
        action_collection["fetch"]->setEnabled(true);
    }
}

void CDDAHeaderWidget::load()
{
    qDebug() << "Supported cover image file MIME types:" << cdda_model->coverSupportedMimeTypeList();
    QString filename = QFileDialog::getOpenFileName(this, tr("Load Cover"), QDir::homePath(), cdda_model->coverSupportedMimeTypeList());
    if (!filename.isEmpty()) {
        if (!cdda_model->setCover(filename)) {
            ErrorDialog::show(this, cdda_model->lastError().message(), cdda_model->lastError().details());
        }
    }
}

void CDDAHeaderWidget::save()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Cover"), QDir::homePath() + '/' + cdda_model->title() + ".jpg", cdda_model->coverSupportedMimeTypeList());
    if (!filename.isEmpty()) {
        if (!cdda_model->saveCoverToFile(filename)) {
            ErrorDialog::show(this, cdda_model->lastError().message(), cdda_model->lastError().details());
        }
    }
}

void CDDAHeaderWidget::view_cover()
{
    QString tmp_path = tmp_dir->tmpPath();
    if (tmp_dir->error()) {
        QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
        tmp_path = dirs.size() ? dirs[0] : "/var/tmp/";
        if (tmp_path.right(1) != "/")
            tmp_path += "/";
        qDebug() << "Temporary folder in use:" << tmp_path;
    }

    QString filename = tmp_path + QString("%1.jpeg").arg(cdda_model->coverChecksum());
    cdda_model->saveCoverToFile(filename);

    QDesktopServices::openUrl(QUrl(filename));
}

void CDDAHeaderWidget::remove()
{
    cdda_model->clearCover();
    update();
}

void CDDAHeaderWidget::edit_data()
{
    QApplication::restoreOverrideCursor();
    cursor_on_link1 = false;

    auto *dialog = new CDDAHeaderDataDialog(cdda_model, this);

    if (dialog->exec() != QDialog::Accepted) {
        delete dialog;
        return;
    }
    delete dialog;
    update();
    Q_EMIT headerDataChanged();
}

void CDDAHeaderWidget::set_cover(const QByteArray &cover)
{
    if (!cover.isEmpty())
        cdda_model->setCover(cover);
    fetching_cover_in_progress = false;
    action_collection["fetch"]->setEnabled(true);
    if (cover_browser_dialog) {
        delete cover_browser_dialog;
        cover_browser_dialog = nullptr;
    }
    if (!cover.isEmpty())
        update();
}

void CDDAHeaderWidget::fetch_first_cover()
{
    if (cover_browser_dialog) {
        if (cover_browser_dialog->count() == 0) {
            qDebug() << "no cover found";
            ErrorDialog::show(this, tr("No cover found."), tr("Check your artist name and title. Otherwise you can load a custom cover from an image file."));
            delete cover_browser_dialog;
            cover_browser_dialog = nullptr;
            fetching_cover_in_progress = false;
            action_collection["fetch"]->setEnabled(true);
        } else {
            connect(cover_browser_dialog, SIGNAL(coverFetched(const QByteArray &)), this, SLOT(set_cover(const QByteArray &)));
            cover_browser_dialog->startFetchCover(0);
        }
    }
}

void CDDAHeaderWidget::fetchCoverFinished(bool showDialog)
{
    fetching_cover_in_progress = false;
    action_collection["fetch"]->setEnabled(true);
    if (cover_browser_dialog) {
        cover_browser_dialog->close();
        cover_browser_dialog->deleteLater();
        cover_browser_dialog = nullptr;
        if (showDialog) {
            ErrorDialog::show(this, tr("No cover found."), tr("Check your artist name and title. Otherwise you can load a custom cover from an image file."));
        }
    }
}

void CDDAHeaderWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void CDDAHeaderWidget::dropEvent(QDropEvent *event)
{
    if (event->source() == this)
    {
        event->dropAction();
        return;
    }

    // fetch urls until a valid image
    for (const QUrl& url : event->mimeData()->urls())
    {
        if (url.isLocalFile())
        {
            QString filepath = url.toLocalFile();
            if (QPixmap(filepath).isNull())
               continue;
            event->acceptProposedAction();
            if (!cdda_model->setCover(filepath))
                ErrorDialog::show(this, cdda_model->lastError().message(), cdda_model->lastError().details());
            return;
        }
    }
    event->dropAction();
}

void CDDAHeaderWidget::auto_fetch_cover_failed()
{
    fetchCoverFinished(false);
}

void CDDAHeaderWidget::fetch_cover_failed()
{
    fetchCoverFinished(true);
}

void CDDAHeaderWidget::context_menu(const QPoint &point)
{
    qDebug() << "context menu requested at point" << point;
    if ((cursor_on_cover) && (!fetching_cover_in_progress)) {
        QApplication::restoreOverrideCursor();
        cursor_on_cover = false;
        QMenu contextMenu(this);
        contextMenu.clear();
        contextMenu.addAction(action_collection["fetch"]);
        contextMenu.addAction(action_collection["load"]);
        contextMenu.addAction(action_collection["save"]);
        contextMenu.addSeparator();
        contextMenu.addAction(action_collection["view"]);
        contextMenu.addSeparator();
        contextMenu.addAction(action_collection["remove"]);
        contextMenu.exec(this->mapToGlobal(point));
    }
}

void CDDAHeaderWidget::setup_actions()
{
    auto *fetchCoverAction = new QAction(this);
    fetchCoverAction->setText(tr("Fetch cover from Network..."));
    action_collection["fetch"] = fetchCoverAction;
    connect(fetchCoverAction, SIGNAL(triggered(bool)), this, SLOT(thumbnailer()));

    auto *loadCoverAction = new QAction(this);
    loadCoverAction->setText(tr("Set Custom Cover..."));
    action_collection["load"] = loadCoverAction;
    connect(loadCoverAction, SIGNAL(triggered(bool)), this, SLOT(load()));

    auto *saveCoverAction = new QAction(this);
    saveCoverAction->setText(tr("Save Cover To File..."));
    action_collection["save"] = saveCoverAction;
    connect(saveCoverAction, SIGNAL(triggered(bool)), this, SLOT(save()));

    auto *viewCoverAction = new QAction(this);
    viewCoverAction->setText(tr("Show Full Size Cover..."));
    action_collection["view"] = viewCoverAction;
    connect(viewCoverAction, SIGNAL(triggered(bool)), this, SLOT(view_cover()));

    auto *removeCoverAction = new QAction(this);
    removeCoverAction->setText(tr("Remove Cover"));
    action_collection["remove"] = removeCoverAction;
    connect(removeCoverAction, SIGNAL(triggered(bool)), this, SLOT(remove()));
}
