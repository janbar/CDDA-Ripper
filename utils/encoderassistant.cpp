/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "encoderassistant.h"

long makeVersionNumber(int major, int minor, int patch)
{
    long versionNumber = ((major & 0xFF) << 16) | ((minor & 0xFF) << 8) | (patch & 0xFF);
    return versionNumber;
}

const QString EncoderAssistant::name(const EncoderAssistant::Encoder encoder)
{
    switch (encoder) {
    case EncoderAssistant::LAME:
        return ENCODER_LAME_NAME;
    case EncoderAssistant::OGGENC:
        return ENCODER_OGGENC_NAME;
    case EncoderAssistant::FLAC:
        return ENCODER_FLAC_NAME;
    case EncoderAssistant::FAAC:
        return ENCODER_FAAC_NAME;
    case EncoderAssistant::WAVE:
        return ENCODER_WAVE_NAME;
    case EncoderAssistant::CUSTOM:
        return ENCODER_CUSTOM_NAME;
    case EncoderAssistant::OPUSENC:
        return ENCODER_OPUSENC_NAME;
    default:
        break;
    }

    return "";
}

const QString EncoderAssistant::encoderName(const Encoder encoder)
{
    switch (encoder) {
    case EncoderAssistant::LAME:
        return ENCODER_LAME_ENCODER_NAME;
    case EncoderAssistant::OGGENC:
        return ENCODER_OGGENC_ENCODER_NAME;
    case EncoderAssistant::FLAC:
        return ENCODER_FLAC_ENCODER_NAME;
    case EncoderAssistant::FAAC:
        return ENCODER_FAAC_ENCODER_NAME;
    case EncoderAssistant::WAVE:
        return ENCODER_WAVE_ENCODER_NAME;
    case EncoderAssistant::CUSTOM:
        return ENCODER_CUSTOM_ENCODER_NAME;
    case EncoderAssistant::OPUSENC:
        return ENCODER_OPUSENC_ENCODER_NAME;
    default:
        break;
    }

    return "";
}

const QString EncoderAssistant::icon(const EncoderAssistant::Encoder encoder)
{
    switch (encoder) {
    case EncoderAssistant::LAME:
        return ENCODER_LAME_ICON;
    case EncoderAssistant::OGGENC:
        return ENCODER_OGGENC_ICON;
    case EncoderAssistant::FLAC:
        return ENCODER_FLAC_ICON;
    case EncoderAssistant::FAAC:
        return ENCODER_FAAC_ICON;
    case EncoderAssistant::WAVE:
        return ENCODER_WAVE_ICON;
    case EncoderAssistant::CUSTOM:
        return ENCODER_CUSTOM_ICON;
    case EncoderAssistant::OPUSENC:
        return ENCODER_OPUSENC_ICON;
    default:
        break;
    }

    return "";
}

bool EncoderAssistant::available(const EncoderAssistant::Encoder encoder)
{
    switch (encoder) {
    case EncoderAssistant::LAME:
        return (QProcess::execute(ENCODER_LAME_BIN, QStringList() << ENCODER_LAME_VERSION_PARA) == 0);
    case EncoderAssistant::OGGENC:
        return (QProcess::execute(ENCODER_OGGENC_BIN, QStringList() << ENCODER_OGGENC_VERSION_PARA) == 0);
    case EncoderAssistant::FLAC:
        return (QProcess::execute(ENCODER_FLAC_BIN, QStringList() << ENCODER_FLAC_VERSION_PARA) == 0);
    case EncoderAssistant::FAAC:
        return (QProcess::execute(ENCODER_FAAC_BIN, QStringList() << ENCODER_FAAC_VERSION_PARA) == 1);
    case EncoderAssistant::WAVE:
        return (QProcess::execute(ENCODER_WAVE_BIN, QStringList() << ENCODER_WAVE_VERSION_PARA) == 0);
    case EncoderAssistant::CUSTOM:
        return true;
    case EncoderAssistant::OPUSENC:
        return (QProcess::execute(ENCODER_OPUSENC_BIN, QStringList() << ENCODER_OPUSENC_VERSION_PARA) == 0);
    default:
        break;
    }

    return false;
}

