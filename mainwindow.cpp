/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mainwindow.h"
#include "cddb/kcmcddb.h"

#include <QMenu>
#include <QWidgetAction>

class CDDATreeView : public QTreeView
{
public:
    CDDATreeView(QWidget *parent = nullptr)
        : QTreeView(parent)
    {
    }

protected:
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint) override
    {
        QTreeView::closeEditor(editor, hint);
        if ((this->currentIndex().row() < this->model()->rowCount() - 1) && (hint == QAbstractItemDelegate::SubmitModelCache)) {
            QTreeView::closeEditor(nullptr, QAbstractItemDelegate::EditNextItem);
        }
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    profile_model = new ProfileModel(this);
    if (!profile_model) {
        qDebug() << "Unable to create ProfileModel object. Low mem?";
        ErrorDialog::show(this,
                          tr("Unable to create ProfileModel object."),
                          tr("Internal error. Check your hardware. If all okay please make bug report."));
        return;
    }
    if (profile_model->lastError().isValid()) {
        ErrorDialog::show(this, profile_model->lastError().message(), profile_model->lastError().details());
        return;
    }

    firstRun();

    cdda_model = new CDDAModel(this);
    if (!cdda_model) {
        qDebug() << "Unable to create CDDAModel object. Low mem?";
        ErrorDialog::show(this, tr("Unable to create CDDAModel object."), tr("Internal error. Check your hardware. If all okay please make bug report."));
        return;
    }
    if (cdda_model->lastError().isValid()) {
        ErrorDialog::show(this, cdda_model->lastError().message(), cdda_model->lastError().details());
        return;
    }

    connect(cdda_model, SIGNAL(audioDiscDetected()), this, SLOT(new_audio_disc_detected()));
    connect(cdda_model, SIGNAL(audioDiscRemoved()), this, SLOT(audio_disc_removed()));

    connect(cdda_model, SIGNAL(cddbLookupStarted()), this, SLOT(cddb_lookup_start()));
    connect(cdda_model, SIGNAL(cddbLookupDone(const bool)), this, SLOT(cddb_lookup_done(const bool)));
    connect(cdda_model, SIGNAL(cddbDataModified()), this, SLOT(update_layout()));

    connect(profile_model, SIGNAL(profilesRemovedOrInserted()), this, SLOT(update_profile_action()));
    connect(profile_model, SIGNAL(currentProfileIndexChanged(int)), this, SLOT(update_profile_action(int)));

    // configure thumbnailer
    tbn_tmp_Dir = new TmpDir(QGuiApplication::applicationName(), "cover_fetcher");
    tbn = new thumbnailer::Thumbnailer(tbn_tmp_Dir->tmpPath(), 20 * 1024 * 1024);

    setup_actions();
    setup_layout();
    setupGUI();

    enable_layout(false);
}

bool MainWindow::firstRun()
{
    if (Preferences::firstRun()) {
        profile_model->autoCreate();
        Preferences::setFirstRun(false);
        Preferences::self()->save();
        return true;
    }

    return false;
}

MainWindow::~MainWindow()
{
    for (QAction *action : std::as_const(action_collection)) {
      if (action)
        delete action;
    }

    action_collection.clear();

    delete tbn;
    delete tbn_tmp_Dir;
    delete profile_model;
    delete cdda_model;
}

void MainWindow::eject()
{
    qDebug() << "eject requested";
    cdda_model->eject();
}

void MainWindow::cddb_lookup()
{
    cdda_model->lookupCDDB();
}

