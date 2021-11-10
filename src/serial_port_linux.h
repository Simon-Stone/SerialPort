#ifndef SERIAL_PORT_LINUX_H_
#define SERIAL_PORT_LINUX_H_

#if defined(__linux__)

#include <serial_port/serial_port.h>
#include "interface.h"

namespace serial_port
{
	class SerialPortLinux : public Interface
	{
	public:
		// Inherit the constructors from the interface
		using Interface::Interface;

		// Make sure the port gets properly closed on destruction
		~SerialPortLinux() override { SerialPortLinux::Close(); }

		// Implement the interface
		void Open() override;
		void Close() override;
		bool IsOpen() override;

		unsigned long NumBytesAvailable() override;
		void FlushBuffer() const override;

		unsigned long ReadData(char* data, unsigned long num_bytes) override;
		unsigned long WriteData(const char* data, unsigned long num_bytes) override;

		std::string ReadString() override;
		unsigned long WriteString(const std::string& str) override;

	private:
		int handle_{ -1 };
	};
}

#endif // __linux__

#endif // !SERIAL_PORT_LINUX_H_