cmake_minimum_required(VERSION 3.20.0)

project(CDDARipper VERSION "1.0")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
configure_file(config.h.cmake ${CMAKE_BINARY_DIR}/config.h)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

include(FeatureSummary)

option(QT_VERSION_PREFERRED "preferred Qt version")
include(qtchooser)
if(NOT QT_FOUND)
    message(FATAL_ERROR "Qt NOT found")
endif()

find_package(Qt${QT_VERSION_MAJOR} ${QT_VERSION} COMPONENTS
    Core Gui Widgets DBus Xml Network REQUIRED)

#if (QT_VERSION_MAJOR STREQUAL 6)
#    find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
#endif()

# Automatically create moc files
set(CMAKE_AUTOMOC ON)

find_package(Cdparanoia REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CDPARANOIA_INCLUDE_DIR}
)

remove_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_FOREACH
)

set(cddaripper_SRCS
    main.cpp
    mainwindow.cpp
    goodstyle.cpp
    preferences.cpp
    models/cddamodel.cpp
    models/profilemodel.cpp
    utils/pid.cpp
    utils/coverfetcher.cpp
    utils/schemeparser.cpp
    utils/playlist.cpp
    utils/wavefilewriter.cpp
    utils/cddaparanoia.cpp
    utils/cddadevices.cpp
    utils/encoderwrapper.cpp
    utils/cddaextractthread.cpp
    utils/encoderassistant.cpp
    utils/hashlist.cpp
    utils/3rdparty/crc32.cpp
    utils/parameters.cpp
    utils/cachedimage.cpp
    utils/cuesheetwriter.cpp
    utils/tmpdir.cpp
    utils/discidcalculator.cpp
    widgets/waitingspinnerwidget.cpp
    widgets/cddaheaderwidget.cpp
    widgets/generalsettingswidget.cpp
    widgets/devicewidget.cpp
    widgets/profilewidget.cpp
    widgets/lamewidget.cpp
    widgets/oggencwidget.cpp
    widgets/flacwidget.cpp
    widgets/faacwidget.cpp
    widgets/wavewidget.cpp
    widgets/customwidget.cpp
    widgets/opusencwidget.cpp
    dialogs/aboutwidget.cpp
    dialogs/coverbrowserdialog.cpp
    dialogs/cddaheaderdatadialog.cpp
    dialogs/profiledatadialog.cpp
    dialogs/profiledatacoverdialog.cpp
    dialogs/profiledataplaylistdialog.cpp
    dialogs/profiledatainfodialog.cpp
    dialogs/profiledatahashlistdialog.cpp
    dialogs/profiledatacuesheetdialog.cpp
    dialogs/profiledatasinglefiledialog.cpp
    dialogs/schemewizarddialog.cpp
    dialogs/commandwizarddialog.cpp
    dialogs/simpleschemewizarddialog.cpp
    dialogs/extractingprogressdialog.cpp
    dialogs/protocoldialog.cpp
    dialogs/errordialog.cpp
    core/audex.cpp
)

set(cddaripper_HDRS
    mainwindow.h
    goodstyle.h
    preferences.h
    models/cddamodel.h
    models/profilemodel.h
    utils/pid.h
    utils/coverfetcher.h
    utils/schemeparser.h
    utils/playlist.h
    utils/wavefilewriter.h
    utils/cddaparanoia.h
    utils/cddadevices.h
    utils/encoderwrapper.h
    utils/cddaextractthread.h
    utils/encoderassistant.h
    utils/hashlist.h
    utils/3rdparty/crc32.h
    utils/parameters.h
    utils/cachedimage.h
    utils/cuesheetwriter.h
    utils/tmpdir.h
    utils/discidcalculator.h
    widgets/waitingspinnerwidget.h
    widgets/cddaheaderwidget.h
    widgets/generalsettingswidget.h
    widgets/devicewidget.h
    widgets/profilewidget.h
    widgets/lamewidget.h
    widgets/oggencwidget.h
    widgets/flacwidget.h
    widgets/faacwidget.h
    widgets/wavewidget.h
    widgets/customwidget.h
    widgets/opusencwidget.h
    dialogs/aboutwidget.h
    dialogs/coverbrowserdialog.h
    dialogs/cddaheaderdatadialog.h
    dialogs/profiledatadialog.h
    dialogs/profiledatacoverdialog.h
    dialogs/profiledataplaylistdialog.h
    dialogs/profiledatainfodialog.h
    dialogs/profiledatahashlistdialog.h
    dialogs/profiledatacuesheetdialog.h
    dialogs/profiledatasinglefiledialog.h
    dialogs/schemewizarddialog.h
    dialogs/commandwizarddialog.h
    dialogs/simpleschemewizarddialog.h
    dialogs/extractingprogressdialog.h
    dialogs/protocoldialog.h
    dialogs/errordialog.h
    core/audex.h
)

