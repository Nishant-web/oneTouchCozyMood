cmake_minimum_required(VERSION 3.15.4)
project(DELETE_ALL_USER)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_compile_options(-Wall)
add_executable(DELETE_ALL ${SOURCES})

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(DELETE_ALL ${WIRINGPI_LIBRARIES})

