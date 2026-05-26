/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef ENCODERWRAPPER_HEADER
#define ENCODERWRAPPER_HEADER

#include <QImage>
#include <QString>

#include <QProcess>

#include "utils/cachedimage.h"
#include "utils/encoderassistant.h"

class EncoderWrapper : public QObject
{
    Q_OBJECT

public:
    explicit EncoderWrapper(QObject *parent,
                            const QStringList& command,
                            const QString &encoderName,
                            EncoderAssistant::Encoder encoderType,
                            const bool deleteFractionFiles = true);
    ~EncoderWrapper() override;

    bool isProcessing();
    const QStringList &protocol();

public Q_SLOTS:
    bool encode(int n,
                int cdno,
                int trackoffset,
                int nooftracks,
                const QString &artist,
                const QString &album,
                const QString &tartist,
                const QString &ttitle,
                const QString &genre,
                const QString &date,
                const QString &suffix,
                CachedImage *cover,
                const QString &tmppath,
                const QString &input,
                const QString &output);
    void cancel();

private Q_SLOTS:
    void parseOutput();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError err);

Q_SIGNALS:
    void progress(int percent_of_track);
    void finished();

    void error(const QString &message, const QString &details = QString());
    void warning(const QString &message);
    void info(const QString &message);

private:
    QStringList encoder_command;
    QString encoder_name;
    EncoderAssistant::Encoder encoder_type;
    bool delete_fraction_files;

    QString encoder;
    QStringList _protocol;

    QString processing_filename;

    bool termination;
    int processing;

    QProcess proc;

    int not_found_counter;
};

#endif
