/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: Copyright (C) 2007 Marco Nelles
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "profilemodel.h"

#include <QIcon>

ProfileModel::ProfileModel(QObject *parent)
{
    Q_UNUSED(parent);
    revert();
}

ProfileModel::~ProfileModel()
{
    clear();
}

int ProfileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return p_cache.count();
}

int ProfileModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return PROFILE_MODEL_COLUMN_NUM;
}

QVariant ProfileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if ((index.row() < 0) || (index.row() >= p_cache.count())) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }

    if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
        switch (index.column()) {
        case PROFILE_MODEL_COLUMN_PROFILEINDEX_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_PROFILEINDEX_KEY];
        case PROFILE_MODEL_COLUMN_NAME_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_NAME_KEY];
        case PROFILE_MODEL_COLUMN_ICON_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_ICON_KEY];

        case PROFILE_MODEL_COLUMN_ENCODER_SELECTED_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_ENCODER_SELECTED_KEY];

        case PROFILE_MODEL_COLUMN_SCHEME_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_SCHEME_KEY];
        case PROFILE_MODEL_COLUMN_FAT32COMPATIBLE_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_FAT32COMPATIBLE_KEY];
        case PROFILE_MODEL_COLUMN_UNDERSCORE_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_UNDERSCORE_KEY];
        case PROFILE_MODEL_COLUMN_2DIGITSTRACKNUM_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_2DIGITSTRACKNUM_KEY];
        case PROFILE_MODEL_COLUMN_SC_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_SC_KEY];
        case PROFILE_MODEL_COLUMN_SC_SCALE_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_SC_SCALE_KEY];
        case PROFILE_MODEL_COLUMN_SC_SIZE_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_SC_SIZE_KEY];
        case PROFILE_MODEL_COLUMN_SC_FORMAT_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_SC_FORMAT_KEY];
        case PROFILE_MODEL_COLUMN_SC_NAME_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_SC_NAME_KEY];
        case PROFILE_MODEL_COLUMN_PL_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_PL_KEY];
        case PROFILE_MODEL_COLUMN_PL_FORMAT_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_PL_FORMAT_KEY];
        case PROFILE_MODEL_COLUMN_PL_NAME_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_PL_NAME_KEY];
        case PROFILE_MODEL_COLUMN_PL_ABS_FILE_PATH_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_PL_ABS_FILE_PATH_KEY];
        case PROFILE_MODEL_COLUMN_PL_UTF8_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_PL_UTF8_KEY];
        case PROFILE_MODEL_COLUMN_INF_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_INF_KEY];
        case PROFILE_MODEL_COLUMN_INF_TEXT_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_INF_TEXT_KEY];
        case PROFILE_MODEL_COLUMN_INF_NAME_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_INF_NAME_KEY];
        case PROFILE_MODEL_COLUMN_INF_SUFFIX_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_INF_SUFFIX_KEY];
        case PROFILE_MODEL_COLUMN_HL_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_HL_KEY];
        case PROFILE_MODEL_COLUMN_HL_FORMAT_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_HL_FORMAT_KEY];
        case PROFILE_MODEL_COLUMN_HL_NAME_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_HL_NAME_KEY];
        case PROFILE_MODEL_COLUMN_CUE_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_CUE_KEY];
        case PROFILE_MODEL_COLUMN_CUE_NAME_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_CUE_NAME_KEY];
        case PROFILE_MODEL_COLUMN_SF_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_SF_KEY];
        case PROFILE_MODEL_COLUMN_SF_NAME_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_SF_NAME_KEY];
        case PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY];
        case PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY];
        case PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_KEY];
        case PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY];
        case PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_KEY];
        case PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_INDEX:
            return p_cache.at(index.row())[PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_KEY];
        }
    }

    if (role == Qt::DecorationRole) {
        QString iconName(p_cache.at(index.row())[PROFILE_MODEL_ICON_KEY].toString());

        if (!iconName.isEmpty()) {
            QIcon icon(iconName);
            if (!icon.isNull()) {
                return icon;
            }
        }

        return QIcon::fromTheme(DEFAULT_ICON);
    }

    return QVariant();
}

