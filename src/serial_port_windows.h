#ifndef SERIAL_PORT_WINDOWS_H_
#define SERIAL_PORT_WINDOWS_H_

#if defined(_WIN32)

#include <serial_port/serial_port.h>

#include "interface.h"
#include <Windows.h>

namespace serial_port
{
	class SerialPortWindows : public Interface
	{
	public:
		// Inherit the constructors from the interface
		using Interface::Interface;

		// Make sure the port gets properly closed on destruction
		~SerialPortWindows() override { SerialPortWindows::Close(); }
		
		// Implement the interface
		void Open() override;
		void Close() override;
		bool IsOpen() override;

		unsigned long NumBytesAvailable() override;
		void FlushBuffer() const override;

		unsigned long ReadData(char* data, unsigned long num_bytes) override;
		unsigned long WriteData(const char* data, unsigned long num_bytes) override;

	private:
		HANDLE handle_{ INVALID_HANDLE_VALUE };
		COMMCONFIG comm_config_;
		COMMTIMEOUTS comm_timeouts_;
		bool isOpen_{ false };
	};
}

#endif // _WIN32

#endif // !SERIAL_PORT_WINDOWS_H_


