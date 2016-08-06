include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib ${CMAKE_CURRENT_SOURCE_DIR})

function(NS_ADD_TEST name src)
add_executable(test_${name} ${src} Test.h)
#target_link_libraries(test_${name} ns_lib)
target_link_libraries(test_${name} ${Boost_LIBRARIES})
#target_link_libraries(test_${name} ${DEAL_II_LIBRARIES})
set_target_properties(test_${name} PROPERTIES VERSION ${NS_Version})
add_test(NAME ${name} COMMAND test_${name})
set_tests_properties(${name} PROPERTIES DEPENDS test_${name})
if(VERBOSE)
	message(STATUS "Test: test_${name}: ${src}")
endif()
endfunction()

# TESTS
NS_ADD_TEST(vector vector.cpp)
NS_ADD_TEST(matrix matrix.cpp)
NS_ADD_TEST(sparsematrix sparsematrix.cpp)