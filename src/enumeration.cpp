#include "enumeration.h"

#include <algorithm>

#if defined(_WIN32)

#include <atlbase.h>
#include <atlstr.h>

namespace
{
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

	std::vector<serial_port::PortInfo> enumerate_windows()
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
			more_items_left = RegEnumValue(serial_comm_key, static_cast<DWORD>(ports.size()), device_name.data(),
				&value_name_size, nullptr, nullptr, nullptr, nullptr);
			if (more_items_left == ERROR_NO_MORE_ITEMS) break;
			auto short_name = get_registry_value_string(serial_comm_key, device_name.data());
			ports.emplace_back(device_name, short_name);
		}

		std::sort(ports.begin(), ports.end());
		return ports;
	}
}
#endif

#if defined(__linux__)

#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

#include <iostream>

namespace {
	std::string get_driver(const ::std::string& tty) {
		struct stat file_status {};
		::std::string device_dir = tty;
		device_dir += "/device";

		// Get the file status of the device_dir
		if (lstat(device_dir.c_str(), &file_status) != 0) {
			return {};
		}
		// Since the device_dir is a symbolic link, find the actual location
		if (S_ISLNK(file_status.st_mode)) 
		{
			char buffer[1024];
			memset(buffer, 0, sizeof(buffer));

			// Append '/driver' and return basename of the target
			device_dir += "/driver";
			if (readlink(device_dir.c_str(), buffer, sizeof(buffer)) > 0) {
				return basename(buffer);
			}
		}
		return {};
	}

	void
		register_comport(std::vector<std::string>& comList, std::vector<std::string>& comList8250, const std::string& dir) {
		// Get the driver the device is using
		std::string driver = get_driver(dir);

		// Skip devices without a driver
		if (driver.size() > 0) 
		{
			std::string devfile = std::string("/dev/") + basename(dir.c_str());

			// Put serial8250-devices in a separate list
			if (driver == "serial8250") 
			{
				comList8250.push_back(devfile);
			}
			else 
			{
				comList.push_back(devfile);
			}
		}
	}

	void probe_serial8250_comports(std::vector<std::string>& comList, std::vector<std::string> comList8250) {
		struct serial_struct serinfo;
		auto it = comList8250.begin();

		// Iterate over all serial8250-devices
		while (it != comList8250.end()) 
		{
			// Try to open the device
			int fd = open((*it).c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY);

			if (fd >= 0) {
				// Get serial_info
				if (ioctl(fd, TIOCGSERIAL, &serinfo) == 0) 
				{
					// If device type is no PORT_UNKNOWN we accept the port
					if (serinfo.type != PORT_UNKNOWN)
						comList.push_back(*it);
				}
				close(fd);
			}
			it++;
		}
	}

	std::vector<serial_port::PortInfo> enumerate_linux()
	{
		int n;
		struct dirent** namelist;
		std::vector<std::string> comList;
		std::vector<std::string> comList8250;

		// The directory /sys/class/tty contains all TTY devices on the system
		const char* sysdir = "/sys/class/tty/";
		n = scandir(sysdir, &namelist, nullptr, nullptr);

		if (n < 0)
			perror("scandir");
		else
		{
			while (n--)
			{
				if (strcmp(namelist[n]->d_name, "..") != 0 && strcmp(namelist[n]->d_name, ".") != 0) {
					// Construct full absolute file path
					std::string devicedir = sysdir;
					devicedir += namelist[n]->d_name;

					// Register the device
					register_comport(comList, comList8250, devicedir);
				}
				free(namelist[n]);
			}
			free(namelist);
		}

		// Only non-serial8250 have been added to the comList so far
		// The serial8250-devices must be probed because they may be non-functional
		probe_serial8250_comports(comList, comList8250);

		// Convert to PortInfo objects
		std::vector<serial_port::PortInfo> port_info;
		for (const auto& port : comList)
		{
			port_info.emplace_back(port, port);  // Linux does not have long names for the serial ports
		}
		return port_info;
	}
}
#endif
std::vector<serial_port::PortInfo> enumeration::enumerate()
{
#if defined(_WIN32)
	return enumerate_windows();
#elif defined(__linux__)
	return enumerate_linux();

#endif
}
