# SerialPort
A light-weight C++ library for cross-platform serial communication on Linux and Windows.

![CMake Linux](https://github.com/nullpunktTUD/SerialPort/actions/workflows/cmake_linux.yml/badge.svg) ![CMake Windows](https://github.com/nullpunktTUD/SerialPort/actions/workflows/cmake_windows.yml/badge.svg) 

## Build instructions
The build instructions are the same for both Windows (tested on Windows 10) and Linux (tested on Ubuntu 20.04):

- Get the latet release of [CMake for your platform](https://cmake.org/download/)
- Create a folder ``build`` inside the cloned repository folder:
```
mkdir build
```
- Navigate to ``build``:
```
cd build
```
- Configure the project and generate a build system:
```
cmake ..
```
- Then perform the actual build (in Release mode):
```
cmake --build . --config Release
```

And once more from the top for easy copy-and-paste:
```
git clone https://github.com/nullpunktTUD/SerialPort/
cd SerialPort
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Getting started
To use the `SerialPort` in your own project, add the folder `SerialPort/include` to your include directories and link against `SerialPort/lib/Release/SerialPort.lib` on Windows or `SerialPort/lib/Release/libSerialPort.a` on Linux.

Here is a minimal example to get you started:

```cpp
#include <chrono>
#include <iostream>
#include <thread>

#include <serial_port/serial_port.h>

using namespace serial_port;

int main() 
{
  // Instantiate two ports (Make sure these are available and connected to each other on your system)
  // Otherwise exceptions will be thrown!
  SerialPort out_port("COM1", 9600);  // Use appropriate name on Linux, e.g., /dev/ttyS0
  SerialPort in_port("COM2", 9600);  // Use appropriate name on Linux, e.g., /dev/ttyS1
  
  // Open both ports
  out_port.Open();
  in_port.Open();
  
  // Write out a string (mind that you are responsible to terminate with \n yourself!)
  const auto out_string = std::string("Hello World!\r\n");  
  out_port.WriteString(out_string);

  // Allow some time for data transfer to take place
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  
  // Read a string from the listening port (currently always expects \n as the string terminator)
  const auto in_string = in_port.ReadString();
  
  // Print out string
  std::cout << in_string << std::endl;
    
  return 0;
} // Ports are closed automatically in their destructor

```

For more details, check out the [documentation](https://simon-stone.github.io/SerialPort/).
