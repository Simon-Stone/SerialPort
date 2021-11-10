#if defined(_WIN32)
#include "serial_port_windows.h"


namespace
{
	unsigned get_baud_rate(unsigned long baud)
	{
		unsigned baud_rate{ 0 };
		switch (baud)
		{
		case 110:
			baud_rate = CBR_110;
			break;
		case 300:
			baud_rate = CBR_300;
			break;
		case 600:
			baud_rate = CBR_600;
			break;
		case 1200:
			baud_rate = CBR_1200;
			break;
		case 2400:
			baud_rate = CBR_2400;
			break;
		case 4800:
			baud_rate = CBR_4800;
			break;
		case 9600:
			baud_rate = CBR_9600;
			break;
		case 14400:
			baud_rate = CBR_14400;
			break;
		case 19200:
			baud_rate = CBR_19200;
			break;
		case 38400:
			baud_rate = CBR_38400;
			break;
		case 56000:
			baud_rate = CBR_56000;
			break;
		case 57600:
			baud_rate = CBR_57600;
			break;
		case 115200:
			baud_rate = CBR_115200;
			break;
		case 128000:
			baud_rate = CBR_128000;
			break;
		case 256000:
			baud_rate = CBR_256000;
			break;
		default:
			// Invalid baud rate
			throw serial_port::IoException("[SerialPortWindows::get_baud_rate()] Invalid baud rate requested.");
		}

		return baud_rate;
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
	const DWORD IN_QUEUE_SIZE = 32768;
	const DWORD OUT_QUEUE_SIZE = 32768;
	SetupComm(handle_, IN_QUEUE_SIZE, OUT_QUEUE_SIZE);

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

	COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(handle_, &timeouts))
	{
		throw IoException("[SerialPortWindows::Open()] Error setting the timeouts.");
	}

	if (SetCommConfig(handle_, &comm_config_, sizeof(COMMCONFIG)) == 0)
	{
		throw IoException("[SerialPortWindows::Open()] Error setting the port settings! Error code: " + std::to_string(GetLastError()));
	}

	isOpen_ = true;
}

void serial_port::SerialPortWindows::Close()
{
	if (this->IsOpen())
	{
		CloseHandle(handle_);
		handle_ = INVALID_HANDLE_VALUE;
		isOpen_ = false;
	}
}

bool serial_port::SerialPortWindows::IsOpen()
{
	return isOpen_;
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