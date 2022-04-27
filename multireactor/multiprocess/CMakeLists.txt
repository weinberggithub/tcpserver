cmake_minimum_required(VERSION 2.8)

project(multipreactor C CXX)

aux_source_directory(. SRC_DIR)
set(CMAKE_CXX_FLAGS "-g -std=c++11 -lpthread")


add_executable(${CMAKE_PROJECT_NAME} ${SRC_DIR})