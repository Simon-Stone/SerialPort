cmake_minimum_required( VERSION 3.0 )

project( SerialPort )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(SerialPort SHARED 
"src/serial_port.cc"
"src/interface.cc" "src/interface.h"
"src/serial_port_windows.cc" "src/serial_port_windows.h")

target_include_directories (SerialPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