qt_wrap_ui(cddaripper_SRCS
    widgets/generalsettingswidgetUI.ui
    widgets/devicewidgetUI.ui
    widgets/profilewidgetUI.ui
    widgets/lamewidgetUI.ui
    widgets/oggencwidgetUI.ui
    widgets/flacwidgetUI.ui
    widgets/faacwidgetUI.ui
    widgets/wavewidgetUI.ui
    widgets/customwidgetUI.ui
    widgets/opusencwidgetUI.ui
    dialogs/aboutwidgetUI.ui
    dialogs/coverbrowserwidgetUI.ui
    dialogs/cddaheaderdatawidgetUI.ui
    dialogs/profiledatawidgetUI.ui
    dialogs/profiledatacoverwidgetUI.ui
    dialogs/profiledataplaylistwidgetUI.ui
    dialogs/profiledatainfowidgetUI.ui
    dialogs/profiledatahashlistwidgetUI.ui
    dialogs/profiledatacuesheetwidgetUI.ui
    dialogs/profiledatasinglefilewidgetUI.ui
    dialogs/schemewizardwidgetUI.ui
    dialogs/commandwizardwidgetUI.ui
    dialogs/simpleschemewizardwidgetUI.ui
    dialogs/extractingprogresswidgetUI.ui
    dialogs/protocolwidgetUI.ui
)

qt_add_resources(cddaripper_SRCS cddaripper.qrc goodstyle.qrc)

add_subdirectory(cddb)
add_subdirectory(thumbnailer)
add_subdirectory(solid)

add_executable(cddaripper ${cddaripper_SRCS} ${cddaripper_HDRS})
target_link_libraries(cddaripper
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Xml
    ${CDPARANOIA_LIBRARIES}
    kcmcddb
    Thumbnailer
    Solid_static
)

set(APP_ID            "io.github.janbar.cddaripper")
set(APP_NAME          "CDDA-Ripper")

set(EXEC              "cddaripper")
set(ICON_NAME         "${EXEC}")
set(APP_DESKTOP_ID    "${EXEC}")
set(DESKTOP_FILE_NAME "cddaripper.desktop")
set(APPDATA_FILE_NAME "cddaripper.appdata.xml")

include(GNUInstallDirs)
set(APPDATA_DIR ${CMAKE_INSTALL_DATADIR}/metainfo)
set(DESKTOP_DIR ${CMAKE_INSTALL_DATADIR}/applications)
set(PLUGINS_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/${EXEC})

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${APP_DESKTOP_ID}.desktop "[Desktop Entry]
Name=${APP_NAME}
Exec=${EXEC}
Icon=${ICON_NAME}
Terminal=false
Type=Application
Categories=Audio;")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake IMMEDIATE @ONLY
)
add_custom_target(
    "uninstall"
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${APP_DESKTOP_ID}.desktop
    DESTINATION ${DESKTOP_DIR}
)

install(FILES "icons/${ICON_NAME}-16x16.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/16x16/apps RENAME "${ICON_NAME}.png")
install(FILES "icons/${ICON_NAME}-32x32.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/32x32/apps RENAME "${ICON_NAME}.png")
install(FILES "icons/${ICON_NAME}-48x48.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/48x48/apps RENAME "${ICON_NAME}.png")
install(FILES "icons/${ICON_NAME}-64x64.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/64x64/apps RENAME "${ICON_NAME}.png")
install(FILES "icons/${ICON_NAME}-96x96.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/96x96/apps RENAME "${ICON_NAME}.png")
install(FILES "icons/${ICON_NAME}-128x128.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/128x128/apps RENAME "${ICON_NAME}.png")
install(FILES "icons/${ICON_NAME}-192x192.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/192x192/apps RENAME "${ICON_NAME}.png")
install(FILES "icons/${ICON_NAME}-256x256.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps RENAME "${ICON_NAME}.png")
install(FILES "icons/${ICON_NAME}-512x512.png"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/512x512/apps RENAME "${ICON_NAME}.png")

configure_file(
    ${APPDATA_FILE_NAME}.in
    ${CMAKE_CURRENT_BINARY_DIR}/${APPDATA_FILE_NAME}
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${APPDATA_FILE_NAME}
  DESTINATION ${APPDATA_DIR} RENAME ${APPDATA_FILE_NAME}
)

install(TARGETS cddaripper RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES)
