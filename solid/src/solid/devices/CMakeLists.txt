include (CheckCXXSourceCompiles)

if(WIN32)
    add_definitions(-DYY_NO_UNISTD_H)
endif()

if(MSVC OR (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Intel"))
    check_cxx_source_compiles("int main() { __asm { pxor mm0, mm0 }; }" HAVE_X86_MMX)
    check_cxx_source_compiles("int main() { __asm { xorps xmm0, xmm0 }; }" HAVE_X86_SSE)
    check_cxx_source_compiles("int main() { __asm { xorpd xmm0, xmm0 }; }" HAVE_X86_SSE2)
    check_cxx_source_compiles("int main() { __asm { femms }; }" HAVE_X86_3DNOW)
else()
    check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                            #define __asm__ asm
                                    #endif
                    int main() { __asm__(\"pxor %mm0, %mm0\") ; }" HAVE_X86_MMX)
    check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                            #define __asm__ asm
                                    #endif
                    int main() { __asm__(\"xorps %xmm0, %xmm0\"); }" HAVE_X86_SSE)
    check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                            #define __asm__ asm
                                    #endif
                    int main() { __asm__(\"xorpd %xmm0, %xmm0\"); }" HAVE_X86_SSE2)
    check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                            #define __asm__ asm
                                    #endif
                    int main() { __asm__(\"femms\"); }" HAVE_X86_3DNOW)
endif()
check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                        #define __asm__ asm
                                #endif
                int main() { __asm__(\"mtspr 256, %0; vand %%v0, %%v0, %%v0\" : : \"r\"(-1) ); }" HAVE_PPC_ALTIVEC)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/devices/
    ${CMAKE_CURRENT_SOURCE_DIR}/devices/frontend/
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(solid_LIB_SRCS
    ${solid_LIB_SRCS}
    devices/managerbase.cpp
    devices/managerbase_p.h
    devices/predicateparse.cpp
    devices/predicateparse.h
    devices/soliddefs_p.h
    devices/solidnamespace.cpp

    devices/frontend/block.cpp
    devices/frontend/block.h
    devices/frontend/block_p.h
    devices/frontend/device.cpp
    devices/frontend/device.h
    devices/frontend/device_p.h
    devices/frontend/deviceinterface.cpp
    devices/frontend/deviceinterface.h
    devices/frontend/deviceinterface_p.h
    devices/frontend/devicemanager.cpp
    devices/frontend/devicemanager_p.h
    devices/frontend/devicenotifier.h
    devices/frontend/genericinterface.cpp
    devices/frontend/genericinterface.h
    devices/frontend/genericinterface_p.h
    devices/frontend/opticaldisc.cpp
    devices/frontend/opticaldisc.h
    devices/frontend/opticaldisc_p.h
    devices/frontend/opticaldrive.cpp
    devices/frontend/opticaldrive.h
    devices/frontend/opticaldrive_p.h
    devices/frontend/predicate.cpp
    devices/frontend/predicate.h
    devices/frontend/solidnamespace.h
    devices/frontend/storagedrive.cpp
    devices/frontend/storagedrive.h
    devices/frontend/storagedrive_p.h
    devices/frontend/storagevolume.cpp
    devices/frontend/storagevolume.h
    devices/frontend/storagevolume_p.h

    devices/ifaces/block.cpp
    devices/ifaces/block.h
    devices/ifaces/device.cpp
    devices/ifaces/device.h
    devices/ifaces/deviceinterface.cpp
    devices/ifaces/deviceinterface.h
    devices/ifaces/devicemanager.cpp
    devices/ifaces/devicemanager.h
    devices/ifaces/genericinterface.cpp
    devices/ifaces/genericinterface.h
    devices/ifaces/opticaldisc.cpp
    devices/ifaces/opticaldisc.h
    devices/ifaces/opticaldrive.cpp
    devices/ifaces/opticaldrive.h
    devices/ifaces/storagedrive.cpp
    devices/ifaces/storagedrive.h
    devices/ifaces/storagevolume.cpp
    devices/ifaces/storagevolume.h

    devices/backends/shared/rootdevice.cpp
    devices/backends/shared/rootdevice.h
)

bison_target(SolidParser
             devices/predicate_parser.y
             ${CMAKE_CURRENT_BINARY_DIR}/predicate_parser.c
             COMPILE_FLAGS "-p Solid -d -b predicate_parser"
)
set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/predicate_parser.h PROPERTY SKIP_AUTOMOC TRUE) # don't run automoc on this file

flex_target(SolidLexer
            devices/predicate_lexer.l
            ${CMAKE_CURRENT_BINARY_DIR}/predicate_lexer.c
            COMPILE_FLAGS "-P Solid"
)
add_flex_bison_dependency(SolidLexer SolidParser)
list(APPEND solid_LIB_SRCS ${BISON_SolidParser_OUTPUTS} ${FLEX_SolidLexer_OUTPUTS})

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckCXXSourceCompiles)

configure_file(devices/config-solid.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-solid.h)

set(solid_OPTIONAL_LIBS)

add_device_backends_build("${CMAKE_CURRENT_SOURCE_DIR}/devices/backends" solid_LIB_SRCS solid_OPTIONAL_LIBS)
