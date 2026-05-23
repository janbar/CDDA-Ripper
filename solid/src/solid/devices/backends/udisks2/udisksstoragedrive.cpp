/*
    SPDX-FileCopyrightText: 2010 Michael Zanetti <mzanetti@kde.org>
    SPDX-FileCopyrightText: 2010-2012 Lukáš Tinkl <ltinkl@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "udisksstoragedrive.h"

#include <QDebug>

using namespace Solid::Backends::UDisks2;

StorageDrive::StorageDrive(Device *dev)
    : Block(dev)
{
}

StorageDrive::~StorageDrive()
{
}

qulonglong StorageDrive::size() const
{
    return m_device->prop("Size").toULongLong();
}

bool StorageDrive::isRemovable() const
{
    return m_device->prop("MediaRemovable").toBool() || m_device->prop("Removable").toBool();
}

Solid::StorageDrive::DriveType StorageDrive::driveType() const
{
    const QStringList mediaTypes = m_device->prop("MediaCompatibility").toStringList();

    if (m_device->isOpticalDrive()) { // optical disks
        return Solid::StorageDrive::CdromDrive;
    } else if (mediaTypes.contains("floppy")) {
        return Solid::StorageDrive::Floppy;
    }
#if 0 // TODO add to Solid
    else if (mediaTypes.contains("floppy_jaz")) {
        return Solid::StorageDrive::Jaz;
    } else if (mediaTypes.contains("floppy_zip")) {
        return Solid::StorageDrive::Zip;
    } else if (mediaTypes.contains("flash")) {
        return Solid::StorageDrive::Flash;
    }
#endif
    else if (mediaTypes.contains("flash_cf")) {
        return Solid::StorageDrive::CompactFlash;
    } else if (mediaTypes.contains("flash_ms")) {
        return Solid::StorageDrive::MemoryStick;
    } else if (mediaTypes.contains("flash_sm")) {
        return Solid::StorageDrive::SmartMedia;
    } else if (mediaTypes.contains("flash_sd") //
               || mediaTypes.contains("flash_sdhc") //
               || mediaTypes.contains("flash_mmc") //
               || mediaTypes.contains("flash_sdxc")) {
        return Solid::StorageDrive::SdMmc;
    }
    // FIXME: udisks2 doesn't know about xD cards
    else {
        return Solid::StorageDrive::HardDisk;
    }
}

#include "moc_udisksstoragedrive.cpp"
