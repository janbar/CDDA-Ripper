
set(solid_BUILD_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

include_directories(${solid_BUILD_INCLUDE_DIRS})
include(devices/CMakeLists.txt)

set(solid_LIB_SRCS ${solid_LIB_SRCS})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/solid_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/solid_export.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/block.h
    ${CMAKE_CURRENT_BINARY_DIR}/block.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/device.h
    ${CMAKE_CURRENT_BINARY_DIR}/device.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/deviceinterface.h
    ${CMAKE_CURRENT_BINARY_DIR}/deviceinterface.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/devicenotifier.h
    ${CMAKE_CURRENT_BINARY_DIR}/devicenotifier.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/genericinterface.h
    ${CMAKE_CURRENT_BINARY_DIR}/genericinterface.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/opticaldisc.h
    ${CMAKE_CURRENT_BINARY_DIR}/opticaldisc.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/opticaldrive.h
    ${CMAKE_CURRENT_BINARY_DIR}/opticaldrive.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/predicate.h
    ${CMAKE_CURRENT_BINARY_DIR}/predicate.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/solidnamespace.h
    ${CMAKE_CURRENT_BINARY_DIR}/solidnamespace.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/storagedrive.h
    ${CMAKE_CURRENT_BINARY_DIR}/storagedrive.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/storagevolume.h
    ${CMAKE_CURRENT_BINARY_DIR}/storagevolume.h COPYONLY)

########### static lib ###############

add_library(Solid_static STATIC ${solid_LIB_SRCS})
set_target_properties(Solid_static PROPERTIES COMPILE_FLAGS -DSOLID_STATIC_DEFINE=1)

target_link_libraries(Solid_static
  PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Gui
    ${solid_OPTIONAL_LIBS}
 )
if (TARGET Qt${QT_VERSION_MAJOR}::DBus)
  target_link_libraries(Solid_static PRIVATE Qt${QT_VERSION_MAJOR}::DBus)
endif()

target_include_directories(Solid_static PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR};${CMAKE_CURRENT_SOURCE_DIR}}/..;${CMAKE_CURRENT_BINARY_DIR}/..>")
