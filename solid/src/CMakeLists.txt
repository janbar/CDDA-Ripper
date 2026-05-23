
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(QT_VERSION_PREFERRED "preferred Qt version")
include(qtchooser)
if(NOT QT_FOUND)
    message(FATAL_ERROR "Qt${QT_VERSION_PREFERRED} NOT found")
endif()

find_package(Qt${QT_VERSION_MAJOR} ${QT_VERSION} COMPONENTS
    Core Xml Gui DBus REQUIRED)

set(CMAKE_AUTOMOC ON)

add_subdirectory(solid)
#add_subdirectory(imports)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/Block
    CONTENT "#include \"solid/block.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/Device
    CONTENT "#include \"solid/device.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/DeviceInterface
    CONTENT "#include \"solid/deviceinterface.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/DeviceNotifier
    CONTENT "#include \"solid/devicenotifier.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/GenericInterface
    CONTENT "#include \"solid/genericinterface.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/OpticalDisc
    CONTENT "#include \"solid/opticaldisc.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/OpticalDrive
    CONTENT "#include \"solid/opticaldrive.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/Predicate
    CONTENT "#include \"solid/predicate.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/SolidNamespace
    CONTENT "#include \"solid/solidnamespace.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/StorageDrive
    CONTENT "#include \"solid/storagedrive.h\"" )
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Solid/StorageVolume
    CONTENT "#include \"solid/storagevolume.h\"" )
