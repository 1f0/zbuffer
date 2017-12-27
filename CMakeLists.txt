cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

find_package(Eigen3 REQUIRED)
if(EIGEN3_FOUND)
    message("-- Eigen @ ${EIGEN3_INCLUDE_DIR}")
    message("-- Eigen version: ${EIGEN3_VERSION}")
    include_directories(${EIGEN3_INCLUDE_DIR})
endif(EIGEN3_FOUND)

find_package(glfw3 3.2 REQUIRED)

add_subdirectory(src)
 