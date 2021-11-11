#ifndef ENUMERATION_H
#define ENUMERATION_H

#include <vector>

#include "serial_port/types.h"

#if defined(_WIN32)
#include <atlbase.h>
#include <atlstr.h>
namespace Enumeration
{
	std::vector<serial_port::PortInfo> enumerate_from_registry();
}

#endif
#endif // ENUMERATION_H
