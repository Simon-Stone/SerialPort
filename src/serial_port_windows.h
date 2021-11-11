#ifndef SERIAL_PORT_WINDOWS_H_
#define SERIAL_PORT_WINDOWS_H_

#if defined(_WIN32)

#include <serial_port/serial_port.h>

#include "interface.h"
#include <Windows.h>

namespace serial_port
{
	class SerialPortWindows final : public Interface
	{
	public:
		// Inherit the constructors from the interface
		using Interface::Interface;

		// Allow moving a serial port
		SerialPortWindows(SerialPortWindows&&) = default;
		SerialPortWindows& operator=(SerialPortWindows&& other) = default;
		// Do not allow copying a serial port
		SerialPortWindows(const SerialPortWindows&) = delete;
		SerialPortWindows& operator=(const SerialPortWindows& other) = delete;

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
		// ReSharper disable once CommentTypo
		// COMMTIMEOUTS comm_timeouts_;
	};
}

#endif // _WIN32

#endif // !SERIAL_PORT_WINDOWS_H_


