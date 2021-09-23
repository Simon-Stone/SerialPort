cmake_minimum_required( VERSION 3.0 )

project( SerialPort )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(SerialPort SHARED src/SerialPort.cpp)

target_include_directories (SerialPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