void MainWindow::rip()
{
    if (cdda_model->empty()) {
        if (QMessageBox::warning(this,
                                 tr("Disc information not found"),
                                 tr("No disc information set. Do you really want to continue?"),
                                 QMessageBox::StandardButton::No,
                                 QMessageBox::StandardButton::Yes)
            == QMessageBox::StandardButton::No)
            return;
    }

    if ((profile_model->data(profile_model->index(profile_model->currentProfileRow(), PROFILE_MODEL_COLUMN_SF_INDEX)).toBool())
        && (cdda_model->numOfAudioTracksInSelection() < cdda_model->numOfAudioTracks())) {
        if (QMessageBox::warning(this,
                                 tr("Not all audio tracks selected for single file rip"),
                                 tr("Single file rip selected but not all audio tracks to rip selected. Do you really want to continue?"),
                                 QMessageBox::StandardButton::No,
                                 QMessageBox::StandardButton::Yes)
            == QMessageBox::StandardButton::No)
            return;
    }

    auto *dialog = new ExtractingProgressDialog(profile_model, cdda_model, this);

    dialog->setWindowModality(Qt::ApplicationModal);

    dialog->exec();

    delete dialog;
}

void MainWindow::configure()
{
  Preferences::self()->showDialog(this, profile_model);
}

void MainWindow::new_audio_disc_detected()
{
    enable_layout(true);
    resizeColumns();
    if (Preferences::cddbLookupAuto()) {
        qDebug() << "Performing CDDB auto lookup";
        QTimer::singleShot(0, this, SLOT(cddb_lookup()));
    }

    update_layout();
}

void MainWindow::audio_disc_removed()
{
    enable_layout(false);

    update_layout();
}

void MainWindow::cddb_lookup_start()
{
    spinner->start();
    action_collection["fetch"]->setEnabled(false);
}

void MainWindow::cddb_lookup_done(const bool successful)
{
    spinner->stop();
    action_collection["fetch"]->setEnabled(true);

    if (!successful) {
        ErrorDialog::show(this,
                          tr("CDDB lookup failed, with the following error:\n%1").arg(cdda_model->lastError().message()),
                          cdda_model->lastError().details(),
                          tr("CDD Lookup Failure"));
    }
    update_layout();
    if (Preferences::coverLookupAuto())
        cdda_header_widget->fetchCover();
}

void MainWindow::update_layout()
{
    if (!cdda_model->isVarious()) {
        cdda_tree_view->hideColumn(CDDA_MODEL_COLUMN_ARTIST_INDEX);
    } else {
        cdda_tree_view->showColumn(CDDA_MODEL_COLUMN_ARTIST_INDEX);
    }
    resizeColumns();
    action_collection["selectall"]->setEnabled(cdda_model->selectedTracks().count() < cdda_model->numOfAudioTracks());
    action_collection["selectnone"]->setEnabled(cdda_model->selectedTracks().count() > 0);
}

void MainWindow::enable_layout(bool enabled)
{
    layout_enabled = enabled;
    cdda_tree_view->setEnabled(enabled);
    cdda_header_dock->setEnabled(enabled);
    cdda_header_widget->setEnabled(enabled);
    action_collection["profile_label"]->setEnabled((profile_model->rowCount() > 0) && (enabled));
    profile_combobox->setEnabled((profile_model->rowCount() > 0) && (enabled));
    action_collection["profile"]->setEnabled((profile_model->rowCount() > 0) && (enabled));
    action_collection["fetch"]->setEnabled(enabled);
    action_collection["eject"]->setEnabled(enabled);
    action_collection["rip"]->setEnabled(enabled);
    action_collection["splittitles"]->setEnabled(enabled);
    action_collection["swapartistsandtitles"]->setEnabled(enabled);
    action_collection["capitalize"]->setEnabled(enabled);
    action_collection["autofillartists"]->setEnabled(enabled);
    action_collection["selectall"]->setEnabled(enabled);
    action_collection["selectnone"]->setEnabled(enabled);
    action_collection["invertselection"]->setEnabled(enabled);
}

void MainWindow::current_profile_updated_from_ui(int row)
{
    if (row >= 0) {
        profile_model->blockSignals(true);
        profile_model->setRowAsCurrentProfileIndex(row);
        profile_model->blockSignals(false);
    }
}

