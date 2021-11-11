cmake_minimum_required(VERSION 3.14)

project( SerialPort )

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(SerialPort STATIC
"src/serial_port.cc"
"src/interface.cc" "src/interface.h"
"src/serial_port_windows.cc" "src/serial_port_windows.h" 
"src/serial_port_linux.cc" "src/serial_port_linux.h"
"include/serial_port/types.h" "src/enumeration.h" "src/enumeration.cpp")

target_include_directories (SerialPort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

add_executable(
  "serial_port_tests"
  "test/tests.cc"
 "src/enumeration.h" "src/enumeration.cpp")

target_link_libraries(
  serial_port_tests
  gtest_main
  SerialPort
)

include(GoogleTest)
gtest_discover_tests(serial_port_tests)
