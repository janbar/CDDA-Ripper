/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "profilewidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QIcon>

profileWidget::profileWidget(ProfileModel *profileModel, QWidget *parent)
    : profileWidgetUI(parent)
{
    profile_model = profileModel;
    if (!profile_model) {
        qDebug() << "ProfileModel is NULL!";
        return;
    }

    listView->setModel(profile_model);
    listView->setModelColumn(1);
    connect(listView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(_update()));
    connect(listView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(mod_profile(const QModelIndex &)));
    connect(kpushbutton_add, SIGNAL(clicked()), this, SLOT(add_profile()));
    connect(kpushbutton_rem, SIGNAL(clicked()), this, SLOT(rem_profile()));
    connect(kpushbutton_mod, SIGNAL(clicked()), this, SLOT(mod_profile()));
    connect(kpushbutton_copy, SIGNAL(clicked()), this, SLOT(copy_profile()));
    connect(kpushbutton_load, SIGNAL(clicked()), this, SLOT(load_profiles()));
    connect(kpushbutton_save, SIGNAL(clicked()), this, SLOT(save_profiles()));
    connect(kpushbutton_init, SIGNAL(clicked()), this, SLOT(init_profiles()));

    kpushbutton_add->setIcon(QIcon(QPixmap(":/icons/emblem-list-add.png")));
    kpushbutton_rem->setIcon(QIcon(QPixmap(":/icons/emblem-list-delete.png")));

    kpushbutton_load->setIcon(QIcon(QPixmap(":/icons/document-open.png")));
    kpushbutton_save->setIcon(QIcon(QPixmap(":/icons/document-save.png")));

    kpushbutton_init->setIcon(QIcon(QPixmap(":/icons/emblem-synchronizing.png")));

    _update();
}

profileWidget::~profileWidget()
{
}

void profileWidget::_update()
{
    kpushbutton_rem->setEnabled(listView->selectionModel()->selectedIndexes().count() > 0);
    kpushbutton_mod->setEnabled(listView->selectionModel()->selectedIndexes().count() > 0);
    kpushbutton_copy->setEnabled(listView->selectionModel()->selectedIndexes().count() > 0);
    kpushbutton_save->setEnabled(profile_model->rowCount() > 0);
}

void profileWidget::add_profile()
{
    auto *dialog = new ProfileDataDialog(profile_model, -1, this);

    if (dialog->exec() != QDialog::Accepted) {
        delete dialog;
        return;
    }
    delete dialog;

    profile_model->sortItems();

    _update();
}

void profileWidget::rem_profile()
{
    if (QMessageBox::warning(
            this,
            tr("Delete profile"),
            tr("Do you really want to delete profile \"%1\"?").arg(profile_model->data(profile_model->index(listView->currentIndex().row(), PROFILE_MODEL_COLUMN_NAME_INDEX)).toString()),
            QMessageBox::StandardButton::Ok,
            QMessageBox::StandardButton::Cancel)
        != QMessageBox::StandardButton::Ok)
        return;

    QModelIndex ci = listView->currentIndex();
    profile_model->removeRows(ci.row(), 1);

    profile_model->commit();

    if (ci.isValid())
        listView->setCurrentIndex(ci);

    _update();
}

void profileWidget::mod_profile(const QModelIndex &index)
{
    auto *dialog = new ProfileDataDialog(profile_model, index.row(), this);

    dialog->exec();

    delete dialog;

    _update();
}

void profileWidget::mod_profile()
{
    mod_profile(listView->currentIndex());
}

void profileWidget::copy_profile()
{
    profile_model->copy(listView->currentIndex().row());
    profile_model->commit();
    profile_model->sortItems();
    _update();
}

void profileWidget::save_profiles()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Cover"), QDir::homePath(), "*.apf");
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".apf"))
            filename.append(".apf");
        profile_model->saveProfilesToFile(filename);
    }
}

void profileWidget::load_profiles()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load Profiles"), QDir::homePath(), "*.apf");
    if (!filename.isEmpty()) {
        profile_model->loadProfilesFromFile(filename);
    }
}

void profileWidget::init_profiles()
{
    if (QMessageBox::StandardButton::Ok ==
        QMessageBox::question(this,
                              tr("Codec Scan"),
                              tr("<p>Do you wish to rescan your system for codecs (Lame, Ogg Vorbis, Flac, etc.)?</p>"
                                   "<p><font style=\"font-style:italic;\">This will attempt to create some sample profiles based upon any found codecs.</font></p>"),
                              QMessageBox::StandardButton::Ok,
                              QMessageBox::Cancel)) {
        int sizeBefore = profile_model->rowCount();
        profile_model->autoCreate();
        int diff = profile_model->rowCount() - sizeBefore;
        QMessageBox::information(this, tr("Codec Scan"),
                                 0 == diff ? tr("No new codecs found")
                                           : tr("%n new profile(s) added", "", diff));
    }
}