void MainWindow::update_profile_action(int index)
{
    if (index == -1) {
        if (layout_enabled) {
            action_collection["profile_label"]->setEnabled(false);
            action_collection["profile"]->setEnabled(false);
        }
    } else {
        if (layout_enabled) {
            action_collection["profile_label"]->setEnabled(true);
            action_collection["profile"]->setEnabled(true);
        }
        profile_combobox->setCurrentIndex(profile_model->getRowByIndex(index));
    }
}

void MainWindow::update_profile_action()
{
    // When the Profile model emits 'reset' the profile combo clears its current settings.
    // Therefore, we need to try and reset these...
    if (profile_combobox->currentText().isEmpty()) {
        profile_combobox->setCurrentIndex(profile_model->currentProfileRow());
    }

    if (layout_enabled) {
        action_collection["profile_label"]->setEnabled(profile_model->rowCount() > 0);
        action_collection["profile"]->setEnabled(profile_model->rowCount() > 0);
    }
}

void MainWindow::split_titles()
{
    bool ok;
    QString divider = QInputDialog::getText(this,
                                            tr("Split titles"),
                                            tr("Please set a divider string. Be aware of empty spaces.\n\nDivider:"),
                                            QLineEdit::Normal,
                                            " - ",
                                            &ok);
    if (ok && !divider.isEmpty()) {
        cdda_model->splitTitleOfTracks(divider);
    }
}

void MainWindow::swap_artists_and_titles()
{
    if (QMessageBox::warning(this,
                             tr("Swap artists and titles"),
                             tr("Do you really want to swap all artists and titles?"),
                             QMessageBox::StandardButton::No,
                             QMessageBox::StandardButton::Yes)
        == QMessageBox::StandardButton::No)
        return;

    cdda_model->swapArtistAndTitle();
    cdda_model->swapArtistAndTitleOfTracks();
}

void MainWindow::capitalize()
{
    if (QMessageBox::warning(this,
                             tr("Capitalize artists and titles"),
                             tr("Do you really want to capitalize all artists and titles?"),
                             QMessageBox::StandardButton::No,
                             QMessageBox::StandardButton::Yes)
        == QMessageBox::StandardButton::No)
        return;

    cdda_model->capitalizeHeader();
    cdda_model->capitalizeTracks();
}

void MainWindow::auto_fill_artists()
{
    if (QMessageBox::warning(this,
                             tr("Autofill artists"),
                             tr("Do you really want to autofill track artists?"),
                             QMessageBox::StandardButton::No,
                             QMessageBox::StandardButton::Yes)
        == QMessageBox::StandardButton::No)
        return;

    cdda_model->setTitleArtistsFromHeader();
}

void MainWindow::toggle(const QModelIndex &idx)
{
    if (idx.isValid() && (idx.column() == CDDA_MODEL_COLUMN_RIP_INDEX)) {
        cdda_model->toggle(idx.row());
        cdda_tree_view->update(idx);
    }
}

void MainWindow::resizeColumns()
{
    for (int i = 0; i < CDDA_MODEL_COLUMN_COUNT; ++i)
        cdda_tree_view->resizeColumnToContents(i);
}

