find_package(Qt${QT_VERSION_MAJOR}Qml ${QT_VERSION} CONFIG QUIET)

set_package_properties(Qt${QT_VERSION_MAJOR}Qml
    PROPERTIES  URL "git://gitorious.org/qt/qtdeclarative.git"
    DESCRIPTION "QML support for Solid"
    TYPE OPTIONAL
    PURPOSE "Allows to use Solid in QML code"
)


if(NOT TARGET Qt${QT_VERSION_MAJOR}::Qml)
    message(STATUS "Qt${QT_VERSION_MAJOR}Qml not found, qml imports will not be built.")
    return()
endif()

set(solidextensionplugin_SRCS
    ${solid_common_SRCS}
    solidextensionplugin.cpp
    devices.cpp
    )

add_library(solidextensionplugin SHARED ${solidextensionplugin_SRCS})

target_link_libraries(
    solidextensionplugin
    KF5::Solid
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Qml
)

if(MINGW)
    set_target_properties(solidextensionplugin PROPERTIES PREFIX "")
endif()

install(TARGETS solidextensionplugin DESTINATION ${CMAKE_INSTALL_LIBDIR}/solid)
install(FILES qmldir DESTINATION ${CMAKE_INSTALL_LIBDIR}/solid)

