CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

PROJECT(NumericSim)

#SETS
SET(VENDOR "NumericSim project 2016")
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_HEX 0x0001)
SET(NS_Version ${VERSION_MAJOR}.${VERSION_MINOR})

SET(NS_Root ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

if(BUILD_SHARED_LIBS AND WIN32 AND NOT CYGWIN)
   SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
endif(BUILD_SHARED_LIBS AND WIN32 AND NOT CYGWIN)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
SET(CMAKE_DEBUG_POSTFIX  "_d")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

option(VERBOSE "Show debug information." OFF)
option(NS_BUILD_TESTS "Build tests." ON)
option(NS_BUILD_EXAMPLES "Build examples." ON)

#CHECKS
if(VERBOSE)
	MESSAGE(STATUS "Building NumericSim ${NS_Version}")
	MESSAGE(STATUS "Root: ${NS_Root}")
endif()

IF(MINGW)
	MESSAGE(WARNING "Please don't use MinGW with NumericSim, use the MSVC version instead")
ENDIF()

IF(MSYS)
	MESSAGE(WARNING "Please don't use MSYS with NumericSim, use the MSVC version instead")
ENDIF()

IF(CYGWIN)
	MESSAGE(WARNING "Please don't use CYGWIN with NumericSim, use the MSVC version instead")
ENDIF()

#PACKAGE
find_package(Doxygen)

#DEFINITIONS AND FLAGS
IF(MSVC)
	SET(DEFS "${DEFS};_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_WARNINGS;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi /fp:fast /EHsc /W3")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /Ot /GL")
	IF(CMAKE_CL_64)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	ENDIF()
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	IF (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		if(VERBOSE)
			message(STATUS "C++11 activated.")
		endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    ELSEIF (GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
        message(WARNING "C++0x activated. If you get any errors update to a compiler which fully supports C++11")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
    ELSE ()
        message(WARNING "C++11 needed. Build can fail.")   
    ENDIF ()
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# We do not check version any further.
	if(VERBOSE)
		message(STATUS "C++11 activated.")
	endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSE()
    message(WARNING "Unknown Compiler. C++11 needed. Build can fail.")   
ENDIF()

#CONFIGURE
add_subdirectory(src)

if(NS_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()

if(NS_BUILD_TESTS)
enable_testing()
add_subdirectory(tests)
endif()

if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)