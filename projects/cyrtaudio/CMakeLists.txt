set(PROJECT_NAME cyrtaudio)

set(PROJECT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/cyrtaudio.cpp)
set(PROJECT_PYX ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_PYX})

add_library( 
    ${PROJECT_NAME} 
    MODULE
    $<IF:$<BOOL:${USE_STATICLIB}>,,${RTAUDIO_SRC}>
    ${PROJECT_SRC}
)

add_dependencies(
    ${PROJECT_NAME}
    rtaudio
)

add_custom_command(
    OUTPUT ${PROJECT_SRC}
    COMMAND cythonize -3 ${PROJECT_PYX}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${PROJECT_PYX}
    COMMENT "Generating ${PROJECT_SRC}"
)

add_custom_target(
    cyrtaudio_cpp
    DEPENDS ${PROJECT_PYX}
)

add_dependencies(
    ${PROJECT_NAME}
    cyrtaudio_cpp
)

string(TOLOWER ${CMAKE_SYSTEM_NAME} platform)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_NAME "${PROJECT_NAME}.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-${platform}"
)
    
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${Python_INCLUDE_DIRS}
    ${PROJECT_INCLUDES}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    ${RTAUDIO_COMPILE_DEFS}
    -DNDEBUG
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

target_link_options(
    ${PROJECT_NAME}
    PUBLIC
    $<$<PLATFORM_ID:Darwin>:-dynamic>
)

target_link_directories(
    ${PROJECT_NAME} 
    PUBLIC
    ${Python_LIBRARY_DIRS}
    ${PROJECT_LIB_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC
    ${Python_LIBRARIES}
    "$<$<PLATFORM_ID:Darwin>:-ldl>"
    ${RTAUDIO_LINK_LIBS}
    $<$<BOOL:${USE_STATICLIB}>:rtaudio>
)



