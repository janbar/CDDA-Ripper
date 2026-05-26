/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "coverbrowserdialog.h"
#include "preferences.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

CoverBrowserDialog::CoverBrowserDialog(thumbnailer::Thumbnailer& tbn, QWidget *parent)
    : QDialog(parent), cover_fetcher(tbn)
{
    Q_UNUSED(parent);

    setup();
}

CoverBrowserDialog::~CoverBrowserDialog()
{
}

void CoverBrowserDialog::fetchThumbnails(const QString &artist, const QString &title, int fetchCount)
{
    if (fetchCount == 0)
        cover_fetcher.startFetchThumbnails(artist, title, Preferences::fetchCount());
    else
        cover_fetcher.startFetchThumbnails(artist, title, fetchCount);
    ui.label->setText(tr("Searching for covers..."));
}

void CoverBrowserDialog::startFetchCover(int no)
{
    cover_fetcher.startFetchCover(no);
}

void CoverBrowserDialog::slotAccepted()
{
    select_this(ui.listWidget->selectedItems().at(0));
    accept();
}

void CoverBrowserDialog::select_this(QListWidgetItem *item)
{
    cover_fetcher.stopFetchThumbnails();
    int match = item->data(Qt::UserRole).toInt();
    cover_fetcher.startFetchCover(match);
    accept();
}

void CoverBrowserDialog::enable_select_button()
{
    okButton->setEnabled(ui.listWidget->selectedItems().count() > 0);
}

void CoverBrowserDialog::add_item(const QByteArray &cover, const QString &caption, int no)
{
    QPixmap pixmap;
    if (pixmap.loadFromData(cover)) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(caption);
        // item->setToolTip(tr("%1\nCover Size: %2x%3").arg(caption, pixmap.width(), pixmap.height()));
        item->setIcon(QIcon(pixmap.scaled(128, 128, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        item->setData(Qt::UserRole, no - 1);
        ui.listWidget->addItem(item);
    }
    ui.label->setText(tr("Fetching Thumbnail %1 / %2...").arg(no).arg(cover_fetcher.count()));
}

void CoverBrowserDialog::all_fetched()
{
    ui.label->setText(tr("Found %n Cover(s)", "", cover_fetcher.count()));
    Q_EMIT allCoverThumbnailsFetched();
}

void CoverBrowserDialog::nothing_fetched()
{
    ui.label->setText(tr("No Covers Found"));
    Q_EMIT nothingFetched();
}

void CoverBrowserDialog::cover_fetched(const QByteArray &cover)
{
    Q_EMIT coverFetched(cover);
}

void CoverBrowserDialog::error(const QString &description, const QString &solution)
{
    ErrorDialog::show(this, description, solution);
    Q_EMIT nothingFetched();
}

void CoverBrowserDialog::setup()
{
    static const int constIconSize = 128;

    setWindowTitle(tr("Fetch Cover From Network"));

    auto *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CoverBrowserDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CoverBrowserDialog::reject);

    QWidget *widget = new QWidget(this);
    mainLayout->addWidget(widget);
    mainLayout->addWidget(buttonBox);
    ui.setupUi(widget);

    connect(&cover_fetcher, SIGNAL(fetchedThumbnail(const QByteArray &, const QString &, int)), this, SLOT(add_item(const QByteArray &, const QString &, int)));
    connect(&cover_fetcher, SIGNAL(allCoverThumbnailsFetched()), this, SLOT(all_fetched()));
    connect(&cover_fetcher, SIGNAL(nothingFetched()), this, SLOT(nothing_fetched()));
    connect(&cover_fetcher, SIGNAL(fetchedCover(const QByteArray &)), this, SLOT(cover_fetched(const QByteArray &)));
    connect(&cover_fetcher, SIGNAL(error(const QString &, const QString &)), this, SLOT(error(const QString &, const QString &)));

    ui.listWidget->setIconSize(QSize(constIconSize, constIconSize));
    ui.listWidget->setWordWrap(true);
    ui.listWidget->setViewMode(QListView::IconMode);
    connect(ui.listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(enable_select_button()));
    connect(ui.listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(select_this(QListWidgetItem *)));
    ui.listWidget->setMinimumSize((constIconSize + 12) * 4, (constIconSize + 12) * 2);
    enable_select_button();
}
