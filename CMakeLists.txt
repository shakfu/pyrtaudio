cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(python_rtmusic_project LANGUAGES CXX C)

# standards version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")
endif()

# ----------------------------------------------------------------------------
# global variables

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ----------------------------------------------------------------------------
# global options

option(BUILD_SHARED_LIBS "Build as shared library" OFF)
option(USE_STATICLIB "build using rtaudio / rtmidi staticlib" OFF)
option(BUILD_TESTS "build rtaudio / rtmidi tests")

# ----------------------------------------------------------------------------
# build optimization

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# use ccache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Found ccache in ${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

if (ENABLE_LTO_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_lto_supported OUTPUT check_lto_ipo_error)
    if(ipo_lto_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
        message(STATUS "IPO / LTO enabled")
    else()
        message(STATUS "IPO / LTO not supported: <${check_lto_ipo_error}>")
    endif()
endif()

message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

# ----------------------------------------------------------------------------
# common variables

set(PROJECT_LIB_DIRS ${CMAKE_SOURCE_DIR}/lib)
set(PROJECT_INCLUDES ${CMAKE_SOURCE_DIR}/include)

# ----------------------------------------------------------------------------
# rtaudio common variables

set(RTAUDIO_DIR
    ${CMAKE_SOURCE_DIR}/include/rtaudio
)

set(RTAUDIO_SRC
    ${RTAUDIO_DIR}/RtAudio.cpp
    ${RTAUDIO_DIR}/RtAudio.h
    ${RTAUDIO_DIR}/rtaudio_c.cpp
    ${RTAUDIO_DIR}/RtAudio_c.h
)

set(RTAUDIO_LINK_LIBS
    "$<$<PLATFORM_ID:Darwin>:-lpthread>"
    "$<$<PLATFORM_ID:Darwin>:-framework CoreFoundation>"
    "$<$<PLATFORM_ID:Darwin>:-framework CoreAudio>"
)
set(RTAUDIO_COMPILE_DEFS
    $<$<PLATFORM_ID:Darwin>:__MACOSX_CORE__>
)

# ----------------------------------------------------------------------------
# rtmidi common variables

set(RTMIDI_DIR
    ${CMAKE_SOURCE_DIR}/include/rtmidi
)

set(RTMIDI_SRC
    ${RTMIDI_DIR}/RtMidi.cpp
    ${RTMIDI_DIR}/RtMidi.h
    ${RTMIDI_DIR}/rtmidi_c.cpp
    ${RTMIDI_DIR}/rtmidi_c.h
)

set(RTMIDI_LINK_LIBS
    "$<$<PLATFORM_ID:Darwin>:-lpthread>"
    "$<$<PLATFORM_ID:Darwin>:-framework CoreFoundation>"
    "$<$<PLATFORM_ID:Darwin>:-framework CoreServices>"
    "$<$<PLATFORM_ID:Darwin>:-framework CoreAudio>"
    "$<$<PLATFORM_ID:Darwin>:-framework CoreMIDI>"

)
set(RTMIDI_COMPILE_DEFS
    $<$<PLATFORM_ID:Darwin>:__MACOSX_CORE__>
)

# ----------------------------------------------------------------------------
# main subprojects

add_subdirectory(projects)

# ----------------------------------------------------------------------------
# optional tests


if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