bool EncoderAssistant::canEmbedCover(const Encoder encoder, int *maxCoverSize)
{
    switch (encoder) {
    case EncoderAssistant::LAME:
        if (maxCoverSize)
            *maxCoverSize = ENCODER_LAME_MAX_EMBED_COVER_SIZE;
        return true;
    case EncoderAssistant::OGGENC:
    case EncoderAssistant::FLAC:
    case EncoderAssistant::OPUSENC:
        return true;
    case EncoderAssistant::FAAC:
    case EncoderAssistant::WAVE:
    case EncoderAssistant::CUSTOM:
    default:
        break;
    }

    if (maxCoverSize)
        *maxCoverSize = 0;

    return false;
}

const QString EncoderAssistant::version(const EncoderAssistant::Encoder encoder)
{
    QProcess process;
    process.setProcessChannelMode(QProcess::SeparateChannels);
    process.setReadChannel(QProcess::StandardError);
    QString cmd;
    QStringList args;
    switch (encoder) {
    case EncoderAssistant::LAME:
        cmd = ENCODER_LAME_BIN;
        args.push_back(ENCODER_LAME_VERSION_PARA);
        break;
    case EncoderAssistant::OGGENC:
        cmd = ENCODER_OGGENC_BIN;
        args.push_back(ENCODER_OGGENC_VERSION_PARA);
        break;
    case EncoderAssistant::FLAC:
        cmd = ENCODER_FLAC_BIN;
        args.push_back(ENCODER_FLAC_VERSION_PARA);
        break;
    case EncoderAssistant::FAAC:
          cmd = ENCODER_FAAC_BIN;
          args.push_back(ENCODER_FAAC_VERSION_PARA);
        break;
    case EncoderAssistant::WAVE:
        return "";
    case EncoderAssistant::CUSTOM:
        return "";
    case EncoderAssistant::OPUSENC:
        cmd = ENCODER_OPUSENC_BIN;
        args.push_back(ENCODER_OPUSENC_VERSION_PARA);
        break;
    default:
        return "";
    }
    process.start(cmd, args);
    if (!process.waitForFinished())
        return "";
    QByteArray rawoutput = process.readAllStandardError();
    if (rawoutput.size() == 0)
        rawoutput = process.readAllStandardOutput();
    QString output(rawoutput);
    QStringList list = output.trimmed().split('\n');
    if (list.count() == 0)
        return "";
    QStringList words = list[0].split(' ');
    if (words.count() == 0)
        return "";

    switch (encoder) {
    case EncoderAssistant::LAME:
        if ((words.contains("version")) && (words.indexOf("version") + 1 < words.count()))
            return words[words.indexOf("version") + 1];
        if (words.count() < 2)
            return "";
        return words[words.count() - 2];

    case EncoderAssistant::OGGENC:
    case EncoderAssistant::FLAC:
        return words.last();

    case EncoderAssistant::FAAC:
        if (list.count() < 2)
            return "";
        words = list[1].split(' ');
        if (words.count() < 2)
            return "";
        if ((words.contains("FAAC")) && (words.indexOf("FAAC") + 1 < words.count()))
            return words[words.indexOf("FAAC") + 1];
        return words[1];

    case EncoderAssistant::OPUSENC:
        if ((words.contains("libopus")) && (words.indexOf("libopus") + 1 < words.count()))
            return words[words.indexOf("libopus") + 1];
        if (words.count() < 3)
            return "";
        return words[2];

    case EncoderAssistant::WAVE:
    case EncoderAssistant::CUSTOM:
    default:;
    }

    return "";
}

long EncoderAssistant::versionNumber(const EncoderAssistant::Encoder encoder)
{
    QString v = version(encoder);
    long versionNumber = 0;

    switch (encoder) {
    case EncoderAssistant::LAME:
    case EncoderAssistant::OGGENC:
    case EncoderAssistant::FLAC:
    case EncoderAssistant::FAAC:
    case EncoderAssistant::OPUSENC:

    {
        // At present all encoders seem to use 2 or 3 version number items
        // separated by . so we use the same code for all
        // convert to a number for easy version comparison.
        // Each of the 3 version components must be < 255
        QStringList version = v.split('.');
        int major = 0;
        int minor = 0;
        int patch = 0;

        if (version.count() > 0) {
            major = version[0].toUInt();
            versionNumber = (major & 0xFF) << 16;
        }
        if (version.count() > 1) {
            minor = version[1].toUInt();
            versionNumber = versionNumber | (minor & 0xFF) << 8;
        }
        if (version.count() > 2) {
            patch = version[2].toUInt();
            versionNumber = versionNumber | (patch & 0xFF);
        }
    } break;

    case EncoderAssistant::WAVE:
    case EncoderAssistant::CUSTOM:
    default:
      break;
    }
    return versionNumber;
}

