/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QDockWidget>
#include <QInputDialog>
#include <QLabel>
#include <QObject>
#include <QTreeView>
#include <QMap>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>

#include <QComboBox>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QStatusBar>

#include <cddb/cdinfo.h>
#include <cddb/client.h>
#include <cddb/kcddb.h>

#include "utils/cuesheetwriter.h"
#include "utils/error.h"

#include "models/cddamodel.h"
#include "models/profilemodel.h"

#include "preferences.h"
#include "widgets/cddaheaderwidget.h"

#include "dialogs/errordialog.h"
#include "dialogs/extractingprogressdialog.h"

#include "utils/encoderassistant.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    bool firstRun();
    QAction * action(const QString& name) { return action_collection[name]; }

    void resizeEvent(QResizeEvent *event) override;

private Q_SLOTS:
    void eject();
    void cddb_lookup();
    void rip();
    void configure();

    void new_audio_disc_detected();
    void audio_disc_removed();

    void cddb_lookup_start();
    void cddb_lookup_done(const bool successful);

    void update_layout();

    void enable_layout(bool enabled);

    void current_profile_updated_from_ui(int row);
    void update_profile_action(int index);
    void update_profile_action();

    void split_titles();
    void swap_artists_and_titles();
    void capitalize();
    void auto_fill_artists();
    void toggle(const QModelIndex &idx);
    void resizeColumns();

    void select_all();
    void select_none();
    void invert_selection();

    void cdda_context_menu(const QPoint &pos);

    void selection_changed(const int num_selected);

    void onQuit();
    void onDockLocationChanged(Qt::DockWidgetArea area);
    void showAbout();

private:
    CDDAModel *cdda_model;
    ProfileModel *profile_model;

    QLabel *profile_label;
    QComboBox *profile_combobox;
    QMap<QString, QAction*> action_collection;

    void setup_actions();
    void setup_layout();
    void setupGUI();

    QTreeView *cdda_tree_view;

    QDockWidget *cdda_header_dock;
    CDDAHeaderWidget *cdda_header_widget;

    bool layout_enabled;

    int current_profile_index;
    void set_profile(int profile_index);

    TmpDir* tbn_tmp_Dir;
    thumbnailer::Thumbnailer* tbn;
};

#endif
