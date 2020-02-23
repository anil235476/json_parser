cmake_minimum_required(VERSION 3.7)

add_definitions(/ZI)

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	
endif()

add_library(json_parser
	json_parser.h
	json_parser.cpp
	)
	
if(ENABLE_UNIT_TEST)
add_subdirectory(unit_test)
endif(ENABLE_UNIT_TEST)

target_include_directories(json_parser PUBLIC 
	$<BUILD_INTERFACE:${ABSL_PATH}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../utility>
	
	
	)

target_sources(json_parser PRIVATE
${ABSL_PATH}/absl/types/bad_any_cast.cc
)

target_link_libraries(json_parser PRIVATE utility)