QStringList EncoderAssistant::scheme(const EncoderAssistant::Encoder encoder, const Parameters &parameters)
{
    QStringList arguments;

    switch (encoder) {
    case EncoderAssistant::LAME: {
        int preset = parameters.value(ENCODER_LAME_PRESET_KEY, ENCODER_LAME_PRESET).toInt();
        bool cbr = parameters.value(ENCODER_LAME_CBR_KEY).toBool();
        int bitrate = parameters.value(ENCODER_LAME_BITRATE_KEY, ENCODER_LAME_BITRATE).toInt();
        bool embed_cover = parameters.value(ENCODER_LAME_EMBED_COVER_KEY).toBool();
        arguments.push_back(ENCODER_LAME_BIN);
        switch (preset) {
        case ENCODER_LAME_PRESET_MEDIUM:
            arguments.push_back("--preset");
            arguments.push_back("medium");
            break;
        case ENCODER_LAME_PRESET_STANDARD:
            arguments.push_back("--preset");
            arguments.push_back("standard");
            break;
        case ENCODER_LAME_PRESET_EXTREME:
            arguments.push_back("--preset");
            arguments.push_back("extreme");
            break;
        case ENCODER_LAME_PRESET_INSANE:
            arguments.push_back("--preset");
            arguments.push_back("insane");
            break;
        case ENCODER_LAME_PRESET_CUSTOM:
            arguments.push_back("--preset");
            if (cbr)
                arguments.push_back("cbr");
            arguments.push_back(QString("%1").arg(bitrate));
            break;
        default:
            arguments.push_back("--preset");
            arguments.push_back("standard");
        }
        QString v = EncoderAssistant::version(EncoderAssistant::LAME);
        if ((v.startsWith(QLatin1String("3.95"))) || (v.startsWith(QLatin1String("3.96"))) || (v.startsWith(QLatin1String("3.97"))))
            arguments.push_back("--vbr-new");

        if (embed_cover)
        {
            arguments.push_back("--ti");
            arguments.push_back(QString::fromUtf8("${" VAR_COVER_FILE "}"));
        }

        arguments.push_back("--add-id3v2");
        arguments.push_back("--id3v2-only");
        arguments.push_back("--ignore-tag-errors");
        arguments.push_back("--tt");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_TITLE ""));
        arguments.push_back("--ta");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_ARTIST ""));
        arguments.push_back("--tl");
        arguments.push_back(QString::fromUtf8("$" VAR_ALBUM_TITLE ""));
        arguments.push_back("--ty");
        arguments.push_back(QString::fromUtf8("$" VAR_DATE ""));
        arguments.push_back("--tn");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_NO "/$" VAR_NO_OF_TRACKS ""));
        arguments.push_back("--tc");
        arguments.push_back(QString::fromUtf8("$" VAR_APP " / Encoder $" VAR_ENCODER ""));
        arguments.push_back("--tg");
        arguments.push_back(QString::fromUtf8("$" VAR_GENRE ""));
        arguments.push_back("--tv");
        arguments.push_back(QString::fromUtf8("TPOS=$" VAR_CD_NO ""));
        arguments.push_back(QString::fromUtf8("$" VAR_INPUT_FILE ""));
        arguments.push_back(QString::fromUtf8("$" VAR_OUTPUT_FILE ""));
        return arguments;
    }


    case EncoderAssistant::OGGENC: {
        qreal quality = parameters.value(ENCODER_OGGENC_QUALITY_KEY, ENCODER_OGGENC_QUALITY).toReal();
        bool min_bitrate = parameters.value(ENCODER_OGGENC_MINBITRATE_KEY).toBool();
        int min_bitrate_value = parameters.value(ENCODER_OGGENC_MINBITRATE_VALUE_KEY, ENCODER_OGGENC_MINBITRATE_VALUE).toInt();
        bool max_bitrate = parameters.value(ENCODER_OGGENC_MAXBITRATE_KEY).toBool();
        int max_bitrate_value = parameters.value(ENCODER_OGGENC_MAXBITRATE_VALUE_KEY, ENCODER_OGGENC_MAXBITRATE_VALUE).toInt();
        arguments.push_back(ENCODER_OGGENC_BIN);
        arguments.push_back("-q");
        arguments.push_back(QString("%1").arg(quality, 0, 'f', 2));

        if (min_bitrate)
        {
            arguments.push_back("-m");
            arguments.push_back(QString("%1").arg(min_bitrate_value));
        }
        if (max_bitrate)
        {
            arguments.push_back("-M");
            arguments.push_back(QString("%1").arg(max_bitrate_value));
        }
        arguments.push_back("-c");
        arguments.push_back(QString::fromUtf8("Artist=$" VAR_TRACK_ARTIST ""));
        arguments.push_back("-c");
        arguments.push_back(QString::fromUtf8("Title=$" VAR_TRACK_TITLE ""));
        arguments.push_back("-c");
        arguments.push_back(QString::fromUtf8("Album=$" VAR_ALBUM_TITLE ""));
        arguments.push_back("-c");
        arguments.push_back(QString::fromUtf8("Date=$" VAR_DATE""));
        arguments.push_back("-c");
        arguments.push_back(QString::fromUtf8("Tracknumber=$" VAR_TRACK_NO ""));
        arguments.push_back("-c");
        arguments.push_back(QString::fromUtf8("Genre=$" VAR_GENRE ""));
        arguments.push_back("-c");
        arguments.push_back(QString::fromUtf8("Discnumber=$" VAR_CD_NO ""));
        arguments.push_back("-o");
        arguments.push_back(QString::fromUtf8("$" VAR_OUTPUT_FILE""));
        arguments.push_back(QString::fromUtf8("$" VAR_INPUT_FILE ""));
        return arguments;
    }

    case EncoderAssistant::FLAC: {
        int compression = parameters.value(ENCODER_FLAC_COMPRESSION_KEY, ENCODER_FLAC_COMPRESSION).toInt();
        bool embed_cover = parameters.value(ENCODER_FLAC_EMBED_COVER_KEY).toBool();
        arguments.push_back(ENCODER_FLAC_BIN);
        if (embed_cover) {
            long versionNumber = EncoderAssistant::versionNumber(EncoderAssistant::FLAC);
            if (versionNumber >= makeVersionNumber(1, 1, 3))
                arguments.push_back(QString::fromUtf8("--picture=\\|\\|\\|\\|${" VAR_COVER_FILE "}"));
        }

        arguments.push_back(QString("-%1").arg(compression));

        arguments.push_back("-T");
        arguments.push_back(QString::fromUtf8("Artist=$" VAR_TRACK_ARTIST ""));
        arguments.push_back("-T");
        arguments.push_back(QString::fromUtf8("Title=$" VAR_TRACK_TITLE ""));
        arguments.push_back("-T");
        arguments.push_back(QString::fromUtf8("Album=$" VAR_ALBUM_TITLE ""));
        arguments.push_back("-T");
        arguments.push_back(QString::fromUtf8("Date=$" VAR_DATE ""));
        arguments.push_back("-T");
        arguments.push_back(QString::fromUtf8("Tracknumber=$" VAR_TRACK_NO ""));
        arguments.push_back("-T");
        arguments.push_back(QString::fromUtf8("Genre=$" VAR_GENRE ""));
        arguments.push_back("-o");
        arguments.push_back(QString::fromUtf8("$" VAR_OUTPUT_FILE ""));
        arguments.push_back(QString::fromUtf8("$" VAR_INPUT_FILE ""));
        return arguments;
    }

    case EncoderAssistant::FAAC: {
        int quality = parameters.value(ENCODER_FAAC_QUALITY_KEY, ENCODER_FAAC_QUALITY).toInt();
        bool embed_cover = parameters.value(ENCODER_FAAC_EMBED_COVER_KEY).toBool();
        arguments.push_back(ENCODER_FAAC_BIN);
        arguments.push_back("-q");
        arguments.push_back(QString("%1").arg(quality));

        if (embed_cover)
        {
            arguments.push_back("--cover-art");
            arguments.push_back(QString::fromUtf8("${" VAR_COVER_FILE "}"));
        }

        arguments.push_back("--title");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_TITLE ""));
        arguments.push_back("--artist");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_ARTIST ""));
        arguments.push_back("--album");
        arguments.push_back(QString::fromUtf8("$" VAR_ALBUM_TITLE ""));
        arguments.push_back("--year");
        arguments.push_back(QString::fromUtf8("$" VAR_DATE ""));
        arguments.push_back("--track");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_NO));
        arguments.push_back("--disc");
        arguments.push_back(QString::fromUtf8("$" VAR_CD_NO));
        arguments.push_back("--genre");
        arguments.push_back(QString::fromUtf8("$" VAR_GENRE ""));
        arguments.push_back("-o");
        arguments.push_back(QString::fromUtf8("$" VAR_OUTPUT_FILE ""));
        arguments.push_back(QString::fromUtf8("$" VAR_INPUT_FILE ""));
        return arguments;
    }

    case EncoderAssistant::WAVE: {
        arguments.push_back(ENCODER_WAVE_BIN);
        arguments.push_back(QString::fromUtf8("$" VAR_INPUT_FILE ""));
        arguments.push_back(QString::fromUtf8("$" VAR_OUTPUT_FILE ""));
        return arguments;
    }

    case EncoderAssistant::CUSTOM: {
        arguments.push_back(parameters.value(ENCODER_CUSTOM_COMMAND_SCHEME_KEY, ENCODER_CUSTOM_COMMAND_SCHEME).toString());
        return arguments;
    }

    case EncoderAssistant::OPUSENC: {
        int bitrate = parameters.value(ENCODER_OPUSENC_BITRATE_KEY, ENCODER_OPUSENC_BITRATE).toInt();
        bool embed_cover = parameters.value(ENCODER_OPUSENC_EMBED_COVER_KEY).toBool();
        arguments.push_back(ENCODER_OPUSENC_BIN);
        if (embed_cover) {
            arguments.push_back(QString::fromUtf8("--picture=\\|\\|\\|\\|${" VAR_COVER_FILE "}"));
        }

        arguments.push_back("--music");

        arguments.push_back("--bitrate");
        arguments.push_back(QString("%1").arg(bitrate));

        arguments.push_back("--artist");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_ARTIST ""));
        arguments.push_back("--title");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_TITLE ""));
        arguments.push_back("--album");
        arguments.push_back(QString::fromUtf8("$" VAR_ALBUM_TITLE ""));
        arguments.push_back("--date");
        arguments.push_back(QString::fromUtf8("$" VAR_DATE ""));
        arguments.push_back("--tracknumber");
        arguments.push_back(QString::fromUtf8("$" VAR_TRACK_NO ""));
        arguments.push_back("--genre");
        arguments.push_back(QString::fromUtf8("$" VAR_GENRE ""));
        arguments.push_back(QString::fromUtf8("$" VAR_INPUT_FILE ""));
        arguments.push_back(QString::fromUtf8("$" VAR_OUTPUT_FILE ""));
        return arguments;
    }

    default:
        break;
    }

    return arguments;
}

