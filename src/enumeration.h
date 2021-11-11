#ifndef ENUMERATION_H
#define ENUMERATION_H

#include <vector>
#include <string>

#include "serial_port/types.h"

#if defined(_WIN32)
#include <atlbase.h>
#include <atlstr.h>
namespace Enumeration
{
	std::vector<serial_port::PortInfo> enumerate_from_registry();
}
#endif

#if defined(__linux__)
namespace Enumeration
{
    std::vector<serial_port::PortInfo> get_available_ports();
}
#endif

#endif // ENUMERATION_H
