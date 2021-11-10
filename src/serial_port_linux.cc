#if defined(__linux__)

#include <fcntl.h> // Contains file controls like O_RDWR
#include <cerrno> // Error integer 
#include <cstring> // strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include "serial_port_linux.h"


void serial_port::SerialPortLinux::Open()
{
	handle_ = open(settings_.port_name.c_str(), O_RDWR);

	if (handle_ < 0)
	{
		throw IoException("[SerialPortWindows::Open()] Could not open serial port.");
	}
}

void serial_port::SerialPortLinux::Close()
{
	close(handle_);
	handle_ = -1;
}

bool serial_port::SerialPortLinux::IsOpen()
{
	return handle_ >= 0;
}

unsigned long serial_port::SerialPortLinux::NumBytesAvailable()
{
	return 0;
}

void serial_port::SerialPortLinux::FlushBuffer() const
{
}

unsigned long serial_port::SerialPortLinux::ReadData(char* data, unsigned long num_bytes)
{
	return 0;
}

unsigned long serial_port::SerialPortLinux::WriteData(const char* data, unsigned long num_bytes)
{
	return 0;
}


#endif // __linux__