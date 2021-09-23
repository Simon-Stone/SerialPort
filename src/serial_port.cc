#include "serial_port/serial_port.h"

#if defined(_WIN32)
#include "serial_port_windows.h"
#endif

serial_port::SerialPort::SerialPort()
{
#if defined(_WIN32)
	sp_ = new SerialPortWindows();
#endif
}

serial_port::SerialPort::~SerialPort()
{
	delete sp_;
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