void MainWindow::setup_actions()
{
    auto *ejectAction = new QAction(this);
    ejectAction->setText(tr("Eject"));
    ejectAction->setIcon(QIcon(QPixmap(":/icons/media-eject.png")));
    action_collection["eject"] = ejectAction;
    //actionCollection()->setDefaultShortcut(ejectAction, Qt::CTRL + Qt::Key_E);
    connect(ejectAction, SIGNAL(triggered(bool)), this, SLOT(eject()));

    profile_label = new QLabel(this);
    profile_label->setText(tr("Profile:"));
    profile_combobox = new QComboBox(this);
    profile_combobox->setModel(profile_model);
    profile_combobox->setModelColumn(1);
    profile_combobox->setMinimumWidth(80);
    profile_combobox->setMaximumWidth(220);
    profile_combobox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    profile_combobox->resize(QSize(220, profile_combobox->height()));
    profile_combobox->setCurrentIndex(profile_model->currentProfileRow());
    connect(profile_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(current_profile_updated_from_ui(int)));

    auto *plabelAction = new QWidgetAction(this);
    plabelAction->setText(tr("Profile:"));
    plabelAction->setDefaultWidget(profile_label);
    profile_label->setBuddy(profile_combobox);
    action_collection["profile_label"] = plabelAction;

    auto *profileAction = new QWidgetAction(this);
    profileAction->setText(tr("Profile"));
    profileAction->setDefaultWidget(profile_combobox);
    action_collection["profile"] = profileAction;
    //actionCollection()->setDefaultShortcut(profileAction, Qt::Key_F6);
    //actionCollection()->setShortcutsConfigurable(profileAction, false);
    update_profile_action();

    auto *cddbLookupAction = new QAction(this);
    cddbLookupAction->setText(tr("Fetch Data"));
    cddbLookupAction->setIcon(QIcon(QPixmap(":/icons/media-load.png")));
    action_collection["fetch"] = cddbLookupAction;
    //actionCollection()->setDefaultShortcut(cddbLookupAction, Qt::CTRL + Qt::Key_F);
    connect(cddbLookupAction, SIGNAL(triggered(bool)), this, SLOT(cddb_lookup()));

    auto *extractAction = new QAction(this);
    extractAction->setText(tr("Rip"));
    extractAction->setIcon(QIcon(QPixmap(":/icons/media-import.png")));
    action_collection["rip"] = extractAction;
    //actionCollection()->setDefaultShortcut(extractAction, Qt::CTRL + Qt::Key_X);
    connect(extractAction, SIGNAL(triggered(bool)), this, SLOT(rip()));

    auto *configureAction = new QAction(this);
    configureAction->setText(tr("Preferences"));
    configureAction->setIcon(QIcon(QPixmap(":/icons/emblem-system.png")));
    action_collection["preferences"] = configureAction;
    connect(configureAction, SIGNAL(triggered(bool)), this, SLOT(configure()));

    auto *splitTitlesAction = new QAction(this);
    splitTitlesAction->setText(tr("Split Titles"));
    splitTitlesAction->setIcon(QIcon(QPixmap(":/icons/emblem-system.png")));
    action_collection["splittitles"] = splitTitlesAction;
    connect(splitTitlesAction, SIGNAL(triggered(bool)), this, SLOT(split_titles()));

    auto *swapArtistsAndTitlesAction = new QAction(this);
    swapArtistsAndTitlesAction->setText(tr("Swap Artists And Titles"));
    swapArtistsAndTitlesAction->setIcon(QIcon(QPixmap(":/icons/emblem-system.png")));
    action_collection["swapartistsandtitles"] = swapArtistsAndTitlesAction;
    connect(swapArtistsAndTitlesAction, SIGNAL(triggered(bool)), this, SLOT(swap_artists_and_titles()));

    auto *capitalizeAction = new QAction(this);
    capitalizeAction->setText(tr("Capitalize"));
    capitalizeAction->setIcon(QIcon(QPixmap(":/icons/emblem-system.png")));
    action_collection["capitalize"] = capitalizeAction;
    connect(capitalizeAction, SIGNAL(triggered(bool)), this, SLOT(capitalize()));

    auto *autoFillArtistsAction = new QAction(this);
    autoFillArtistsAction->setText(tr("Auto Fill Artists"));
    autoFillArtistsAction->setIcon(QIcon(QPixmap(":/icons/emblem-system.png")));
    action_collection["autofillartists"] = autoFillArtistsAction;
    connect(autoFillArtistsAction, SIGNAL(triggered(bool)), this, SLOT(auto_fill_artists()));

    auto *selectAllAction = new QAction(this);
    selectAllAction->setText(tr("Select All Tracks"));
    action_collection["selectall"] = selectAllAction;
    connect(selectAllAction, SIGNAL(triggered(bool)), this, SLOT(select_all()));

    auto *selectNoneAction = new QAction(this);
    selectNoneAction->setText(tr("Deselect All Tracks"));
    action_collection["selectnone"] = selectNoneAction;
    connect(selectNoneAction, SIGNAL(triggered(bool)), this, SLOT(select_none()));

    auto *invertSelectionAction = new QAction(this);
    invertSelectionAction->setText(tr("Invert Selection"));
    action_collection["invertselection"] = invertSelectionAction;
    connect(invertSelectionAction, SIGNAL(triggered(bool)), this, SLOT(invert_selection()));

    auto *quitAction = new QAction(this);
    quitAction->setText(tr("Quit"));
    quitAction->setIcon(QIcon(QPixmap(":/icons/quit.png")));
    action_collection["quit"] = quitAction;
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(onQuit()));

    auto *aboutAction = new QAction(this);
    aboutAction->setText(tr("About"));
    aboutAction->setIcon(QIcon(QPixmap(":/icons/emblem-question.png")));
    action_collection["about"] = aboutAction;
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(showAbout()));
}

