/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "encoderwrapper.h"

#include <QDebug>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QRegExp>
#else
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#endif

EncoderWrapper::EncoderWrapper(QObject *parent, const QStringList& command, const QString &encoderName, EncoderAssistant::Encoder encoderType, const bool deleteFractionFiles)
    : QObject(parent)
{
    encoder_command = command;
    encoder_name = encoderName;
    encoder_type = encoderType;
    delete_fraction_files = deleteFractionFiles;

    connect(&proc, SIGNAL(readyReadStandardError()), this, SLOT(parseOutput()));
    connect(&proc, SIGNAL(readyReadStandardOutput()), this, SLOT(parseOutput()));
    connect(&proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&proc, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

    proc.setProcessChannelMode(QProcess::SeparateChannels);
    proc.setReadChannel(QProcess::StandardError);

    termination = false;
    processing = 0;

    not_found_counter = 0;
}

EncoderWrapper::~EncoderWrapper()
{
}

bool EncoderWrapper::encode(int n,
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
                            const QString &output)
{
    if (!processing)
        processing = 1;
    else
        return false;
    termination = false;

    if (encoder_command.isEmpty()) {
        Q_EMIT error(tr("Command scheme is empty."));
        return false;
    }

    QString cmd;
    QStringList args;

    if (encoder_type == EncoderAssistant::CUSTOM) {
          cmd = "sh";
          args.push_back("-c");
    }

    SchemeParser schemeparser;
    for (const QString& v : encoder_command)
    {
        if (cmd.isEmpty()) {
            qDebug() << v;
            cmd = v;
        } else {
            QString tmp = schemeparser.parseCommandScheme(v,
                                                          input,
                                                          output,
                                                          n,
                                                          cdno,
                                                          trackoffset,
                                                          nooftracks,
                                                          artist,
                                                          album,
                                                          tartist,
                                                          ttitle,
                                                          date,
                                                          genre,
                                                          suffix,
                                                          cover,
                                                          tmppath,
                                                          encoder_name);

            // using custom encoder the command is wrapped into a shell script,
            // therefore few special characters should be escaped or discarded
            if (encoder_type == EncoderAssistant::CUSTOM) {
                tmp.replace(QChar('!'), QChar('_')); // discard !
                tmp.replace(QChar('$'), "\\$"); // escape $
            }

            args.push_back(tmp);
            qDebug() << tmp;
        }
    }

    proc.start(cmd, args);
    proc.waitForStarted();

    processing_filename = output;

    Q_EMIT info(tr("Encoding track %1...").arg(n));

    return true;
}

void EncoderWrapper::cancel()
{
    if (!processing)
        return;

    // we need to suppress normal error messages, because with a cancel the user known what he does
    termination = true;
    proc.terminate();

    if (delete_fraction_files) {
        QFile file(processing_filename);
        if (file.exists()) {
            file.remove();
            Q_EMIT warning(tr("Deleted partial file \"%1\".").arg(processing_filename.mid(processing_filename.lastIndexOf("/") + 1)));
            qDebug() << "deleted partial file" << processing_filename;
        }
    }

    Q_EMIT error(tr("User canceled encoding."));
    qDebug() << "Interrupt encoding.";
}

bool EncoderWrapper::isProcessing()
{
    return (processing > 0);
}

const QStringList &EncoderWrapper::protocol()
{
    return _protocol;
}

void EncoderWrapper::parseOutput()
{
    QByteArray rawoutput = proc.readAllStandardError();
    if (rawoutput.size() == 0)
        rawoutput = proc.readAllStandardOutput();
    bool found = false;
    if (rawoutput.size() > 0) {
        QString output(rawoutput);
        QStringList list = output.trimmed().split('\n');
        _protocol << list;
        for (int i = 0; i < list.count(); ++i) {
            if (list.at(i).contains('%')) {
                QString line = list.at(i);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                int startPos = line.indexOf(QRegExp("\\d+[,.]?\\d*\\%"));
                if (startPos == -1)
                    continue;
#else
                QRegularExpression re("\\d+[,.]?\\d*\\%");
                QRegularExpressionMatch match = re.match(line);
                if (!match.hasMatch())
                  continue;
                int startPos = match.capturedStart(0);
#endif
                QString p = line.mid(startPos);
                p = p.left(p.indexOf('%'));
                bool conversionSuccessful = false;
                double percent = p.toDouble(&conversionSuccessful);
                if ((conversionSuccessful) && (percent >= 0) && (percent <= 100)) {
                    Q_EMIT progress((int)percent);
                    found = true;
                    not_found_counter = 0;
                }
            }
        }
    }
    if (!found) {
        if (not_found_counter > 5)
            Q_EMIT progress(-1);
        else
            ++not_found_counter;
    }
}

void EncoderWrapper::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    processing = 0;
    if (termination) {
        Q_EMIT finished();
        return;
    }
    if ((exitStatus == QProcess::NormalExit) && (exitCode == 0)) {
        Q_EMIT info(tr("Encoding OK (\"%1\").").arg(processing_filename));
    } else {
        Q_EMIT error(tr("An error occurred while encoding file \"%1\".").arg(processing_filename), tr("Please check your profile."));
    }
    Q_EMIT finished();
    qDebug() << "encoding finished.";
}

void EncoderWrapper::processError(QProcess::ProcessError err)
{
    if (termination)
        return;
    switch (err) {
    case QProcess::FailedToStart:
        Q_EMIT error(tr("%1 failed to start.").arg(encoder), tr("Either it is missing, or you may have insufficient permissions to invoke the program."));
        break;
    case QProcess::Crashed:
        Q_EMIT error(tr("%1 crashed some time after starting successfully.").arg(encoder), tr("Please check your profile."));
        break;
    case QProcess::Timedout:
        Q_EMIT error(tr("%1 timed out. This should not happen.").arg(encoder), tr("Please check your profile."));
        break;
    case QProcess::WriteError:
        Q_EMIT error(tr("An error occurred when attempting to write to %1.").arg(encoder), tr("For example, the process may not be running, or it may have closed its input channel."));
        break;
    case QProcess::ReadError:
        Q_EMIT error(tr("An error occurred when attempting to read from %1.").arg(encoder), tr("For example, the process may not be running."));
        break;
    case QProcess::UnknownError:
        Q_EMIT error(tr("An unknown error occurred to %1. This should not happen.").arg(encoder), tr("Please check your profile."));
        break;
    }
    Q_EMIT finished();
    qDebug() << "encoding finished.";
}
