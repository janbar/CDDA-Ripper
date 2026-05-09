set(backend_sources
    udevdevice.cpp
    udevmanager.cpp
    udevdeviceinterface.cpp
    udevgenericinterface.cpp
    udevblock.cpp
    ../shared/udevqtclient.cpp
    ../shared/udevqtdevice.cpp
)
set(backend_libs udev)

set(UDEV_DETAILED_OUTPUT OFF CACHE BOOL "provide extended output regarding udev events")
if(UDEV_DETAILED_OUTPUT)
    add_definitions(-DUDEV_DETAILED_OUTPUT)
endif()
