/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "extractingprogressdialog.h"

ExtractingProgressDialog::ExtractingProgressDialog(ProfileModel *profile_model, CDDAModel *cdda_model, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Rip And Encode"));
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ExtractingProgressDialog::slotCancel);

    QWidget *widget = new QWidget(this);
    mainLayout->addWidget(widget);
    mainLayout->addWidget(buttonBox);
    ui.setupUi(widget);

    this->profile_model = profile_model;
    this->cdda_model = cdda_model;

    QString title = QString("%1 - %2").arg(cdda_model->artist(), cdda_model->title());
    ui.label_header->setText(title);

    p_single_file = profile_model->data(profile_model->index(profile_model->currentProfileRow(), PROFILE_MODEL_COLUMN_SF_INDEX)).toBool();

    if (p_single_file) {
        ui.label_extracting->setText(tr("Ripping whole CD as single track"));
        ui.label_encoding->setText(tr("Encoding"));

    } else {
        ui.label_extracting->setText(tr("Ripping Track 0 of %1").arg(cdda_model->numOfAudioTracks()));
        ui.label_encoding->setText(tr("Encoding Track 0 of %1").arg(cdda_model->numOfAudioTracks()));
    }

    audex = new Audex(this, profile_model, cdda_model);

    connect(audex, SIGNAL(error(const QString &, const QString &)), this, SLOT(show_error(const QString &, const QString &)));
    connect(audex, SIGNAL(warning(const QString &)), this, SLOT(show_warning(const QString &)));
    connect(audex, SIGNAL(info(const QString &)), this, SLOT(show_info(const QString &)));
    connect(audex, SIGNAL(finished(bool)), this, SLOT(conclusion(bool)));
    connect(audex, SIGNAL(speedEncode(double)), this, SLOT(show_speed_encode(double)));
    connect(audex, SIGNAL(speedExtract(double)), this, SLOT(show_speed_extract(double)));
    connect(audex, SIGNAL(progressExtractTrack(int)), this, SLOT(show_progress_extract_track(int)));
    connect(audex, SIGNAL(progressExtractOverall(int)), this, SLOT(show_progress_extract_overall(int)));
    connect(audex, SIGNAL(progressEncodeTrack(int)), this, SLOT(show_progress_encode_track(int)));
    connect(audex, SIGNAL(progressEncodeOverall(int)), this, SLOT(show_progress_encode_overall(int)));
    connect(audex, SIGNAL(changedExtractTrack(int, int, const QString &, const QString &)), this, SLOT(show_changed_extract_track(int, int, const QString &, const QString &)));
    connect(audex, SIGNAL(changedEncodeTrack(int, int, const QString &)), this, SLOT(show_changed_encode_track(int, int, const QString &)));
    connect(audex, SIGNAL(timeout()), this, SLOT(ask_timeout()));
    connect(ui.details_button, SIGNAL(pressed()), this, SLOT(toggle_details()));

    canceled = false;
    finished = false;

    progressbar_np_flag = false;
}

ExtractingProgressDialog::~ExtractingProgressDialog()
{
    delete audex;
}

int ExtractingProgressDialog::exec()
{
    resize(600, 400);
    current_extract_overall = 0;
    current_encode_overall = 0;
    ui.details_button->setArrowType(Qt::UpArrow);
    toggle_details();
    show();
    setModal(true);
    if (audex->prepare()) {
        audex->start();
    }
    int rv = QDialog::exec();

    return rv;
}

void ExtractingProgressDialog::calc_overall_progress()
{
    ui.progressBar_overall->setValue((int)(((float)(current_extract_overall + current_encode_overall) / 2.0f) + .5f));
}

void ExtractingProgressDialog::toggle_details()
{
    if (Qt::UpArrow == ui.details_button->arrowType()) {
        ui.details_button->setArrowType(Qt::DownArrow);
        ui.details->setVisible(false);
        ui.label_overall->setVisible(false);
        ui.label_overall_track->setVisible(true);
        ui.progressBar_overall->setVisible(true);
        resize(width(), 32);

    } else {
        ui.details_button->setArrowType(Qt::UpArrow);
        ui.details->setVisible(true);
        ui.label_overall_track->setVisible(false);

        if (cdda_model->numOfAudioTracksInSelection() < 2) {
            ui.label_overall->setVisible(false);
            ui.progressBar_overall->setVisible(false);
        } else {
            ui.label_overall->setVisible(true);
        }
        resize(width(), 400);
    }
}

void ExtractingProgressDialog::slotCancel()
{
    cancel();
}

void ExtractingProgressDialog::slotClose()
{
    close();
}

void ExtractingProgressDialog::slotEncoderProtocol()
{
    open_encoder_protocol_dialog();
}

void ExtractingProgressDialog::slotExtractProtocol()
{
    open_extract_protocol_dialog();
}

