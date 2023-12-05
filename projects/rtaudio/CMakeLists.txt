set(PROJECT_NAME rtaudio)


add_library( 
    ${PROJECT_NAME} 
    STATIC
    ${RTAUDIO_SRC}
)

    
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_INCLUDES}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    ${RTAUDIO_COMPILE_DEFS}
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    -std=c++11
    $<$<PLATFORM_ID:Darwin>:-Wno-unused-result>
    $<$<PLATFORM_ID:Darwin>:-Wsign-compare>
    $<$<PLATFORM_ID:Darwin>:-Wunreachable-code>
    $<$<PLATFORM_ID:Darwin>:-fno-common>
    $<$<PLATFORM_ID:Darwin>:-Wall>  
    $<$<PLATFORM_ID:Darwin>:-g>
    $<$<PLATFORM_ID:Darwin>:-fwrapv>
    $<$<PLATFORM_ID:Darwin>:-O3>
    $<$<PLATFORM_ID:Windows>:/O2>
    $<$<PLATFORM_ID:Windows>:/MD> # api module works with this
)

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC
    ${RTAUDIO_LINK_LIBS}
)
