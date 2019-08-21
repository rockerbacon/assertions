cmake_minimum_required(VERSION 3.10)

project("Your project name here" CXX)

set(libs_dir "src/libs")
set(programs_src_dir "src/main")
set(tests_src_dir "tests")

set(programs_build_dir "release")
set(tests_build_dir "tests_build")
set(libs_build_dir "libs_build")

# set(warnings "-Wall -Wextra -Werror")
set(cpp_version "c++14")

set(CMAKE_CXX_FLAGS "${warnings} --std=${cpp_version}")

include_directories(${libs_dir})

file(GLOB_RECURSE libs_src_files "${libs_dir}/*.cpp")
add_library(all_libs ${libs_src_files})
# add_executable(libs ${libs_src_files})

add_subdirectory(${programs_src_dir} ${programs_build_dir})
add_subdirectory(${tests_src_dir} ${tests_build_dir})
