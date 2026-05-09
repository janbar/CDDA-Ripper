cmake_minimum_required(VERSION 3.20)
project(cddb VERSION "5.1.0")

include(FeatureSummary)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

option(QT_VERSION_PREFERRED "preferred Qt version")
include(qtchooser)
if(NOT QT_FOUND)
    message(FATAL_ERROR "Qt${QT_VERSION_PREFERRED} NOT found")
endif()

find_package(Qt${QT_VERSION_MAJOR} ${QT_VERSION} COMPONENTS Core Widgets REQUIRED)

# Automatically create moc files
set(CMAKE_AUTOMOC ON)

find_package(MusicBrainz5)
set_package_properties(MusicBrainz5 PROPERTIES
   DESCRIPTION "Music metadata lookup through libkcddb. Version 5.x of libmusicbrainz"
   TYPE OPTIONAL)

if(MUSICBRAINZ5_FOUND)
    set(HAVE_MUSICBRAINZ5 1)
else()
    set(HAVE_MUSICBRAINZ5 0)
endif()

configure_file(config-musicbrainz.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(kcmcddb STATIC)

target_sources(kcmcddb
    PRIVATE
        kcmcddb.cpp kcmcddb.h
)

target_link_libraries(kcmcddb
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        KCddb
)

target_include_directories(kcmcddb
    PRIVATE
        ${CMAKE_SOURCE_DIR}
)

configure_file(kcmcddb.h ${PROJECT_BINARY_DIR}/ COPYONLY)

add_subdirectory(libkcddb)

if(BUILD_TESTING)
    find_package(Qt${QT_VERSION_MAJOR}Test REQUIRED)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(tests)
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
