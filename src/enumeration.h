#ifndef ENUMERATION_H
#define ENUMERATION_H

#include <vector>

#include "serial_port/types.h"

namespace enumeration
{
	std::vector<serial_port::PortInfo> enumerate();
}

#endif // ENUMERATION_H