Parameters EncoderAssistant::stdParameters(const Encoder encoder, const Quality quality)
{
    Parameters parameters;

    switch (encoder) {
    case EncoderAssistant::LAME:

        switch (quality) {
        case NORMAL:
            parameters.setValue(ENCODER_LAME_PRESET_KEY, ENCODER_LAME_PRESET);
            parameters.setValue(ENCODER_LAME_EMBED_COVER_KEY, ENCODER_LAME_EMBED_COVER);
            parameters.setValue(ENCODER_LAME_BITRATE_KEY, ENCODER_LAME_BITRATE);
            parameters.setValue(ENCODER_LAME_EMBED_COVER_KEY, ENCODER_LAME_EMBED_COVER);
            break;

        case MOBILE:
            parameters.setValue(ENCODER_LAME_PRESET_KEY, ENCODER_LAME_PRESET_M);
            parameters.setValue(ENCODER_LAME_EMBED_COVER_KEY, ENCODER_LAME_EMBED_COVER_M);
            parameters.setValue(ENCODER_LAME_BITRATE_KEY, ENCODER_LAME_BITRATE_M);
            parameters.setValue(ENCODER_LAME_EMBED_COVER_KEY, ENCODER_LAME_EMBED_COVER_M);
            break;

        case EXTREME:
            parameters.setValue(ENCODER_LAME_PRESET_KEY, ENCODER_LAME_PRESET_X);
            parameters.setValue(ENCODER_LAME_EMBED_COVER_KEY, ENCODER_LAME_EMBED_COVER_X);
            parameters.setValue(ENCODER_LAME_BITRATE_KEY, ENCODER_LAME_BITRATE_X);
            parameters.setValue(ENCODER_LAME_EMBED_COVER_KEY, ENCODER_LAME_EMBED_COVER_X);
            break;
        }

        break;

    case EncoderAssistant::OGGENC:

        switch (quality) {
        case NORMAL:
            parameters.setValue(ENCODER_OGGENC_QUALITY_KEY, ENCODER_OGGENC_QUALITY);
            parameters.setValue(ENCODER_OGGENC_MINBITRATE_KEY, ENCODER_OGGENC_MINBITRATE);
            parameters.setValue(ENCODER_OGGENC_MINBITRATE_VALUE_KEY, ENCODER_OGGENC_MINBITRATE_VALUE);
            parameters.setValue(ENCODER_OGGENC_MAXBITRATE_KEY, ENCODER_OGGENC_MAXBITRATE);
            parameters.setValue(ENCODER_OGGENC_MAXBITRATE_VALUE_KEY, ENCODER_OGGENC_MAXBITRATE_VALUE);
            break;

        case MOBILE:
            parameters.setValue(ENCODER_OGGENC_QUALITY_KEY, ENCODER_OGGENC_QUALITY_M);
            parameters.setValue(ENCODER_OGGENC_MINBITRATE_KEY, ENCODER_OGGENC_MINBITRATE_M);
            parameters.setValue(ENCODER_OGGENC_MINBITRATE_VALUE_KEY, ENCODER_OGGENC_MINBITRATE_VALUE_M);
            parameters.setValue(ENCODER_OGGENC_MAXBITRATE_KEY, ENCODER_OGGENC_MAXBITRATE_M);
            parameters.setValue(ENCODER_OGGENC_MAXBITRATE_VALUE_KEY, ENCODER_OGGENC_MAXBITRATE_VALUE_M);
            break;

        case EXTREME:
            parameters.setValue(ENCODER_OGGENC_QUALITY_KEY, ENCODER_OGGENC_QUALITY_X);
            parameters.setValue(ENCODER_OGGENC_MINBITRATE_KEY, ENCODER_OGGENC_MINBITRATE_X);
            parameters.setValue(ENCODER_OGGENC_MINBITRATE_VALUE_KEY, ENCODER_OGGENC_MINBITRATE_VALUE_X);
            parameters.setValue(ENCODER_OGGENC_MAXBITRATE_KEY, ENCODER_OGGENC_MAXBITRATE_X);
            parameters.setValue(ENCODER_OGGENC_MAXBITRATE_VALUE_KEY, ENCODER_OGGENC_MAXBITRATE_VALUE_X);
            break;
        }

        break;

    case EncoderAssistant::FLAC:

        parameters.setValue(ENCODER_FLAC_COMPRESSION_KEY, ENCODER_FLAC_COMPRESSION);
        parameters.setValue(ENCODER_FLAC_EMBED_COVER_KEY, ENCODER_FLAC_EMBED_COVER);

        break;

    case EncoderAssistant::FAAC:

        switch (quality) {
        case NORMAL:
            parameters.setValue(ENCODER_FAAC_QUALITY_KEY, ENCODER_FAAC_QUALITY);
            break;

        case MOBILE:
            parameters.setValue(ENCODER_FAAC_QUALITY_KEY, ENCODER_FAAC_QUALITY_M);
            break;

        case EXTREME:
            parameters.setValue(ENCODER_FAAC_QUALITY_KEY, ENCODER_FAAC_QUALITY_X);
            break;
        }

        break;

    case EncoderAssistant::OPUSENC:

        switch (quality) {
        case NORMAL:
            parameters.setValue(ENCODER_OPUSENC_BITRATE_KEY, ENCODER_OPUSENC_BITRATE);
            parameters.setValue(ENCODER_OPUSENC_EMBED_COVER_KEY, ENCODER_OPUSENC_EMBED_COVER);
            break;

        case MOBILE:
            parameters.setValue(ENCODER_OPUSENC_BITRATE_KEY, ENCODER_OPUSENC_BITRATE_M);
            parameters.setValue(ENCODER_OPUSENC_EMBED_COVER_KEY, ENCODER_OPUSENC_EMBED_COVER_M);
            break;

        case EXTREME:
            parameters.setValue(ENCODER_OPUSENC_BITRATE_KEY, ENCODER_OPUSENC_BITRATE_X);
            parameters.setValue(ENCODER_OPUSENC_EMBED_COVER_KEY, ENCODER_OPUSENC_EMBED_COVER_X);
            break;
        }

        break;

    case EncoderAssistant::WAVE:
    case EncoderAssistant::CUSTOM:
    default:
        break;
    }

    return parameters;
}

