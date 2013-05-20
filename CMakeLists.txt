project(pattern_matching)
cmake_minimum_required(VERSION 2.8)

# Copied from relaybus
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
endforeach()

if("${CMAKE_CXX_COMPILER}" MATCHES ".*clang${plus}${plus}")
	set(RELAYBUS_COMPILER_CLANG ON)
	add_definitions(-std=c++11)
elseif("${CMAKE_CXX_COMPILER}" MATCHES ".*icc" OR "${CMAKE_CXX_COMPILER}" MATCHES ".*icpc")
	set(RELAYBUS_COMPILER_INTEL ON)
	add_definitions(-std=c++11)
elseif(CMAKE_COMPILER_IS_GNUCXX)
	set(RELAYBUS_COMPILER_GCC ON)
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.7)
		add_definitions(-std=c++11)
		add_definitions(-Wzero-as-null-pointer-constant)
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.6 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.6)
		add_definitions(-std=c++0x)
	else()
		message(FATAL_ERROR "C++11 features are required. Therefore a gcc compiler >=4.6 is needed.")
	endif()
elseif(MSVC10)
	set(RELAYBUS_COMPILER_MSVC ON)
elseif(MSVC90 OR MSVC80 OR MSVC71 OR MSVC70 OR MSVC60)
	message(FATAL_ERROR "C++11 features are required. <VS2010 doesn't support them")
else()
	message(FATAL_ERROR "C++11 features are required. Please verify if relaybus works with your compiler and send a bug report to the relaybus developers")
endif()

if(RELAYBUS_COMPILER_GCC)
	add_definitions( -Wall -Wextra -Wno-unknown-pragmas -Wwrite-strings -Wenum-compare
					 -Wno-conversion-null -Werror=return-type -pedantic)

	if(RELAYBUS_NO_RTTI)
		# we currently disable this feature, since boost serialize is not usable without rtti in boost 1.52
		# add_definitions(-fno-rtti)
	endif()
elseif(RELAYBUS_COMPILER_CLANG)
	add_definitions( -Wall -Wextra -Wno-unknown-pragmas -Wwrite-strings -Wenum-compare
					-Wno-conversion-null -Werror=return-type -Weverything
					-Wno-c++98-compat -Wno-c++98-compat-pedantic
					-Wno-global-constructors -Wno-exit-time-destructors
					-Wno-documentation
					-Wno-padded
					-Wno-weak-vtables)
elseif(RELAYBUS_COMPILER_INTEL)
	add_definitions( -Wall -Wextra -Wno-unknown-pragmas -Wwrite-strings )
	option(RELAYBUS_INTEL_STATIC_ANALYSIS "sets all required flags for the intel compiler to generate static analysis information" OFF)
	if(RELAYBUS_INTEL_STATIC_ANALYSIS)
		add_definitions( -diag-enable sc3 )
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -diag-enable sc3")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -diag-enable sc3")
		set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -diag-enable sc3")
	endif()
elseif(RELAYBUS_COMPILER_MSVC)
	# enable parallel builds
	add_definitions(/MP)

	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DNOMINMAX) # force windows.h not to define min and max

	# disable some annoying compiler warnings
	add_definitions(/wd4275) # disable warning C4275: non dll-interface x used as base for dll-interface y
	add_definitions(/wd4251) # disable warning C4251: x needs to have dll-interface to be used by clients of y

	# enable additional compiler warnings
	add_definitions(/w14062 /w14263 /w14264 /w14289 /w14623 /w14706)
endif()
# end copied from relaybus

add_executable(${PROJECT_NAME} main.cpp pattern_matching.hpp)

