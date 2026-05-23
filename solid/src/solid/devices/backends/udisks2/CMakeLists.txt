if (LibMount_FOUND)
    set(backend_libs libmount)
endif()

set(backend_sources
    udisksmanager.cpp
    udisksdevice.cpp
    udisksdevicebackend.cpp
    udisksblock.cpp
    udisksstoragevolume.cpp
    udisksdeviceinterface.cpp
    udisksopticaldisc.cpp
    udisksopticaldrive.cpp
    udisksstoragedrive.cpp
    udisksgenericinterface.cpp
    dbus/manager.cpp
)
