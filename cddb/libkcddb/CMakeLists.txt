add_definitions(-DTRANSLATION_DOMAIN="libkcddb")

add_library(KCddb STATIC)

if(MUSICBRAINZ5_FOUND)
    set(musicbrainz_sources
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp musicbrainz/asyncmusicbrainzlookup.h)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" OR (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"))
        set(enable_exceptions -EHsc)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(enable_exceptions "-fexceptions -UQT_NO_EXCEPTIONS")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        set(enable_exceptions -fexceptions)
    endif()

    set_source_files_properties(${musicbrainz_sources} PROPERTIES COMPILE_FLAGS ${enable_exceptions})
endif()

target_sources(KCddb PRIVATE
    cache.cpp cache.h
    cdinfo.cpp cdinfo.h
    config.cpp config.h
    configbase.cpp configbase.h
    logging.cpp logging.h
    client.cpp client.h
    kcddb.cpp kcddb.h
    cddb.cpp cddb.h
    lookup.cpp lookup.h
    categories.cpp categories.h
    genres.cpp genres.h
    ${musicbrainz_sources}
)

target_link_libraries(KCddb PRIVATE Qt${QT_VERSION_MAJOR}::Core)

if (TARGET Qt6::Core5Compat)
    target_link_libraries(KCddb PRIVATE Qt6::Core5Compat)
endif()

if(MUSICBRAINZ5_FOUND)
    target_link_libraries(KCddb PRIVATE musicbrainz)
endif()

configure_file(categories.h ${PROJECT_BINARY_DIR}/ COPYONLY)
configure_file(cdinfo.h ${PROJECT_BINARY_DIR}/ COPYONLY)
configure_file(client.h ${PROJECT_BINARY_DIR}/ COPYONLY)
configure_file(genres.h ${PROJECT_BINARY_DIR}/ COPYONLY)
configure_file(configbase.h ${PROJECT_BINARY_DIR}/ COPYONLY)
configure_file(config.h ${PROJECT_BINARY_DIR}/ COPYONLY)
configure_file(kcddb.h ${PROJECT_BINARY_DIR}/ COPYONLY)