void ExtractingProgressDialog::cancel()
{
    if (finished) {
        close();

    } else if (canceled) {
        qWarning() << "Force a stop by ejecting the media.";
        cdda_model->eject();

    } else {
        if (QMessageBox::warning(
                this,
                tr("Cancel"),
                tr("Do you really want to cancel extraction?"),
                QMessageBox::StandardButton::No,
                QMessageBox::StandardButton::Yes)
            == QMessageBox::StandardButton::Yes) {

            lock.lock(); // prevent thread competition
            QPushButton* button = buttonBox->button(QDialogButtonBox::Cancel);
            if (button)
                button->setEnabled(false);
            lock.unlock();

            audex->cancel();
            canceled = true;
            // enable emergency stop after 5 seconds
            QTimer::singleShot(5000, this, SLOT(activateEmergencyStop()));
        }
    }
}

void ExtractingProgressDialog::show_changed_extract_track(int no, int total, const QString &artist, const QString &title)
{
    Q_UNUSED(artist);
    Q_UNUSED(title);

    if (!p_single_file) {
        ui.label_extracting->setText((1 == total) ? tr("Ripping Track") : tr("Ripping Track %1 of %2").arg(no).arg(total));
        ui.label_overall_track->setText((1 == total) ? tr("Overall Progress") : tr("Overall Progress (Ripping Track %1 of %2)").arg(no).arg(total));
        current_track = no;

    } else {
        ui.label_extracting->setText(tr("Ripping whole CD as single track"));
        ui.label_overall_track->setText(tr("Overall Progress"));
    }
}

void ExtractingProgressDialog::show_changed_encode_track(int no, int total, const QString &filename)
{
    Q_UNUSED(filename);

    if (no == 0) {
        ui.label_encoding->setText("<i>" + tr("Waiting for an encoding job...") + "</i>");
        ui.label_speed_encoding->clear();
    } else {
        if (!p_single_file)
            ui.label_encoding->setText((1 == total) ? tr("Encoding Track") : tr("Encoding Track %1 of %2").arg(no).arg(total));
    }
}

void ExtractingProgressDialog::show_progress_extract_track(int percent)
{
    ui.progressBar_extracting->setValue(percent);
}

void ExtractingProgressDialog::show_progress_extract_overall(int percent)
{
    current_extract_overall = percent;
    calc_overall_progress();
}

void ExtractingProgressDialog::show_progress_encode_track(int percent)
{
    if (percent >= 0) {
        ui.progressBar_encoding->setValue(percent);
        if (progressbar_np_flag) {
            ui.progressBar_encoding->setRange(0, 100);
            ui.progressBar_encoding->setTextVisible(true);
            progressbar_np_flag = false;
        }
    } else {
        if (!progressbar_np_flag) {
            progressbar_np_flag = true;
            ui.progressBar_encoding->setRange(0, 0);
            ui.progressBar_encoding->setTextVisible(false);
        }
    }
}

void ExtractingProgressDialog::show_progress_encode_overall(int percent)
{
    current_encode_overall = percent;
    calc_overall_progress();
}

void ExtractingProgressDialog::show_speed_encode(double speed)
{
    QString s = QString("%1").arg((double)speed, 0, 'f', 2);
    ui.label_speed_encoding->setText("<i>" + tr("Speed: %1x").arg(s) + "</i>");
}

void ExtractingProgressDialog::show_speed_extract(double speed)
{
    QString s = QString("%1").arg((double)speed, 0, 'f', 2);
    ui.label_speed_extracting->setText("<i>" + tr("Speed: %1x").arg(s) + "</i>");
}

void ExtractingProgressDialog::activateEmergencyStop()
{
    lock.lock(); // prevent thread competition
    QPushButton* button = buttonBox->button(QDialogButtonBox::Cancel);
    if (button) {
        button->setText(tr("Stop"));
        button->setEnabled(true);
    }
    lock.unlock();
}

