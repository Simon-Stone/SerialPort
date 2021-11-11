#ifndef TYPES_H
#define TYPES_H

#include <ostream>
#include <stdexcept>
#include <string>


#if defined (__linux__)
#define kDefaultName "ttyS0"
#elif (_WIN32)
constexpr auto kDefaultName = "COM1";
#endif

namespace serial_port
{
	enum class Parity { kNone, kOdd, kEven };
	enum class NumStopBits { kOne, kTwo };

	struct PortInfo
	{
		PortInfo() = default;
		PortInfo(const std::string& long_name, const std::string& short_name) : long_name(long_name), short_name(short_name) {}

		std::string long_name;
		std::string short_name;

		friend std::ostream& operator<<(std::ostream& os, const PortInfo& obj)
		{
			return os
				<< "Port info - "
				<< "Long name: " << obj.long_name
				<< "\t"
				<< "Short name: " << obj.short_name;
		}

		// Port info should be sorted by short name, so define the relations that way
		friend bool operator<(const PortInfo& lhs, const PortInfo& rhs) { return lhs.short_name < rhs.short_name; }
		friend bool operator<=(const PortInfo& lhs, const PortInfo& rhs) { return !(rhs < lhs); }
		friend bool operator>(const PortInfo& lhs, const PortInfo& rhs) { return rhs < lhs; }
		friend bool operator>=(const PortInfo& lhs, const PortInfo& rhs) { return !(lhs < rhs); }
	};

	struct Settings
	{
		Settings() = default;
		Settings(const std::string& port_name, const int baud_rate, const Parity parity, const NumStopBits num_stop_bits,
		         const bool hardware_flow_control, const unsigned long timeout_s, const unsigned long timeout_ms) :
			port_name(port_name), baud_rate(baud_rate), parity(parity), num_stop_bits(num_stop_bits),
			hardware_flow_control(hardware_flow_control), timeout_s(timeout_s), timeout_ms(timeout_ms)
		{
		}
		std::string port_name{ kDefaultName };
		int baud_rate{ 9600 };
		Parity parity{ Parity::kNone };
		NumStopBits num_stop_bits{ NumStopBits::kOne };
		bool hardware_flow_control{ false };
		unsigned long timeout_s{ 0 };
		unsigned long timeout_ms{ 0 };

		friend bool operator==(const Settings& lhs, const Settings& rhs)
		{
			return lhs.port_name == rhs.port_name
				&& lhs.baud_rate == rhs.baud_rate
				&& lhs.parity == rhs.parity
				&& lhs.num_stop_bits == rhs.num_stop_bits
				&& lhs.hardware_flow_control == rhs.hardware_flow_control
				&& lhs.timeout_s == rhs.timeout_s
				&& lhs.timeout_ms == rhs.timeout_ms;
		}

		friend bool operator!=(const Settings& lhs, const Settings& rhs)
		{
			return !(lhs == rhs);
		}

		friend std::ostream& operator<<(std::ostream& os, const Settings& obj)
		{
			std::string parity_string;
			switch (obj.parity)
			{
			case Parity::kNone:
				parity_string = "none";
				break;
			case Parity::kOdd:
				parity_string = "odd";
				break;
			case Parity::kEven:
				parity_string = "even";
				break;
			}
			return os
				<< "Name: " << obj.port_name << std::endl
				<< "Baud rate: " << obj.baud_rate << std::endl
				<< "Parity: " << parity_string << std::endl
				<< "Number of stop bits: " << (obj.num_stop_bits == NumStopBits::kOne ? "one" : "two") << std::endl
				<< "Hardware flow control: " << obj.hardware_flow_control << std::endl
				<< "Timeout [s]: " << obj.timeout_s << std::endl
				<< "Timeout [ms]: " << obj.timeout_ms;
		}
	};

	// Exceptions
	using IoException = std::runtime_error;
}
#endif // TYPES_H