const QMap<int, QString> EncoderAssistant::encoderList()
{
    QMap<int, QString> encoders;

    encoders[(int)EncoderAssistant::LAME] = ENCODER_LAME_NAME;
    encoders[(int)EncoderAssistant::OGGENC] = ENCODER_OGGENC_NAME;
    encoders[(int)EncoderAssistant::FLAC] = ENCODER_FLAC_NAME;
    encoders[(int)EncoderAssistant::FAAC] = ENCODER_FAAC_NAME;
    encoders[(int)EncoderAssistant::WAVE] = ENCODER_WAVE_NAME;
    encoders[(int)EncoderAssistant::CUSTOM] = ENCODER_CUSTOM_NAME;
    encoders[(int)EncoderAssistant::OPUSENC] = ENCODER_OPUSENC_NAME;

    return encoders;
}

const QList<QPair<int, QString> > EncoderAssistant::availableEncoderNameList()
{
    QList<QPair<int, QString> > encoders;

    if (EncoderAssistant::available(EncoderAssistant::FLAC))
        encoders.push_back(qMakePair((int)EncoderAssistant::FLAC, QString(ENCODER_FLAC_NAME)));
    if (EncoderAssistant::available(EncoderAssistant::OPUSENC))
        encoders.push_back(qMakePair((int)EncoderAssistant::OPUSENC, QString(ENCODER_OPUSENC_NAME)));
    if (EncoderAssistant::available(EncoderAssistant::OGGENC))
        encoders.push_back(qMakePair((int)EncoderAssistant::OGGENC, QString(ENCODER_OGGENC_NAME)));
    if (EncoderAssistant::available(EncoderAssistant::FAAC))
        encoders.push_back(qMakePair((int)EncoderAssistant::FAAC, QString(ENCODER_FAAC_NAME)));
    if (EncoderAssistant::available(EncoderAssistant::LAME))
        encoders.push_back(qMakePair((int)EncoderAssistant::LAME, QString(ENCODER_LAME_NAME)));
    if (EncoderAssistant::available(EncoderAssistant::WAVE))
        encoders.push_back(qMakePair((int)EncoderAssistant::WAVE, QString(ENCODER_WAVE_NAME)));
    if (EncoderAssistant::available(EncoderAssistant::CUSTOM))
        encoders.push_back(qMakePair((int)EncoderAssistant::CUSTOM, QString(ENCODER_CUSTOM_NAME)));

    return encoders;
}

