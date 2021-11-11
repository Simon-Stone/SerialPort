#include "enumeration.h"

#include <algorithm>

namespace Enumeration
{
#if defined(_WIN32)
	std::string get_registry_value_string(ATL::CRegKey& key, const LPCTSTR name)
	{
		std::string value;
		value.clear();

		// Query only the length of the string at first
		ULONG string_length = 0;
		const LSTATUS status = key.QueryStringValue(name, nullptr, &string_length);
		if (status != ERROR_SUCCESS)
		{
			SetLastError(status);
			return {};
		}

		// Allocate space and then get the actual string		
		value.resize(string_length);
		key.QueryStringValue(name, value.data(), &string_length);
		
		return value;
	}

	std::vector<serial_port::PortInfo> enumerate_from_registry()
	{
		std::vector<serial_port::PortInfo> ports;
		ATL::CRegKey serial_comm_key;
		LSTATUS status = serial_comm_key.Open(HKEY_LOCAL_MACHINE, 
			"Hardware\\DeviceMap\\SerialComm", KEY_QUERY_VALUE);
		if (status != ERROR_SUCCESS)
		{
			SetLastError(status);
			return {};
		}

		// Get the maximum length of a value name
		DWORD max_value_name_len = 0;
		status = RegQueryInfoKey(serial_comm_key, nullptr, nullptr, nullptr, 
			nullptr, nullptr, nullptr, nullptr, 
			&max_value_name_len, nullptr, nullptr, nullptr);
		if (status != ERROR_SUCCESS)
		{
			SetLastError(status);
			return {};
		}

		max_value_name_len += 1; // Reserve one more for the \0 terminator

		std::string device_name;
		device_name.resize(max_value_name_len);

		// Enumerate all values under "Hardware\\DeviceMap\\SerialComm")
		int more_items_left = ERROR_SUCCESS;
		while (more_items_left == ERROR_SUCCESS)
		{
			DWORD value_name_size = max_value_name_len;
			more_items_left = RegEnumValue(serial_comm_key, ports.size(), device_name.data(),
				&value_name_size, nullptr, nullptr, nullptr, nullptr);
			if (more_items_left == ERROR_NO_MORE_ITEMS) break;
			auto short_name = get_registry_value_string(serial_comm_key, device_name.data());
			ports.emplace_back(device_name, short_name);
		}

		std::sort(ports.begin(), ports.end());
		return ports;
	}
#endif
}