void ExtractingProgressDialog::conclusion(bool successful)
{
    // Remove the cancel button
    lock.lock(); // prevent thread competition
    buttonBox->clear();
    lock.unlock();
    // Add the new close button
    buttonBox->addButton(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ExtractingProgressDialog::slotClose);

    finished = true;

    QPalette pal = QApplication::palette();
    if (successful) {
        QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap(":/icons/emblem-default.png")), tr("All jobs successfully done."));
        ui.klistwidget->addItem(item);
        ui.klistwidget->scrollToItem(item);
        pal.setBrush(QPalette::Text, pal.color(QPalette::Active, QPalette::Text));
        ui.label_extracting->setText("<font style=\"font-weight:bold;\">" + tr("Finished!") + "</font>");
        ui.label_encoding->setText("<font style=\"font-weight:bold;\">" + tr("Finished!") + "</font>");
        ui.label_overall_track->setText("<font style=\"font-weight:bold;\">" + tr("Finished!") + "</font>");
        ui.progressBar_extracting->setValue(100);
        ui.progressBar_encoding->setValue(100);
        ui.progressBar_overall->setValue(100);
    } else {
        QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap(":/icons/emblem-error.png")), tr("At least one job failed."));
        pal.setBrush(QPalette::Text, pal.color(QPalette::Active, QPalette::BrightText));
        ui.klistwidget->addItem(item);
        ui.klistwidget->scrollToItem(item);
        ui.label_extracting->setText("<font style=\"color:red;font-weight:bold;\">" + tr("Failed!") + "</font>");
        ui.label_encoding->setText("<font style=\"color:red;font-weight:bold;\">" + tr("Failed!") + "</font>");
        ui.label_overall_track->setText("<font style=\"color:red;font-weight:bold;\">" + tr("Failed!") + "</font>");
        if (audex->encoderProtocol().count() > 0) {
            auto *encoderProtocolButton = new QPushButton();
            encoderProtocolButton->setText(tr("Show encoding log..."));
            encoderProtocolButton->setIcon(QIcon(QPixmap(":/icons/emblem-system.png")));
            buttonBox->addButton(encoderProtocolButton, QDialogButtonBox::HelpRole);
            connect(encoderProtocolButton, &QPushButton::clicked, this, &ExtractingProgressDialog::slotEncoderProtocol);
        }
        if (audex->extractProtocol().count() > 0) {
            auto *extractProtocolButton = new QPushButton();
            extractProtocolButton->setText(tr("Show rip log..."));
            extractProtocolButton->setIcon(QIcon(QPixmap(":/icons/media-import.png")));
            buttonBox->addButton(extractProtocolButton, QDialogButtonBox::HelpRole);
            connect(extractProtocolButton, &QPushButton::clicked, this, &ExtractingProgressDialog::slotExtractProtocol);
        }
    }

    ui.progressBar_extracting->setEnabled(false);
    ui.progressBar_encoding->setEnabled(false);
    ui.progressBar_overall->setEnabled(false);
    ui.label_speed_extracting->setEnabled(false);
    ui.label_speed_encoding->setEnabled(false);
    ui.label_overall->setEnabled(false);

    ui.label_extracting->setPalette(pal);
    ui.label_encoding->setPalette(pal);
}

void ExtractingProgressDialog::show_info(const QString &message)
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap(":/icons/emblem-info.png")), message);
    ui.klistwidget->addItem(item);
    ui.klistwidget->scrollToItem(item);
}

void ExtractingProgressDialog::show_warning(const QString &message)
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap(":/icons/emblem-danger.png")), message);
    ui.klistwidget->addItem(item);
    ui.klistwidget->scrollToItem(item);
}

void ExtractingProgressDialog::show_error(const QString &message, const QString &details)
{
    QListWidgetItem *item;
    if (details.isEmpty()) {
        item = new QListWidgetItem(QIcon(QPixmap(":/icons/emblem-error.png")), QString("%1").arg(message));
    } else {
        item = new QListWidgetItem(QIcon(QPixmap(":/icons/emblem-error.png")), QString("%1 (%2)").arg(message, details));
    }
    ui.klistwidget->addItem(item);
    ui.klistwidget->scrollToItem(item);
}

void ExtractingProgressDialog::ask_timeout()
{
    if (QMessageBox::question(
            this,
            tr("Cancel extraction"),
            tr("Ripping speed was extremely slow for the last 5 minutes.\nDue to extraction quality, CDDA-Ripper is configured to never skip any detected error. "
                 "If your disc is really broken extraction may never end!\nIn some cases, it might be that only this drive has difficulty ripping audio data from this disc. Maybe try another one.\n\n"
                 "However, do you want to continue extraction?"),
            QMessageBox::StandardButton::No,
            QMessageBox::StandardButton::Yes)
        == QMessageBox::StandardButton::Yes) {
        audex->cancel();
    }
}

void ExtractingProgressDialog::keyPressEvent(QKeyEvent *event){
    // catch the key ESC event
    if (event->key() == Qt::Key_Escape) {
        event->accept();
    } else
       event->ignore();
}

void ExtractingProgressDialog::closeEvent(QCloseEvent *event){
    // ignore the close event until finished
    if(!event->spontaneous() && finished) {
        event->accept();
    } else
        event->ignore();
}

void ExtractingProgressDialog::open_encoder_protocol_dialog()
{
    ProtocolDialog *protocolDialog = new ProtocolDialog(audex->encoderProtocol(), tr("Encoding protocol"), this);
    protocolDialog->exec();
    delete protocolDialog;
}

void ExtractingProgressDialog::open_extract_protocol_dialog()
{
    ProtocolDialog *protocolDialog = new ProtocolDialog(audex->extractProtocol(), tr("Ripping protocol"), this);
    protocolDialog->exec();
    delete protocolDialog;
}
