cmake_minimum_required(VERSION 3.2.1)
project(bench_util)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/bench_util.h)
set(CMAKE_CXX_STANDARD 14)
install(FILES ${SRC} DESTINATION /usr/local/include/bench_util/)

# Examples
file(GLOB EX_ALL RELATIVE ${PROJECT_SOURCE_DIR}/examples/ ${PROJECT_SOURCE_DIR}/examples/*)
set(EX_DIRS "")

file(GLOB EX_SRC ${PROJECT_SOURCE_DIR}/examples/*.cpp)

foreach(d ${EX_ALL})
	if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/examples/${d})
		message(STATUS "Found example : " ${d})

		file(GLOB D_SRC RELATIVE ${PROJECT_SOURCE_DIR}/examples/${d}/ ${PROJECT_SOURCE_DIR}/examples/${d}/*.cpp)
		foreach(f ${D_SRC})
			string(REPLACE "main" "" n ${f})
			string(REPLACE ".cpp" "" n ${n})

			add_executable(${d}${n} ${PROJECT_SOURCE_DIR}/examples/${d}/${f})
		endforeach()
	endif()
endforeach()