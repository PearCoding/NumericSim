include_directories(AFTER ../src/lib)

function(NS_ADD_EXAMPLE name src)
add_executable(example_${name} ${src})
#target_link_libraries(example_${name} ns_lib)
set_target_properties(example_${name} PROPERTIES VERSION ${NS_Version})
endfunction()

NS_ADD_EXAMPLE(heat heat/main.cpp)
NS_ADD_EXAMPLE(poisson poisson/main.cpp)