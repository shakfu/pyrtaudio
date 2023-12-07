set(PROJECT_NAME nbrtaudio)

# Detect the installed nanobind package and import it into CMake
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)


set(NBRTAUDIO_LIB ${CMAKE_SOURCE_DIR}/lib)
set(NBRTAUDIO_INCLUDE ${CMAKE_SOURCE_DIR}/include)


find_package(nanobind REQUIRED)


nanobind_add_module(
    ${PROJECT_NAME}
    $<IF:$<BOOL:${USE_STATICLIB}>,,${RTAUDIO_SRC}>
    nbrtaudio.cpp
)

add_dependencies(
    ${PROJECT_NAME}
    rtaudio
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    ${RTAUDIO_COMPILE_DEFS}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${NBRTAUDIO_INCLUDE}/rtaudio
)

target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${NBRTAUDIO_LIB}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    ${RTAUDIO_LINK_LIBS}
    $<$<BOOL:${USE_STATICLIB}>:rtaudio>
)
