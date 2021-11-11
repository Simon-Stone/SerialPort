#if defined(_WIN32)
#include "serial_port_windows.h"


namespace
{
	unsigned get_baud_rate(unsigned long baud)
	{
		switch (baud)
		{
		case 110:
			return CBR_110;
		case 300:
			return CBR_300;
		case 600:
			return CBR_600;
		case 1200:
			return CBR_1200;
		case 2400:
			return CBR_2400;
		case 4800:
			return CBR_4800;
		case 9600:
			return CBR_9600;
		case 14400:
			return CBR_14400;
		case 19200:
			return CBR_19200;
		case 38400:
			return CBR_38400;
		case 56000:
			return CBR_56000;
		case 57600:
			return CBR_57600;
		case 115200:
			return CBR_115200;
		case 128000:
			return CBR_128000;
		case 256000:
			return CBR_256000;
		default:
			// Invalid baud rate
			throw serial_port::IoException("[SerialPortWindows::get_baud_rate()] Invalid baud rate requested.");
		}
	}
}

void serial_port::SerialPortWindows::Open()
{
	if (IsOpen())
	{
		Close();
	}

	// ****************************************************************
	handle_ = CreateFile(settings_.port_name.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
	nullptr);

	if (handle_ == INVALID_HANDLE_VALUE)
	{
		throw IoException("[SerialPortWindows::Open()] Could not open serial port.");
	}

	// Set the recommended queue size for buffering
	constexpr DWORD in_queue_size = 32768;
	constexpr DWORD out_queue_size = 32768;
	SetupComm(handle_, in_queue_size, out_queue_size);

	// Prepare the settings structures
	unsigned long conf_size = sizeof(COMMCONFIG);
	comm_config_.dwSize = conf_size;
	GetCommConfig(handle_, &comm_config_, &conf_size);
	GetCommState(handle_, &(comm_config_.dcb));

	comm_config_.dcb.fBinary = TRUE;
	comm_config_.dcb.fInX = FALSE;
	comm_config_.dcb.fOutX = FALSE;
	comm_config_.dcb.fAbortOnError = FALSE;
	comm_config_.dcb.fNull = FALSE;

	comm_config_.dcb.BaudRate = get_baud_rate(settings_.baud_rate);

	comm_config_.dcb.ByteSize = 8;

	switch (settings_.num_stop_bits)
	{
	case NumStopBits::kOne:
		comm_config_.dcb.StopBits = ONESTOPBIT;
		break;

	case NumStopBits::kTwo:
		comm_config_.dcb.StopBits = TWOSTOPBITS;
		break;
	}

	switch (settings_.parity)
	{
	case Parity::kNone:
		comm_config_.dcb.Parity = NOPARITY;
		comm_config_.dcb.fParity = FALSE;
		break;

	case Parity::kEven:
		comm_config_.dcb.Parity = EVENPARITY;
		comm_config_.dcb.fParity = TRUE;
		break;

	case Parity::kOdd:
		comm_config_.dcb.Parity = ODDPARITY;
		comm_config_.dcb.fParity = TRUE;
		break;
	}

	comm_config_.dcb.fOutxCtsFlow = FALSE;
	if (settings_.hardware_flow_control)
	{
		comm_config_.dcb.fRtsControl = DTR_CONTROL_ENABLE;
	}
	else
	{
		comm_config_.dcb.fRtsControl = RTS_CONTROL_DISABLE;
	}
	comm_config_.dcb.fInX = FALSE;
	comm_config_.dcb.fOutX = FALSE;

	// TODO: Use user-supplied timeouts!
	COMMTIMEOUTS timeouts{MAXDWORD, 0, 0, 10, 50};

	if (!SetCommTimeouts(handle_, &timeouts))
	{
		throw IoException("[SerialPortWindows::Open()] Error setting the timeouts.");
	}

	if (SetCommConfig(handle_, &comm_config_, sizeof(COMMCONFIG)) == 0)
	{
		throw IoException("[SerialPortWindows::Open()] Error setting the port settings! Error code: " + std::to_string(GetLastError()));
	}
}

void serial_port::SerialPortWindows::Close()
{
	if (this->IsOpen())
	{
		CloseHandle(handle_);
		handle_ = INVALID_HANDLE_VALUE;
	}
}

bool serial_port::SerialPortWindows::IsOpen()
{
	return handle_ != INVALID_HANDLE_VALUE;
}

unsigned long serial_port::SerialPortWindows::NumBytesAvailable()
{
	// Determine the number of bytes in the RX buffer of the device.
	COMSTAT com_stat;
	DWORD error_mask = 0;

	// Get the COM port status.
	ClearCommError(handle_, &error_mask, &com_stat);

	// The number of bytes received by the serial provider but not yet 
	// read by a ReadFile operation.
	const unsigned long num_bytes = com_stat.cbInQue;

	return num_bytes;
}

void serial_port::SerialPortWindows::FlushBuffer() const
{
	if(!PurgeComm(handle_, PURGE_RXCLEAR))
	{
		throw std::runtime_error("[SerialPortWindows::FlushBuffer()] Failed to flush RX buffer.");
	}
}

unsigned long serial_port::SerialPortWindows::ReadData(char* data, unsigned long num_bytes)
{
	unsigned long bytes_read;
	ReadFile(handle_, data, num_bytes, &bytes_read, nullptr);

	return bytes_read;
}

unsigned long serial_port::SerialPortWindows::WriteData(const char* data, unsigned long num_bytes)
{
	if(!IsOpen())
	{
		return 0;
	}

	unsigned long bytes_written;

	if(WriteFile(handle_, data, num_bytes, &bytes_written, nullptr))
	{
		FlushFileBuffers(handle_);
		return bytes_written;
	}
	else
	{
		return 0;
	}
}


#endif // _WIN32