void MainWindow::setup_layout()
{
    cdda_tree_view = new CDDATreeView(this);
    cdda_tree_view->setModel(cdda_model);
    cdda_tree_view->setAlternatingRowColors(true);
    cdda_tree_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    cdda_tree_view->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
    cdda_tree_view->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    cdda_tree_view->setIndentation(0);
    cdda_tree_view->setAllColumnsShowFocus(true);
    cdda_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(cdda_tree_view, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(cdda_context_menu(const QPoint &)));
    connect(cdda_tree_view, SIGNAL(clicked(const QModelIndex &)), SLOT(toggle(const QModelIndex &)));
    connect(cdda_model, SIGNAL(selectionChanged(const int)), this, SLOT(selection_changed(const int)));

    spinner = new WaitingSpinnerWidget(cdda_tree_view, true, true);
    spinner->setColor(QApplication::palette().color(QPalette::Disabled, QPalette::WindowText));
    spinner->setInnerRadius(25);
    spinner->setLineLength(25);
    spinner->setLineWidth(10);
    spinner->setRoundness(20.0);
    spinner->setNumberOfLines(10);
    spinner->setRevolutionsPerSecond(0.8);
    spinner->setMinimumTrailOpacity(15.0);
    spinner->setTrailFadePercentage(70.0);

    cdda_header_dock = new QDockWidget(tr("Album Data"), this);
    cdda_header_dock->setObjectName("cdda_header_dock");
    cdda_header_dock->setFeatures(QDockWidget::DockWidgetMovable);
    cdda_header_dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    setCentralWidget(cdda_tree_view);
    cdda_header_widget = new CDDAHeaderWidget(*tbn, cdda_model, cdda_header_dock);
    connect(cdda_header_widget, SIGNAL(headerDataChanged()), this, SLOT(update_layout()));
    cdda_header_dock->setWidget(cdda_header_widget);

    switch (Preferences::guiLayout())
    {
    case 1:
      addDockWidget(Qt::LeftDockWidgetArea, cdda_header_dock);
      break;
    case 2:
      addDockWidget(Qt::TopDockWidgetArea, cdda_header_dock);
      break;
    case 3:
      addDockWidget(Qt::RightDockWidgetArea, cdda_header_dock);
      break;
    default:
      addDockWidget(Qt::BottomDockWidgetArea, cdda_header_dock);
    }
    connect(cdda_header_dock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(onDockLocationChanged(Qt::DockWidgetArea)));

    statusBar()->hide();
    statusBar()->setMaximumHeight(0);
}

