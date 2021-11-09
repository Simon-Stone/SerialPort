#include "serial_port/serial_port.h"

#if defined(_WIN32)
#include "serial_port_windows.h"
#elif defined(__linux__)
#include "serial_port_linux.h"
#else
#error Unknown platform!
#endif

serial_port::SerialPort::SerialPort()
{
#if defined(_WIN32)
	sp_.reset( new SerialPortWindows());
#elif defined (__linux__)
	sp_.reset(new SerialPortLinux());
#endif
}

serial_port::SerialPort::SerialPort(const Settings& settings)
{
#if defined(_WIN32)
	sp_.reset(new SerialPortWindows(settings));
#elif defined (__linux__)
	sp_.reset(new SerialPortLinux(settings));
#endif
}

serial_port::SerialPort::SerialPort(const std::string& port_name, int baud_rate, Parity parity,
	NumStopBits stop_bits, bool hardware_flow_control, unsigned long timeout_s, unsigned long timeout_ms)
{
#if defined(_WIN32)
	sp_.reset(new SerialPortWindows(port_name, baud_rate, parity, stop_bits, hardware_flow_control, timeout_s));
#elif defined (__linux__)
		sp_.reset(new SerialPortLinux(port_name, baud_rate, parity, stop_bits, hardware_flow_control, timeout_s));
#endif
}

void serial_port::SerialPort::Open()
{
	sp_->Open();
}

void serial_port::SerialPort::Close()
{
	sp_->Close();
}

bool serial_port::SerialPort::IsOpen()
{
	return sp_->IsOpen();
}

const serial_port::Settings& serial_port::SerialPort::GetSettings() const
{
	return sp_->GetSettings();
}

unsigned long serial_port::SerialPort::NumBytesAvailable()
{
	return sp_->NumBytesAvailable();
}

void serial_port::SerialPort::FlushBuffer() const
{
	return sp_->FlushBuffer();
}

unsigned long serial_port::SerialPort::ReadData(char* data, unsigned long num_bytes)
{
	return sp_->ReadData(data, num_bytes);
}

unsigned long serial_port::SerialPort::WriteData(const char* data, unsigned long num_bytes)
{
	return sp_->WriteData(data, num_bytes);
}
