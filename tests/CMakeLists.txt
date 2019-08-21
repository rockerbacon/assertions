cmake_minimum_required(VERSION 3.10)

file(GLOB tests_src_files "*.cpp")

foreach(test_src_file ${tests_src_files})
	string(REPLACE ".cpp" "" test_src_without_suffix ${test_src_file})
	string(REGEX REPLACE "^.*/" "" test_binary ${test_src_without_suffix})
	add_executable(${test_binary} ${test_src_file})
	target_link_libraries(${test_binary} all_libs)
endforeach()

