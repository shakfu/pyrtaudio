set(PROJECT_NAME npyrtaudio)

# Detect the installed nanobind package and import it into CMake
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)


set(NPYRTAUDIO_LIB ${CMAKE_SOURCE_DIR}/lib)
set(NPYRTAUDIO_INCLUDE ${CMAKE_SOURCE_DIR}/include)


find_package(nanobind REQUIRED)


nanobind_add_module(
    ${PROJECT_NAME}
    $<IF:$<BOOL:${USE_STATICLIB}>,,${RTAUDIO_SRC}>
    npyrtaudio.cpp
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    ${RTAUDIO_COMPILE_DEFS}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${NPYRTAUDIO_INCLUDE}/rtaudio
)

target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${NPYRTAUDIO_LIB}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    ${RTAUDIO_LINK_LIBS}
    $<$<BOOL:${USE_STATICLIB}>:rtaudio>
)
