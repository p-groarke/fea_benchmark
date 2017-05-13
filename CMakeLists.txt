cmake_minimum_required(VERSION 3.5.0)
set(PROJECT_NAME bench_util)
project(${PROJECT_NAME} CXX)
set(BENCH_UTIL_VERSION 2.0.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/include/bench_util.h)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpedantic
		-Werror)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpedantic
		-Werror)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(${PROJECT_NAME} INTERFACE /W4 /WX)
endif()


# Install Package Configuration

set(BENCH_UTIL_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/bench_util")
install(FILES ${HEADER_FILES} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bench_util")

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/bench_utilConfigVersion.cmake"
	VERSION ${BENCH_UTIL_VERSION}
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
			target_include_directories(${d}${n} PRIVATE
					${CMAKE_CURRENT_SOURCE_DIR}/include)
		endforeach()
	endif()
endforeach()
