
function(add_rtmidi_test)
    set(name ${ARGV0})
    message("name: ${name}")
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} rtmidi)
    target_include_directories(${name} PUBLIC ${PROJECT_INCLUDES})
    target_link_directories(${name} PUBLIC ${PROJECT_LIB_DIRS})
    target_link_libraries(${name} PUBLIC rtmidi ${RTMIDI_LINK_LIBS})
endfunction()


add_rtmidi_test(apinames)
add_rtmidi_test(cmidiin)
add_rtmidi_test(midiclock)
add_rtmidi_test(midiout)
add_rtmidi_test(midiprobe)
add_rtmidi_test(qmidiin)
add_rtmidi_test(sysextest)
add_rtmidi_test(testcapi)

# add_test(NAME apinames COMMAND apinames)
