
function(add_rtaudio_test)
    set(name ${ARGV0})
    message("name: ${name}")
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} rtaudio)
    target_include_directories(${name} PUBLIC ${PROJECT_INCLUDES})
    target_link_directories(${name} PUBLIC ${PROJECT_LIB_DIRS})
    target_link_libraries(${name} PUBLIC rtaudio ${RTAUDIO_LINK_LIBS})
endfunction()


add_rtaudio_test(audioprobe)
add_rtaudio_test(playsaw)
add_rtaudio_test(playraw)
add_rtaudio_test(record)
add_rtaudio_test(duplex)
add_rtaudio_test(apinames)
add_rtaudio_test(testall)
add_rtaudio_test(teststops)

# add_test(NAME apinames COMMAND apinames)