bool ProfileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if ((index.row() < 0) || (index.row() >= p_cache.count())) {
        return false;
    }

    beginResetModel();

    if (role == Qt::EditRole) {
        switch (index.column()) {
        case PROFILE_MODEL_COLUMN_PROFILEINDEX_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_ICON_INDEX:
            break;

        case PROFILE_MODEL_COLUMN_NAME_INDEX:
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Profile name must not be empty."), tr("You have given no name for the profile. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_SELECTED_INDEX:
            if (value.toInt() == -1) {
                p_error = Error(tr("Profile encoder is not defined."), tr("You have given no encoder for the profile. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_SCHEME_INDEX:
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Profile filename scheme is not defined."),
                                tr("You have given no filename scheme for the profile. Please set one."),
                                Error::ERROR,
                                this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_FAT32COMPATIBLE_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_UNDERSCORE_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_2DIGITSTRACKNUM_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_SC_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_SC_SCALE_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_SC_SIZE_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_SC_FORMAT_INDEX:
            if ((value.toString() != "JPEG") && (value.toString() != "JPG") && (value.toString() != "PNG") && (value.toString() != "TIFF") && (value.toString() != "BMP")) {
                p_error = Error(tr("The image file format is unknown."), tr("Your given image file format is unknown. Please choose on of these formats: JPG/JPEG, PNG or BMP."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_SC_NAME_INDEX:
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Cover name must not be empty."), tr("You have given no name for the cover. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_PL_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_PL_FORMAT_INDEX:
            if ((value.toString() != "M3U") && (value.toString() != "PLS") && (value.toString() != "XSPF")) {
                p_error = Error(tr("The playlist file format is unknown."), tr("Your given playlist file format is unknown. Please choose on of these formats: M3U, PLS or XSPF."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_PL_NAME_INDEX:
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Playlist name must not be empty."), tr("You have given no name for the playlist. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_PL_ABS_FILE_PATH_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_PL_UTF8_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_INF_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_INF_TEXT_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_INF_NAME_INDEX:
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Info text name must not be empty."), tr("You have given no name for the info text file. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_INF_SUFFIX_INDEX:
            break;
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Info text file name suffix must not be empty."), tr("You have given no suffix for the info text file. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_HL_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_HL_FORMAT_INDEX:
            if ((value.toString() != "SFV") && (value.toString() != "MD5")) {
                p_error = Error(tr("The hashlist file format is unknown."), tr("Your given hashlist file format is unknown. Please choose on of these formats: SFV, MD5."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_HL_NAME_INDEX:
            break;
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Hashlist name must not be empty."), tr("You have given no name for the hashlist. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_CUE_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_CUE_NAME_INDEX:
            break;
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Cue filename name must not be empty."), tr("You have given no name for the cue sheet. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_SF_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_SF_NAME_INDEX:
            break;
            if (value.toString().isEmpty()) {
                p_error = Error(tr("Filename name must not be empty."), tr("You have given no name for the single audio file. Please set one."), Error::ERROR, this);
                endResetModel();
                return false;
            }
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_INDEX:
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_INDEX:
            break;
        default:
            endResetModel();
            return false;
        }

        // check if name is unique
        if (index.column() == PROFILE_MODEL_COLUMN_NAME_INDEX) {
            bool found = false;
            for (int i = 0; i < p_cache.count(); ++i) {
                if (i == index.row())
                    continue;
                if (value.toString() == p_cache.at(i)[PROFILE_MODEL_NAME_KEY].toString()) {
                    found = true;
                    break;
                }
            }
            if (found) {
                p_error = Error(tr("Profile name already exists."), tr("Your profile name %1 already exists in the set of profiles. Please choose a unique one.").arg(value.toString()), Error::ERROR, this);
                endResetModel();
                return false;
            }
        }

        // check if profile index is unique
        if ((index.column() == PROFILE_MODEL_COLUMN_PROFILEINDEX_INDEX) && (value.toInt() != -1)) {
            bool found = false;
            for (int i = 0; i < p_cache.count(); ++i) {
                if (i == index.row())
                    continue;
                if (value.toInt() == p_cache.at(i)[PROFILE_MODEL_PROFILEINDEX_KEY].toInt()) {
                    found = true;
                    break;
                }
            }
            if (found) {
                p_error = Error(tr("Profile index already exists."), tr("Your profile index %1 already exists in the set of profiles. Please choose a unique one.").arg(value.toInt()), Error::ERROR, this);
                endResetModel();
                return false;
            }
        }

        switch (index.column()) {
        case PROFILE_MODEL_COLUMN_PROFILEINDEX_INDEX:
            p_cache[index.row()][PROFILE_MODEL_PROFILEINDEX_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_NAME_INDEX:
            p_cache[index.row()][PROFILE_MODEL_NAME_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_ICON_INDEX:
            p_cache[index.row()][PROFILE_MODEL_ICON_KEY] = value;
            break;

        case PROFILE_MODEL_COLUMN_ENCODER_SELECTED_INDEX:
            p_cache[index.row()][PROFILE_MODEL_ENCODER_SELECTED_KEY] = value;
            break;

        case PROFILE_MODEL_COLUMN_SCHEME_INDEX:
            p_cache[index.row()][PROFILE_MODEL_SCHEME_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_FAT32COMPATIBLE_INDEX:
            p_cache[index.row()][PROFILE_MODEL_FAT32COMPATIBLE_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_UNDERSCORE_INDEX:
            p_cache[index.row()][PROFILE_MODEL_UNDERSCORE_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_2DIGITSTRACKNUM_INDEX:
            p_cache[index.row()][PROFILE_MODEL_2DIGITSTRACKNUM_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_SC_INDEX:
            p_cache[index.row()][PROFILE_MODEL_SC_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_SC_SCALE_INDEX:
            p_cache[index.row()][PROFILE_MODEL_SC_SCALE_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_SC_SIZE_INDEX:
            p_cache[index.row()][PROFILE_MODEL_SC_SIZE_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_SC_FORMAT_INDEX:
            p_cache[index.row()][PROFILE_MODEL_SC_FORMAT_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_SC_NAME_INDEX:
            p_cache[index.row()][PROFILE_MODEL_SC_NAME_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_PL_INDEX:
            p_cache[index.row()][PROFILE_MODEL_PL_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_PL_FORMAT_INDEX:
            p_cache[index.row()][PROFILE_MODEL_PL_FORMAT_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_PL_NAME_INDEX:
            p_cache[index.row()][PROFILE_MODEL_PL_NAME_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_PL_ABS_FILE_PATH_INDEX:
            p_cache[index.row()][PROFILE_MODEL_PL_ABS_FILE_PATH_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_PL_UTF8_INDEX:
            p_cache[index.row()][PROFILE_MODEL_PL_UTF8_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_INF_INDEX:
            p_cache[index.row()][PROFILE_MODEL_INF_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_INF_TEXT_INDEX:
            p_cache[index.row()][PROFILE_MODEL_INF_TEXT_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_INF_NAME_INDEX:
            p_cache[index.row()][PROFILE_MODEL_INF_NAME_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_INF_SUFFIX_INDEX:
            p_cache[index.row()][PROFILE_MODEL_INF_SUFFIX_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_HL_INDEX:
            p_cache[index.row()][PROFILE_MODEL_HL_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_HL_FORMAT_INDEX:
            p_cache[index.row()][PROFILE_MODEL_HL_FORMAT_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_HL_NAME_INDEX:
            p_cache[index.row()][PROFILE_MODEL_HL_NAME_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_CUE_INDEX:
            p_cache[index.row()][PROFILE_MODEL_CUE_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_CUE_NAME_INDEX:
            p_cache[index.row()][PROFILE_MODEL_CUE_NAME_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_SF_INDEX:
            p_cache[index.row()][PROFILE_MODEL_SF_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_SF_NAME_INDEX:
            p_cache[index.row()][PROFILE_MODEL_SF_NAME_KEY] = value;
            break;

        case PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_INDEX:
            p_cache[index.row()][PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_INDEX:
            p_cache[index.row()][PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_INDEX:
            p_cache[index.row()][PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_INDEX:
            p_cache[index.row()][PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_INDEX:
            p_cache[index.row()][PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_KEY] = value;
            break;
        case PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_INDEX:
            p_cache[index.row()][PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_KEY] = value;
            break;

        default:
            break;
        }
        endResetModel();
        return true;
    }

    endResetModel();

    p_error = Error(tr("Unknown error. No index found in profile model."), tr("This is an internal error. Please report."), Error::ERROR, this);

    return false;
}

bool ProfileModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if ((row < 0) || (row >= p_cache.count())) {
        return false;
    }

    if (count <= 0) {
        return false;
    }

    beginResetModel();
    int c;
    if (row + count > p_cache.count()) {
        c = p_cache.count();
    } else {
        c = row + count;
    }

    for (int i = row; i < c; ++i) {
        p_cache.removeAt(i);
    }

    endResetModel();

    // update current profile index. maybe current has been deleted?
    setCurrentProfileIndex(p_current_profile_index);

    Q_EMIT profilesRemovedOrInserted();

    return true;
}

bool ProfileModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if ((row < 0) || (row > p_cache.count())) {
        return false;
    }

    if (count <= 0) {
        return false;
    }

    bool wasEmpty = (p_cache.count() == 0);

    beginResetModel();

    if (row == p_cache.count()) {
        for (int i = 0; i < count; ++i)
            p_cache.append(p_new_profile());
    } else {
        for (int i = row; i < count; ++i)
            p_cache.insert(i, p_new_profile());
    }

    endResetModel();

    if (wasEmpty) {
        // set first profile as current index
        setCurrentProfileIndex(p_cache.at(0)[PROFILE_MODEL_PROFILEINDEX_KEY].toInt());
    }

    Q_EMIT profilesRemovedOrInserted();

    return true;
}

void ProfileModel::setCurrentProfileIndex(int profile_index)
{
    int pi = profile_index;
    if (p_cache.count() == 0) {
        pi = -1;
    } else if (!indexExists(profile_index)) {
        // set first profile as current index
        pi = p_cache.at(0)[PROFILE_MODEL_PROFILEINDEX_KEY].toInt();
    }
    if (pi != p_current_profile_index) {
        p_current_profile_index = pi;
        QSettings settings;
        settings.beginGroup("Profiles");
        settings.setValue("standard", pi);
        settings.sync();
        Q_EMIT currentProfileIndexChanged(pi);
    }
}

int ProfileModel::setRowAsCurrentProfileIndex(int row)
{
    int i = p_cache.at(row).value(PROFILE_MODEL_PROFILEINDEX_KEY, -1).toInt();
    setCurrentProfileIndex(i);
    return i;
}

int ProfileModel::currentProfileIndex() const
{
    if (p_cache.count() == 0)
        return -1;
    return p_current_profile_index;
}

int ProfileModel::currentProfileRow() const
{
    return getRowByIndex(p_current_profile_index);
}

int ProfileModel::getRowByIndex(int profile_index) const
{
    for (int i = 0; i < p_cache.count(); ++i)
        if (profile_index == p_cache.at(i)[PROFILE_MODEL_PROFILEINDEX_KEY].toInt())
            return i;
    return -1;
}

void ProfileModel::clear()
{
    p_cache.clear();
    p_current_profile_index = -1;
}

bool ProfileModel::nameExists(const QString &name) const
{
    for (int j = 0; j < p_cache.count(); ++j)
        if (name == p_cache.at(j)[PROFILE_MODEL_NAME_KEY].toString())
            return true;

    return false;
}

bool ProfileModel::indexExists(int profile_index) const
{
    for (int j = 0; j < p_cache.count(); ++j)
        if (profile_index == p_cache.at(j)[PROFILE_MODEL_PROFILEINDEX_KEY].toInt())
            return true;

    return false;
}

int ProfileModel::getNewIndex() const
{
    QSet<int> indexes;
    QList<Profile>::ConstIterator it(p_cache.begin()), end(p_cache.end());

    for (; it != end; ++it) {
        indexes.insert((*it)[PROFILE_MODEL_PROFILEINDEX_KEY].toInt());
    }
    for (int i = 0; i < INT_MAX; ++i) {
        if (!indexes.contains(i))
            return i;
    }
    return -1;
}

static bool lessThan(const Profile &p1, const Profile &p2)
{
    return (QString::localeAwareCompare(p1[PROFILE_MODEL_NAME_KEY].toString(), p2[PROFILE_MODEL_NAME_KEY].toString()) < 0);
}

void ProfileModel::sortItems()
{
    beginResetModel();
    std::sort(p_cache.begin(), p_cache.end(), lessThan);
    endResetModel();
    Q_EMIT profilesRemovedOrInserted();
}

void ProfileModel::autoCreate()
{
    bool flag = false;

    bool wasEmpty = (p_cache.count() == 0);

    if (EncoderAssistant::available(EncoderAssistant::LAME)) {
        if (!nameExists(EncoderAssistant::name(EncoderAssistant::LAME) + tr(LABEL_MOBILE_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::LAME) + tr(LABEL_MOBILE_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::LAME);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::LAME;
            p[PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::LAME, EncoderAssistant::MOBILE).toString();
            p_cache.append(p);
            flag = true;
        }

        if (!nameExists(EncoderAssistant::name(EncoderAssistant::LAME) + tr(LABEL_NORMAL_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::LAME) + tr(LABEL_NORMAL_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::LAME);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::LAME;
            p[PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::LAME, EncoderAssistant::NORMAL).toString();
            p_cache.append(p);
            flag = true;
        }

        if (!nameExists(EncoderAssistant::name(EncoderAssistant::LAME) + tr(LABEL_EXTREME_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::LAME) + tr(LABEL_EXTREME_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::LAME);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::LAME;
            p[PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::LAME, EncoderAssistant::EXTREME).toString();
            ;
            p_cache.append(p);
            flag = true;
        }
    }

    if (EncoderAssistant::available(EncoderAssistant::OGGENC)) {
        if (!nameExists(EncoderAssistant::name(EncoderAssistant::OGGENC) + tr(LABEL_MOBILE_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::OGGENC) + tr(LABEL_MOBILE_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::OGGENC);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::OGGENC;
            p[PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::OGGENC, EncoderAssistant::MOBILE).toString();
            p_cache.append(p);
            flag = true;
        }

        if (!nameExists(EncoderAssistant::name(EncoderAssistant::OGGENC) + tr(LABEL_NORMAL_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::OGGENC) + tr(LABEL_NORMAL_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::OGGENC);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::OGGENC;
            p[PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::OGGENC, EncoderAssistant::NORMAL).toString();
            p_cache.append(p);
            flag = true;
        }

        if (!nameExists(EncoderAssistant::name(EncoderAssistant::OGGENC) + tr(LABEL_EXTREME_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::OGGENC) + tr(LABEL_EXTREME_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::OGGENC);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::OGGENC;
            p[PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::OGGENC, EncoderAssistant::EXTREME).toString();
            p_cache.append(p);
            flag = true;
        }
    }

    if ((!nameExists(EncoderAssistant::name(EncoderAssistant::FLAC))) && (EncoderAssistant::available(EncoderAssistant::FLAC))) {
        Profile p = p_new_profile();
        p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::FLAC);
        p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::FLAC);
        p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::FLAC;
        p_cache.append(p);
        flag = true;
    }

    if (EncoderAssistant::available(EncoderAssistant::FAAC)) {
        if (!nameExists(EncoderAssistant::name(EncoderAssistant::FAAC) + tr(LABEL_MOBILE_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::FAAC) + tr(LABEL_MOBILE_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::FAAC);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::FAAC;
            p[PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::FAAC, EncoderAssistant::MOBILE).toString();
            p_cache.append(p);
            flag = true;
        }

        if (!nameExists(EncoderAssistant::name(EncoderAssistant::FAAC) + tr(LABEL_NORMAL_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::FAAC) + tr(LABEL_NORMAL_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::FAAC);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::FAAC;
            p[PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::FAAC, EncoderAssistant::NORMAL).toString();
            p_cache.append(p);
            flag = true;
        }

        if (!nameExists(EncoderAssistant::name(EncoderAssistant::FAAC) + tr(LABEL_EXTREME_QUALITY))) {
            Profile p = p_new_profile();
            p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::FAAC) + tr(LABEL_EXTREME_QUALITY);
            p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::FAAC);
            p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::FAAC;
            p[PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY] = EncoderAssistant::stdParameters(EncoderAssistant::FAAC, EncoderAssistant::EXTREME).toString();
            p_cache.append(p);
            flag = true;
        }
    }

    if ((!nameExists(EncoderAssistant::name(EncoderAssistant::WAVE))) && (EncoderAssistant::available(EncoderAssistant::WAVE))) {
        Profile p = p_new_profile();
        p[PROFILE_MODEL_NAME_KEY] = EncoderAssistant::name(EncoderAssistant::WAVE);
        p[PROFILE_MODEL_ICON_KEY] = EncoderAssistant::icon(EncoderAssistant::WAVE);
        p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = (int)EncoderAssistant::WAVE;
        p_cache.append(p);
        flag = true;
    }

    if (flag) {
        sortItems();
        if (wasEmpty) {
            // set first profile as current index
            setCurrentProfileIndex(p_cache.at(0)[PROFILE_MODEL_PROFILEINDEX_KEY].toInt());
        }
    }

    commit();
}

EncoderAssistant::Encoder ProfileModel::getSelectedEncoderFromCurrentIndex()
{
    return (EncoderAssistant::Encoder)data(index(currentProfileRow(), PROFILE_MODEL_COLUMN_ENCODER_SELECTED_INDEX)).toInt();
}

const Parameters ProfileModel::getSelectedEncoderParametersFromCurrentIndex()
{
    Parameters parameters;

    EncoderAssistant::Encoder encoder = getSelectedEncoderFromCurrentIndex();
    // what parameters does the encoder start with?
    switch (encoder) {
    case EncoderAssistant::LAME:
        parameters.fromString(data(index(currentProfileRow(), PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_INDEX)).toString());
        break;
    case EncoderAssistant::OGGENC:
        parameters.fromString(data(index(currentProfileRow(), PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_INDEX)).toString());
        break;
    case EncoderAssistant::FLAC:
        parameters.fromString(data(index(currentProfileRow(), PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_INDEX)).toString());
        break;
    case EncoderAssistant::FAAC:
        parameters.fromString(data(index(currentProfileRow(), PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_INDEX)).toString());
        break;
    case EncoderAssistant::WAVE:
        parameters.fromString(data(index(currentProfileRow(), PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_INDEX)).toString());
        break;
    case EncoderAssistant::CUSTOM:
        parameters.fromString(data(index(currentProfileRow(), PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_INDEX)).toString());
        break;
    case EncoderAssistant::NUM:;
    }

    return parameters;
}

QStringList ProfileModel::getSelectedEncoderSchemeFromCurrentIndex()
{
    EncoderAssistant::Encoder encoder = getSelectedEncoderFromCurrentIndex();
    Parameters parameters = getSelectedEncoderParametersFromCurrentIndex();

    return EncoderAssistant::scheme(encoder, parameters);
}

const QString ProfileModel::getSelectedEncoderSuffixFromCurrentIndex()
{
    EncoderAssistant::Encoder encoder = getSelectedEncoderFromCurrentIndex();
    Parameters parameters = getSelectedEncoderParametersFromCurrentIndex();

    switch (encoder) {
    case EncoderAssistant::LAME:
        return parameters.value(ENCODER_LAME_SUFFIX_KEY, ENCODER_LAME_SUFFIX).toString();
    case EncoderAssistant::OGGENC:
        return parameters.value(ENCODER_OGGENC_SUFFIX_KEY, ENCODER_OGGENC_SUFFIX).toString();
    case EncoderAssistant::FLAC:
        return parameters.value(ENCODER_FLAC_SUFFIX_KEY, ENCODER_FLAC_SUFFIX).toString();
    case EncoderAssistant::FAAC:
        return parameters.value(ENCODER_FAAC_SUFFIX_KEY, ENCODER_FAAC_SUFFIX).toString();
    case EncoderAssistant::WAVE:
        return parameters.value(ENCODER_WAVE_SUFFIX_KEY, ENCODER_WAVE_SUFFIX).toString();
    case EncoderAssistant::CUSTOM:
        return parameters.value(ENCODER_CUSTOM_SUFFIX_KEY, ENCODER_CUSTOM_SUFFIX).toString();
    case EncoderAssistant::NUM:
        return "";
    }

    return "";
}

const QString ProfileModel::getSelectedEncoderNameAndVersion()
{
    EncoderAssistant::Encoder encoder = getSelectedEncoderFromCurrentIndex();

    return QString("%1 %2").arg(EncoderAssistant::encoderName(encoder), EncoderAssistant::version(encoder));
}

Error ProfileModel::lastError() const
{
    return p_error;
}

void ProfileModel::commit()
{
    QSettings settings;
    p_save(settings);
}

const Profile ProfileModel::p_new_profile()
{
    Profile p;

    p[PROFILE_MODEL_PROFILEINDEX_KEY] = getNewIndex();
    p[PROFILE_MODEL_NAME_KEY] = DEFAULT_NAME;

    p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = DEFAULT_ENCODER_SELECTED;

    p[PROFILE_MODEL_SCHEME_KEY] = DEFAULT_SCHEME;

    p[PROFILE_MODEL_FAT32COMPATIBLE_KEY] = DEFAULT_FAT32;
    p[PROFILE_MODEL_UNDERSCORE_KEY] = DEFAULT_UNDERSCORE;
    p[PROFILE_MODEL_2DIGITSTRACKNUM_KEY] = DEFAULT_2DIGITSTRACKNUM;

    p[PROFILE_MODEL_SC_KEY] = DEFAULT_SC;
    p[PROFILE_MODEL_SC_SCALE_KEY] = DEFAULT_SC_SCALE;
    p[PROFILE_MODEL_SC_SIZE_KEY] = DEFAULT_SC_SIZE;
    p[PROFILE_MODEL_SC_FORMAT_KEY] = DEFAULT_SC_FORMAT;
    p[PROFILE_MODEL_SC_NAME_KEY] = DEFAULT_SC_NAME;
    p[PROFILE_MODEL_PL_KEY] = DEFAULT_PL;
    p[PROFILE_MODEL_PL_FORMAT_KEY] = DEFAULT_PL_FORMAT;
    p[PROFILE_MODEL_PL_NAME_KEY] = DEFAULT_PL_NAME;
    p[PROFILE_MODEL_PL_ABS_FILE_PATH_KEY] = DEFAULT_PL_ABS_FILE_PATH;
    p[PROFILE_MODEL_PL_UTF8_KEY] = DEFAULT_PL_UTF8;
    p[PROFILE_MODEL_INF_KEY] = DEFAULT_INF;
    p[PROFILE_MODEL_INF_TEXT_KEY] = DEFAULT_INF_TEXT;
    p[PROFILE_MODEL_INF_NAME_KEY] = DEFAULT_INF_NAME;
    p[PROFILE_MODEL_INF_SUFFIX_KEY] = DEFAULT_INF_SUFFIX;
    p[PROFILE_MODEL_HL_KEY] = DEFAULT_HL;
    p[PROFILE_MODEL_HL_FORMAT_KEY] = DEFAULT_HL_FORMAT;
    p[PROFILE_MODEL_HL_NAME_KEY] = DEFAULT_HL_NAME;
    p[PROFILE_MODEL_CUE_KEY] = DEFAULT_CUE;
    p[PROFILE_MODEL_CUE_NAME_KEY] = DEFAULT_CUE_NAME;
    p[PROFILE_MODEL_SF_KEY] = DEFAULT_SF;
    p[PROFILE_MODEL_SF_NAME_KEY] = DEFAULT_SF_NAME;

    p[PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY] = DEFAULT_ENCODER_PARAMETERS;
    p[PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY] = DEFAULT_ENCODER_PARAMETERS;
    p[PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_KEY] = DEFAULT_ENCODER_PARAMETERS;
    p[PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY] = DEFAULT_ENCODER_PARAMETERS;
    p[PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_KEY] = DEFAULT_ENCODER_PARAMETERS;
    p[PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_KEY] = DEFAULT_ENCODER_PARAMETERS;

    return p;
}

void ProfileModel::p_new_name(QString &name)
{
    for (int j = 0; j < p_cache.count(); ++j) {
        if (name == p_cache.at(j)[PROFILE_MODEL_NAME_KEY].toString()) {
            name = '_' + name;
            p_new_name(name);
            return;
        }
    }
}

void ProfileModel::revert()
{
    clear();
    QSettings settings;
    p_load(settings);
}

int ProfileModel::copy(const int profileRow)
{
    beginResetModel();
    if ((profileRow < 0) || (profileRow >= rowCount()))
        return -1;

    int key = getNewIndex();
    Profile p = p_cache[profileRow];

    QString name = p_cache[profileRow][PROFILE_MODEL_NAME_KEY].toString();
    p_new_name(name);
    p[PROFILE_MODEL_NAME_KEY] = name;
    p[PROFILE_MODEL_PROFILEINDEX_KEY] = key;
    p_cache.append(p);

    endResetModel();
    Q_EMIT profilesRemovedOrInserted();

    return key;
}

bool ProfileModel::saveProfilesToFile(const QString &filename)
{
    QSettings settings(filename, QSettings::IniFormat);
    p_save(settings);
    return true;
}

bool ProfileModel::loadProfilesFromFile(const QString &filename)
{
    QSettings settings(filename, QSettings::IniFormat);
    beginResetModel();
    p_load(settings);
    endResetModel();
    commit();
    return true;
}

void ProfileModel::p_save(QSettings& settings)
{
    settings.beginGroup("Profiles");
    for (auto& key : settings.allKeys())
      settings.remove(key);

    settings.setValue("standard", p_current_profile_index);
    settings.setValue("count", p_cache.count());

    for (int i = 0; i < p_cache.count(); ++i) {
        settings.beginGroup(QString("Profile_%1").arg(i));

        settings.setValue(PROFILE_MODEL_PROFILEINDEX_KEY, p_cache[i][PROFILE_MODEL_PROFILEINDEX_KEY]);
        settings.setValue(PROFILE_MODEL_NAME_KEY, p_cache[i][PROFILE_MODEL_NAME_KEY]);
        settings.setValue(PROFILE_MODEL_ICON_KEY, p_cache[i][PROFILE_MODEL_ICON_KEY]);

        settings.setValue(PROFILE_MODEL_ENCODER_SELECTED_KEY, p_cache[i][PROFILE_MODEL_ENCODER_SELECTED_KEY]);

        settings.setValue(PROFILE_MODEL_SCHEME_KEY, p_cache[i][PROFILE_MODEL_SCHEME_KEY]);
        settings.setValue(PROFILE_MODEL_FAT32COMPATIBLE_KEY, p_cache[i][PROFILE_MODEL_FAT32COMPATIBLE_KEY]);
        settings.setValue(PROFILE_MODEL_UNDERSCORE_KEY, p_cache[i][PROFILE_MODEL_UNDERSCORE_KEY]);
        settings.setValue(PROFILE_MODEL_2DIGITSTRACKNUM_KEY, p_cache[i][PROFILE_MODEL_2DIGITSTRACKNUM_KEY]);

        settings.setValue(PROFILE_MODEL_SC_KEY, p_cache[i][PROFILE_MODEL_SC_KEY]);
        settings.setValue(PROFILE_MODEL_SC_SCALE_KEY, p_cache[i][PROFILE_MODEL_SC_SCALE_KEY]);
        settings.setValue(PROFILE_MODEL_SC_SIZE_KEY, p_cache[i][PROFILE_MODEL_SC_SIZE_KEY]);
        settings.setValue(PROFILE_MODEL_SC_FORMAT_KEY, p_cache[i][PROFILE_MODEL_SC_FORMAT_KEY]);
        settings.setValue(PROFILE_MODEL_SC_NAME_KEY, p_cache[i][PROFILE_MODEL_SC_NAME_KEY]);
        settings.setValue(PROFILE_MODEL_PL_KEY, p_cache[i][PROFILE_MODEL_PL_KEY]);
        settings.setValue(PROFILE_MODEL_PL_FORMAT_KEY, p_cache[i][PROFILE_MODEL_PL_FORMAT_KEY]);
        settings.setValue(PROFILE_MODEL_PL_NAME_KEY, p_cache[i][PROFILE_MODEL_PL_NAME_KEY]);
        settings.setValue(PROFILE_MODEL_PL_ABS_FILE_PATH_KEY, p_cache[i][PROFILE_MODEL_PL_ABS_FILE_PATH_KEY]);
        settings.setValue(PROFILE_MODEL_PL_UTF8_KEY, p_cache[i][PROFILE_MODEL_PL_UTF8_KEY]);
        settings.setValue(PROFILE_MODEL_INF_KEY, p_cache[i][PROFILE_MODEL_INF_KEY]);
        settings.setValue(PROFILE_MODEL_INF_TEXT_KEY, p_cache[i][PROFILE_MODEL_INF_TEXT_KEY]);
        settings.setValue(PROFILE_MODEL_INF_NAME_KEY, p_cache[i][PROFILE_MODEL_INF_NAME_KEY]);
        settings.setValue(PROFILE_MODEL_INF_SUFFIX_KEY, p_cache[i][PROFILE_MODEL_INF_SUFFIX_KEY]);
        settings.setValue(PROFILE_MODEL_HL_KEY, p_cache[i][PROFILE_MODEL_HL_KEY]);
        settings.setValue(PROFILE_MODEL_HL_FORMAT_KEY, p_cache[i][PROFILE_MODEL_HL_FORMAT_KEY]);
        settings.setValue(PROFILE_MODEL_HL_NAME_KEY, p_cache[i][PROFILE_MODEL_HL_NAME_KEY]);
        settings.setValue(PROFILE_MODEL_CUE_KEY, p_cache[i][PROFILE_MODEL_CUE_KEY]);
        settings.setValue(PROFILE_MODEL_CUE_NAME_KEY, p_cache[i][PROFILE_MODEL_CUE_NAME_KEY]);
        settings.setValue(PROFILE_MODEL_SF_KEY, p_cache[i][PROFILE_MODEL_SF_KEY]);
        settings.setValue(PROFILE_MODEL_SF_NAME_KEY, p_cache[i][PROFILE_MODEL_SF_NAME_KEY]);

        settings.setValue(PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY, p_cache[i][PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY]);
        settings.setValue(PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY, p_cache[i][PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY]);
        settings.setValue(PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_KEY, p_cache[i][PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_KEY]);
        settings.setValue(PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY, p_cache[i][PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY]);
        settings.setValue(PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_KEY, p_cache[i][PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_KEY]);
        settings.setValue(PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_KEY, p_cache[i][PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_KEY]);

        settings.endGroup();
    }

    if (p_cache.count() > 0)
        settings.sync();
}

void ProfileModel::p_load(QSettings& settings)
{
    settings.beginGroup("Profiles");
    clear();
    p_current_profile_index = settings.value("standard", QVariant(-1)).toInt();
    int profileCount = settings.value("count", QVariant(0)).toInt();

    for (int i = 0; i < profileCount; ++i) {
        Profile p;
        settings.beginGroup(QString("Profile_%1").arg(i));

        p[PROFILE_MODEL_PROFILEINDEX_KEY] = settings.value(PROFILE_MODEL_PROFILEINDEX_KEY, DEFAULT_PROFILEINDEX);
        p[PROFILE_MODEL_NAME_KEY] = settings.value(PROFILE_MODEL_NAME_KEY, DEFAULT_NAME);
        p[PROFILE_MODEL_ICON_KEY] = settings.value(PROFILE_MODEL_ICON_KEY, DEFAULT_ICON);

        p[PROFILE_MODEL_ENCODER_SELECTED_KEY] = settings.value(PROFILE_MODEL_ENCODER_SELECTED_KEY, DEFAULT_ENCODER_SELECTED);

        p[PROFILE_MODEL_SCHEME_KEY] = settings.value(PROFILE_MODEL_SCHEME_KEY, DEFAULT_SCHEME);
        p[PROFILE_MODEL_FAT32COMPATIBLE_KEY] = settings.value(PROFILE_MODEL_FAT32COMPATIBLE_KEY, DEFAULT_FAT32);
        p[PROFILE_MODEL_UNDERSCORE_KEY] = settings.value(PROFILE_MODEL_UNDERSCORE_KEY, DEFAULT_UNDERSCORE);
        p[PROFILE_MODEL_2DIGITSTRACKNUM_KEY] = settings.value(PROFILE_MODEL_2DIGITSTRACKNUM_KEY, DEFAULT_2DIGITSTRACKNUM);

        p[PROFILE_MODEL_SC_KEY] = settings.value(PROFILE_MODEL_SC_KEY, DEFAULT_SC);
        p[PROFILE_MODEL_SC_SCALE_KEY] = settings.value(PROFILE_MODEL_SC_SCALE_KEY, DEFAULT_SC_SCALE);
        p[PROFILE_MODEL_SC_SIZE_KEY] = settings.value(PROFILE_MODEL_SC_SIZE_KEY, DEFAULT_SC_SIZE);
        p[PROFILE_MODEL_SC_FORMAT_KEY] = settings.value(PROFILE_MODEL_SC_FORMAT_KEY, DEFAULT_SC_FORMAT);
        p[PROFILE_MODEL_SC_NAME_KEY] = settings.value(PROFILE_MODEL_SC_NAME_KEY, DEFAULT_SC_NAME);
        p[PROFILE_MODEL_PL_KEY] = settings.value(PROFILE_MODEL_PL_KEY, DEFAULT_PL);
        p[PROFILE_MODEL_PL_FORMAT_KEY] = settings.value(PROFILE_MODEL_PL_FORMAT_KEY, DEFAULT_PL_NAME);
        p[PROFILE_MODEL_PL_NAME_KEY] = settings.value(PROFILE_MODEL_PL_NAME_KEY, DEFAULT_PL_NAME);
        p[PROFILE_MODEL_PL_ABS_FILE_PATH_KEY] = settings.value(PROFILE_MODEL_PL_ABS_FILE_PATH_KEY, DEFAULT_PL_ABS_FILE_PATH);
        p[PROFILE_MODEL_PL_UTF8_KEY] = settings.value(PROFILE_MODEL_PL_UTF8_KEY, DEFAULT_PL_UTF8);
        p[PROFILE_MODEL_INF_KEY] = settings.value(PROFILE_MODEL_INF_KEY, DEFAULT_INF);
        p[PROFILE_MODEL_INF_TEXT_KEY] = settings.value(PROFILE_MODEL_INF_TEXT_KEY, DEFAULT_INF_TEXT);
        p[PROFILE_MODEL_INF_NAME_KEY] = settings.value(PROFILE_MODEL_INF_NAME_KEY, DEFAULT_INF_NAME);
        p[PROFILE_MODEL_INF_SUFFIX_KEY] = settings.value(PROFILE_MODEL_INF_SUFFIX_KEY, DEFAULT_INF_SUFFIX);
        p[PROFILE_MODEL_HL_KEY] = settings.value(PROFILE_MODEL_HL_KEY, DEFAULT_HL);
        p[PROFILE_MODEL_HL_FORMAT_KEY] = settings.value(PROFILE_MODEL_HL_FORMAT_KEY, DEFAULT_HL_FORMAT);
        p[PROFILE_MODEL_HL_NAME_KEY] = settings.value(PROFILE_MODEL_HL_NAME_KEY, DEFAULT_HL_NAME);
        p[PROFILE_MODEL_CUE_KEY] = settings.value(PROFILE_MODEL_CUE_KEY, DEFAULT_CUE);
        p[PROFILE_MODEL_CUE_NAME_KEY] = settings.value(PROFILE_MODEL_CUE_NAME_KEY, DEFAULT_CUE_NAME);
        p[PROFILE_MODEL_SF_KEY] = settings.value(PROFILE_MODEL_SF_KEY, DEFAULT_SF);
        p[PROFILE_MODEL_SF_NAME_KEY] = settings.value(PROFILE_MODEL_SF_NAME_KEY, DEFAULT_SF_NAME);

        p[PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY] = settings.value(PROFILE_MODEL_COLUMN_ENCODER_LAME_PARAMETERS_KEY, DEFAULT_ENCODER_PARAMETERS);
        p[PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY] = settings.value(PROFILE_MODEL_COLUMN_ENCODER_OGGENC_PARAMETERS_KEY, DEFAULT_ENCODER_PARAMETERS);
        p[PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_KEY] = settings.value(PROFILE_MODEL_COLUMN_ENCODER_FLAC_PARAMETERS_KEY, DEFAULT_ENCODER_PARAMETERS);
        p[PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY] = settings.value(PROFILE_MODEL_COLUMN_ENCODER_FAAC_PARAMETERS_KEY, DEFAULT_ENCODER_PARAMETERS);
        p[PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_KEY] = settings.value(PROFILE_MODEL_COLUMN_ENCODER_WAVE_PARAMETERS_KEY, DEFAULT_ENCODER_PARAMETERS);
        p[PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_KEY] = settings.value(PROFILE_MODEL_COLUMN_ENCODER_CUSTOM_PARAMETERS_KEY, DEFAULT_ENCODER_PARAMETERS);

        settings.endGroup();
        p_cache.append(p);
    }

    if (profileCount > 0) {
        if (p_current_profile_index == -1)
            p_current_profile_index = 0;
        Q_EMIT profilesRemovedOrInserted();
    }
}
