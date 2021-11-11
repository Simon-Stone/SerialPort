#include "interface.h"


serial_port::Interface::Interface(const serial_port::Settings& settings) : settings_(settings)
{
}

serial_port::Interface::Interface(const std::string& port_name, const int baud_rate,
                                         const serial_port::Parity parity,
                                         const serial_port::NumStopBits stop_bits,
                                         const bool hardware_flow_control,
                                         const unsigned long timeout_s, const unsigned long timeout_ms)
	: settings_{ port_name, baud_rate, parity, stop_bits, hardware_flow_control, timeout_s, timeout_ms }
{
}

// A function body for the pure virtual destructor is still required!
serial_port::Interface::~Interface()
= default;

const serial_port::Settings& serial_port::Interface::GetSettings() const
{
    return settings_;
}

std::string serial_port::Interface::ReadString()
{
	char c{ 'a' };
	std::string str;
	while (c != '\n')
	{
		ReadData(&c, 1);
		str.push_back(c);
	}

	return str;
}

unsigned long serial_port::Interface::WriteString(const std::string& str)
{
	return WriteData(str.c_str(), static_cast<unsigned long>(str.size()));
}
