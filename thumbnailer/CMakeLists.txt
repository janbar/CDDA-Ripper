cmake_minimum_required(VERSION 3.8.2...3.20)
project(Thumbnailer C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

option(QT_VERSION_PREFERRED "preferred Qt version")
include(qtchooser)
if(NOT QT_FOUND)
    message(FATAL_ERROR "Qt${QT_VERSION_PREFERRED} NOT found")
endif()

find_package(Qt${QT_VERSION_MAJOR} ${QT_VERSION} COMPONENTS Core Network Gui REQUIRED)

# Automatically create moc files
set(CMAKE_AUTOMOC ON)

###############################################################################
# configure
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(
    Thumbnailer_SOURCES
    thumbnailer.cpp
    thumbnailerjob.cpp
    ratelimiter.cpp
    netrequest.cpp
    netmanager.cpp
    diskcachemanager.cpp
    artistinfo.cpp
    albuminfo.cpp
    abstractapi.cpp
    tinyxml2.cpp
    xmldict.cpp
    jsonparser.cpp
    lastfm/lastfm.cpp
    lastfm/lfm-artistinfo.cpp
    lastfm/lfm-albuminfo.cpp
    deezer/deezer.cpp
    deezer/deezer-artistinfo.cpp
    deezer/deezer-albuminfo.cpp
)

set(
    Thumbnailer_HEADERS
    thumbnailer.h
    thumbnailerjob.h
    ratelimiter.h
    netrequest.h
    netmanager.h
    diskcachemanager.h
    artistinfo.h
    albuminfo.h
    abstractapi.h
    jsonparser.h
    lastfm/lastfm.h
    deezer/deezer.h
)

add_library(Thumbnailer STATIC ${Thumbnailer_SOURCES} ${Thumbnailer_HEADERS})
target_link_libraries(Thumbnailer
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Gui
)

