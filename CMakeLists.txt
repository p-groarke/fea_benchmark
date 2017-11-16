cmake_minimum_required(VERSION 3.5.0)
project(bench_util CXX)
set(${PROJECT_NAME}_VERSION 2.1.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES};Dev")
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()


file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/include/bench_util/bench_util.h)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3")

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${HEADER_FILES}>
)
target_include_directories(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra
		-Werror)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra
		-Werror)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(${PROJECT_NAME} INTERFACE /W4 /WX)
endif()

option(BENCH_SHUTUP "Disable bench_util console output" Off)
if (BENCH_SHUTUP)
	target_compile_definitions(${PROJECT_NAME} INTERFACE BENCH_SHUTUP=1)
endif()


# Install Package Configuration
set(BENCH_UTIL_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/bench_util")
install(FILES ${HEADER_FILES} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bench_util")

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/bench_utilConfigVersion.cmake"
	VERSION ${${PROJECT_NAME}_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	cmake/bench_utilConfig.cmake.in
	${BENCH_UTIL_INSTALL_CONFIGDIR}/bench_utilConfig.cmake
	INSTALL_DESTINATION ${BENCH_UTIL_INSTALL_CONFIGDIR}
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/${BENCH_UTIL_INSTALL_CONFIGDIR}/bench_utilConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/bench_utilConfigVersion.cmake"
	DESTINATION ${BENCH_UTIL_INSTALL_CONFIGDIR}
)


install(TARGETS ${PROJECT_NAME} EXPORT bench_utilTargets)

export(EXPORT bench_utilTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/bench_utilTargets.cmake"
)

install(
	EXPORT bench_utilTargets FILE bench_utilTargets.cmake
	DESTINATION ${BENCH_UTIL_INSTALL_CONFIGDIR}
)

export(PACKAGE ${PROJECT_NAME})


# Examples
option(BUILD_EXAMPLES "Build accompanying examples." On)

if (${BUILD_EXAMPLES})
	file(GLOB EX_ALL RELATIVE ${PROJECT_SOURCE_DIR}/examples/
			${PROJECT_SOURCE_DIR}/examples/*)
	set(EX_DIRS "")
	file(GLOB EX_SRC ${PROJECT_SOURCE_DIR}/examples/*.cpp)
	foreach(d ${EX_ALL})
		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/examples/${d})
			message(STATUS "Found example : " ${d})

			file(GLOB D_SRC RELATIVE ${PROJECT_SOURCE_DIR}/examples/${d}/
					${PROJECT_SOURCE_DIR}/examples/${d}/*.cpp)
			foreach(f ${D_SRC})
				string(REPLACE "main" "" n ${f})
				string(REPLACE ".cpp" "" n ${n})

				add_executable(${d}${n} ${PROJECT_SOURCE_DIR}/examples/${d}/${f})
				target_link_libraries(${d}${n} ${PROJECT_NAME})
			endforeach()
		endif()
	endforeach()
endif()