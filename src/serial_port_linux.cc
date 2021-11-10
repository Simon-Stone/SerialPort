#if defined(__linux__)

#include <fcntl.h> // Contains file controls like O_RDWR
#include <cerrno> // Error integer 
#include <cstring> // strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include "serial_port_linux.h"

namespace
{
    unsigned get_baud_rate(unsigned long baud) {
        unsigned baud_rate;
        switch (baud) {
            case 0:
                baud_rate = B0;
                break;
            case 50:
                baud_rate = B50;
                break;
            case 75:
                baud_rate = B75;
                break;
            case 110:
                baud_rate = B110;
                break;
            case 134:
                baud_rate = B134;
                break;
            case 150:
                baud_rate = B150;
                break;
            case 200:
                baud_rate = B200;
                break;
            case 300:
                baud_rate = B300;
                break;
            case 600:
                baud_rate = B600;
                break;
            case 1200:
                baud_rate = B1200;
                break;
            case 1800:
                baud_rate = B1800;
                break;
            case 2400:
                baud_rate = B2400;
                break;
            case 4800:
                baud_rate = B4800;
                break;
            case 9600:
                baud_rate = B9600;
                break;
            case 19200:
                baud_rate = B19200;
                break;
            case 38400:
                baud_rate = B38400;
                break;
            case 57600:
                baud_rate = B57600;
                break;
            case 115200:
                baud_rate = B115200;
                break;
            case 230400:
                baud_rate = B230400;
                break;
            case 460800:
                baud_rate = B460800;
                break;
            case 500000:
                baud_rate = B500000;
                break;
            case 576000:
                baud_rate = B576000;
                break;
            case 921600:
                baud_rate = B921600;
                break;
            case 1000000:
                baud_rate = B1000000;
                break;
            case 1152000:
                baud_rate = B1152000;
                break;
            case 1500000:
                baud_rate = B1500000;
                break;
            case 2000000:
                baud_rate = B2000000;
                break;
            case 2500000:
                baud_rate = B2500000;
                break;
            case 3000000:
                baud_rate = B3000000;
                break;
            case 3500000:
                baud_rate = B3500000;
                break;
            case 4000000:
                baud_rate = B4000000;
                break;
            default:
                throw std::invalid_argument("Requested baud rate of " + std::to_string(baud_rate) + " not supported!");
        }
        return baud_rate;
    }
}

void serial_port::SerialPortLinux::Open()
{
	handle_ = open(settings_.port_name.c_str(), O_RDWR);

	if (handle_ < 0)
	{
		throw IoException("[SerialPortLinux::Open()] Could not open serial port.");
	}

    if (tcgetattr(handle_, &tty_) != 0)
    {
        throw IoException("[SerialPortLinux::Open()] Error from tcgetattr(): " + std::string(strerror(errno)));
    }

    // Parity
    switch (settings_.parity)
    {
        case Parity::kNone:
            tty_.c_cflag &= ~PARENB;
            break;
        case Parity::kOdd:
            tty_.c_cflag |= PARENB;
            tty_.c_cflag |= PARODD;
            break;
        case Parity::kEven:
            tty_.c_cflag |= PARENB;
            tty_.c_cflag &= ~PARODD;
            break;
    }
    // Baud rate
    cfsetispeed(&tty_, get_baud_rate(settings_.baud_rate));

    // Character size
    tty_.c_cflag &= ~CSIZE;
    tty_.c_cflag |= CS8;

    // Hardware flow control
    if (settings_.hardware_flow_control)
    {
        tty_.c_cflag |= CRTSCTS;
    }
    else
    {
        tty_.c_cflag &= ~CRTSCTS;
    }

    // Use raw input
    tty_.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Stop bits
    switch(settings_.num_stop_bits)
    {
        case NumStopBits::kOne:
            tty_.c_cflag &= ~CSTOPB;
            break;
        case NumStopBits::kTwo:
            tty_.c_cflag |= CSTOPB;
    }

    // Timeout
    // TODO: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

    // Set the settings
    tcsetattr(handle_, TCSANOW, &tty_);
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
	return read(handle_, data, num_bytes);
}

unsigned long serial_port::SerialPortLinux::WriteData(const char* data, unsigned long num_bytes)
{
	return write(handle_, data, num_bytes);
}


#endif // __linux__