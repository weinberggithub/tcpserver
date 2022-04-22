cmake_minimum_required(VERSION 2.8)

project(multipreactor C CXX)

#set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
#file(GLOB SOURCE "$CMAKE_SOURCE_DIR}/*.cpp")
#file(GLOB RELY_SOURCE "../../tools/*.cpp")
#set(SOURCE_DIRS ${SOURCE_DIRS} "../../tools" )

add_executable(${CMAKE_PROJECT_NAME} reactor.cpp sem.cpp shmtx.cpp main.cpp)
set(CMAKE_CXX_FLAGS "-g -std=c++11")

target_link_libraries(${CMAKE_PROJECT_NAME}
    /opt/leetcode/lib/libtools.dylib
                    )