const QList<QPair<int, QString> > EncoderAssistant::availableEncoderNameListWithVersions()
{
    QList<QPair<int, QString> > encoders;

    if (EncoderAssistant::available(EncoderAssistant::FLAC))
        encoders.push_back(qMakePair((int)EncoderAssistant::FLAC, QString(ENCODER_FLAC_NAME) + ' ' + version(FLAC)));
    if (EncoderAssistant::available(EncoderAssistant::OPUSENC))
        encoders.push_back(qMakePair((int)EncoderAssistant::OPUSENC, QString(ENCODER_OPUSENC_NAME) + ' ' + version(OPUSENC)));
    if (EncoderAssistant::available(EncoderAssistant::OGGENC))
        encoders.push_back(qMakePair((int)EncoderAssistant::OGGENC, QString(ENCODER_OGGENC_NAME) + ' ' + version(OGGENC)));
    if (EncoderAssistant::available(EncoderAssistant::FAAC))
        encoders.push_back(qMakePair((int)EncoderAssistant::FAAC, QString(ENCODER_FAAC_NAME) + ' ' + version(FAAC)));
    if (EncoderAssistant::available(EncoderAssistant::LAME))
        encoders.push_back(qMakePair((int)EncoderAssistant::LAME, QString(ENCODER_LAME_NAME) + ' ' + version(LAME)));
    if (EncoderAssistant::available(EncoderAssistant::WAVE))
        encoders.push_back(qMakePair((int)EncoderAssistant::WAVE, QString(ENCODER_WAVE_NAME) + ' ' + version(WAVE)));
    if (EncoderAssistant::available(EncoderAssistant::CUSTOM))
        encoders.push_back(qMakePair((int)EncoderAssistant::CUSTOM, QString(ENCODER_CUSTOM_NAME) + ' ' + version(CUSTOM)));

    return encoders;
}
