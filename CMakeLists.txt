cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(lbx-fs)

include(utility/utility.cmake)

add_library(${PROJECT_NAME} STATIC "source/fs.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE source)


include(CTest)
enable_testing()

ADD_CMAKE_SUBDIRS_HERE()
