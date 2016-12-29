include_directories(AFTER ../src/lib)
include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}/../src/objloader)

function(NS_ADD_EXAMPLE name src)
add_executable(example_${name} ${src})
#target_link_libraries(example_${name} ns_lib)
target_link_libraries(example_${name} ns_objloader)
set_target_properties(example_${name} PROPERTIES VERSION ${NS_Version})
endfunction()

NS_ADD_EXAMPLE(heat heat/main.cpp)
NS_ADD_EXAMPLE(poisson_fdm poisson/fdm.cpp)
NS_ADD_EXAMPLE(poisson_fem poisson/fem.cpp poisson/triangles.obj.inl poisson/half_circle.obj.inl)