set(PROJECT_NAME pyrtaudio)

set(PYRTAUDIO_LIB ${CMAKE_SOURCE_DIR}/lib)
set(PYRTAUDIO_INCLUDE ${CMAKE_SOURCE_DIR}/include)

# Detect the installed pybind11 package and import it into CMake
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE PB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${PB_DIR}")
find_package(pybind11 CONFIG REQUIRED)

# find_package(pybind11 REQUIRED)


pybind11_add_module(
    ${PROJECT_NAME}
    ${RTAUDIO_SRC}
    pyrtaudio.cpp
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    __MACOSX_CORE__
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PYRTAUDIO_INCLUDE}/rtaudio
)

target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PYRTAUDIO_LIB}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    pthread
    "-framework CoreAudio"
    # ${PYRTAUDIO_LIB}/librtaudio.a
)