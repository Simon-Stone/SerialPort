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
	/// @brief Parity modes
	enum class Parity { kNone, kOdd, kEven };
	/// @brief Number of stop bits used
	enum class NumStopBits { kOne, kTwo };

	/// @brief Holds information about a port
	struct PortInfo
	{
		/// @brief Default constructor
		PortInfo() = default;
		/// @brief Constructor initializing info
		/// @param long_name Long name of the port (same as short name on Linux)
		/// @param short_name Short name of the port (e.g. "COM1" or "/dev/ttyS0")
		PortInfo(const std::string& long_name, const std::string& short_name) : long_name(long_name), short_name(short_name) {}
		/// @brief The long name of the port (same as short name on Linux)
		std::string long_name;
		/// @brief short_name Short name of the port (e.g. "COM1" or "/dev/ttyS0")
		std::string short_name;
		/// @brief Overloaded stream output operator to display port information 
		friend std::ostream& operator<<(std::ostream& os, const PortInfo& obj)
		{
			return os
				<< "Port info - "
				<< "Long name: " << obj.long_name
				<< "\t"
				<< "Short name: " << obj.short_name;
		}

		/// @brief Overloaded relational operators that allow sorting PortInfo objects by their short name
		friend bool operator<(const PortInfo& lhs, const PortInfo& rhs) { return lhs.short_name < rhs.short_name; }
		friend bool operator<=(const PortInfo& lhs, const PortInfo& rhs) { return !(rhs < lhs); }
		friend bool operator>(const PortInfo& lhs, const PortInfo& rhs) { return rhs < lhs; }
		friend bool operator>=(const PortInfo& lhs, const PortInfo& rhs) { return !(lhs < rhs); }
	};

	/// @brief Describes the settings of a port 
	struct Settings
	{
		/// @brief Default constructor
		Settings() = default;
		/// @brief Constructor initializing the settings
		/// @param port_name Name of the port
		/// @param baud_rate Baud rate
		/// @param parity Parity used
		/// @param num_stop_bits Number of stop bits
		/// @param hardware_flow_control Hardware flow control (on or off)
		/// @param timeout_s Timeout in seconds
		/// @param timeout_ms Timeout in milliseconds
		Settings(const std::string& port_name, const int baud_rate, const Parity parity, const NumStopBits num_stop_bits,
		         const bool hardware_flow_control, const unsigned long timeout_s, const unsigned long timeout_ms) :
			port_name(port_name), baud_rate(baud_rate), parity(parity), num_stop_bits(num_stop_bits),
			hardware_flow_control(hardware_flow_control), timeout_s(timeout_s), timeout_ms(timeout_ms)
		{
		}
		/// @brief Name of the port
		std::string port_name{ kDefaultName };
		/// @brief Baud rate
		int baud_rate{ 9600 };
		/// @brief Parity
		Parity parity{ Parity::kNone };
		/// @brief Number of stop bits
		NumStopBits num_stop_bits{ NumStopBits::kOne };
		/// @brief Hardware flow control
		bool hardware_flow_control{ false };
		/// @brief Timeout in seconds
		unsigned long timeout_s{ 0 };
		/// @brief Timeout in milliseconds
		unsigned long timeout_ms{ 0 };
		/// @brief Overloaded equality operator
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
		/// @brief Overloaded inequality operator
		friend bool operator!=(const Settings& lhs, const Settings& rhs)
		{
			return !(lhs == rhs);
		}
		/// @brief Overloaded stream output operator
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

	/// @brief An exception that is thrown when input or output operations go wrong
	using IoException = std::runtime_error;
}
#endif // TYPES_H
