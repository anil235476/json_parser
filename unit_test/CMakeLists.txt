cmake_minimum_required(VERSION 3.7)

add_executable( test_case 
			main.cpp
			)

target_include_directories(test_case PUBLIC 
	#$<BUILD_INTERFACE:${BOOST_LIB}>
	#$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/signaller/>
	#$<BUILD_INTERFACE:${CMAKE_HOME_DIRECTORY}/lib/signaller/>
	#$<BUILD_INTERFACE:${ABSL_PATH}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/catch>
	$<BUILD_INTERFACE:${CMAKE_HOME_DIRECTORY}>
	)
	
target_link_directories( test_case PUBLIC
	$<BUILD_INTERFACE:${BOOST_LIB}/stage/lib>
	#$<BUILD_INTERFACE:${CMAKE_HOME_DIRECTORY}/webrtc_open_source/debug>
)

target_link_libraries( test_case 
json_parser
)