set(PROJECT_NAME pbrtaudio)

set(PBRTAUDIO_LIB ${CMAKE_SOURCE_DIR}/lib)
set(PBRTAUDIO_INCLUDE ${CMAKE_SOURCE_DIR}/include)

# Detect the installed pybind11 package and import it into CMake
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE PB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${PB_DIR}")
find_package(pybind11 CONFIG REQUIRED)

# find_package(pybind11 REQUIRED)


pybind11_add_module(
    ${PROJECT_NAME}
    $<IF:$<BOOL:${USE_STATICLIB}>,,${RTAUDIO_SRC}>
    pbrtaudio.cpp
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
    ${PBRTAUDIO_INCLUDE}/rtaudio
)

target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PBRTAUDIO_LIB}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    ${RTAUDIO_LINK_LIBS}
    $<$<BOOL:${USE_STATICLIB}>:rtaudio>
)