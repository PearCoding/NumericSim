add_subdirectory(lib)

include_directories(AFTER lib)

function(NS_ADD_TARGET name src)
add_executable(${name} ${src})
#target_link_libraries(${name} ns_lib)
set_target_properties(${name} PROPERTIES VERSION ${NS_Version})
endfunction()

add_subdirectory(poisson)