void MainWindow::setupGUI()
{
  setMinimumWidth(800);
  setMinimumHeight(600);
  resize(Preferences::guiWidth(), Preferences::guiHeight());

  QMenu * file = menuBar()->addMenu(tr("Action"));
  file->setMinimumWidth(150);
  file->addAction(action("eject"));
  file->addSeparator();
  file->addAction(action("rip"));
  file->addSeparator();
  file->addAction(action("about"));
  file->addAction(action("quit"));

  QMenu * cddb = menuBar()->addMenu(tr("CDDB"));
  cddb->setMinimumWidth(200);
  cddb->addAction(action("fetch"));
  cddb->addSeparator();
  QMenu * cddb_tools = cddb->addMenu(QIcon(QPixmap(":/icons/emblem-system.png")), tr("Title Correction Tools").append("   "));
  cddb_tools->addAction(action("splittitles"));
  cddb_tools->addAction(action("swapartistsandtitles"));
  cddb_tools->addAction(action("capitalize"));
  cddb_tools->addAction(action("autofillartists"));

  QMenu * settings = menuBar()->addMenu(tr("Settings"));
  settings->setMinimumWidth(200);
  settings->addAction(action("preferences"));

  QToolBar * main_toolbar = addToolBar(tr("Tools"));
  main_toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  main_toolbar->addAction(action("eject"));
  main_toolbar->addSeparator();
  main_toolbar->addAction(action("profile_label"));
  main_toolbar->addAction(action("profile"));
  main_toolbar->addSeparator();
  main_toolbar->addAction(action("fetch"));
  main_toolbar->addSeparator();
  main_toolbar->addAction(action("rip"));
}

void MainWindow::select_all()
{
    cdda_model->selectAll();
}

void MainWindow::select_none()
{
    cdda_model->selectNone();
}

void MainWindow::invert_selection()
{
    cdda_model->invertSelection();
}

void MainWindow::cdda_context_menu(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu(this);
    menu.addAction(action_collection["selectall"]);
    menu.addAction(action_collection["selectnone"]);
    menu.addSeparator();
    menu.addAction(action_collection["invertselection"]);
    menu.exec(QCursor::pos());
}

void MainWindow::selection_changed(const int num_selected)
{
    action_collection["rip"]->setEnabled(num_selected > 0);
    action_collection["selectall"]->setEnabled(num_selected < cdda_model->numOfAudioTracks());
    action_collection["selectnone"]->setEnabled(num_selected > 0);
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    event->ignore();
    Preferences::setGuiWidth(event->size().width());
    Preferences::setGuiHeight(event->size().height());
}

void MainWindow::onDockLocationChanged(Qt::DockWidgetArea area)
{
  switch (area)
  {
  case Qt::LeftDockWidgetArea:
      Preferences::setGuiLayout(1);
      break;
  case Qt::TopDockWidgetArea:
      Preferences::setGuiLayout(2);
      break;
  case Qt::RightDockWidgetArea:
      Preferences::setGuiLayout(3);
      break;
  default:
      Preferences::setGuiLayout(0);
  }
}

void MainWindow::onQuit()
{
  this->close();
}

void MainWindow::showAbout()
{
  QMessageBox::about(this, "", tr(
          "<html>An easy to use audio CD ripping application.<br>"
          "<br><b>Version %1</b>, (2023) Jean-Luc Barrière<br>"
          "<br>CDDA-Ripper was forked from Audex-0.95 in 2023 and reworked outside of the KDE frameworks.<br>"
          "<br>Qt version %2.%3.%4<br>"
          "<br><b>Code base:</b>"
          "<br>Audex-0.95 (Marco Nelles)"
          "<br>Solid (kde.org)"
          "<br>Kcddb (kde.org)"
          "<br>Thumbnailler (Jean-Luc Barrière)"
          "<br>GoodStyle (Juergen Skrotzky)"
          "</html>")
          .arg(CDDARIPPER_VERSION).arg(QT_VERSION_MAJOR).arg(QT_VERSION_MINOR).arg(QT_VERSION_PATCH));
}
