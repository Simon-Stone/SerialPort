#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <string>

namespace serial_port
{
	enum class Parity {kNone, kOdd, kEven};
    enum class NumStopBits { kOne, kTwo };
    struct Settings
    {
        std::string portName;
        int baudRate{ 9600 };
        Parity parity{ Parity::kNone };
        NumStopBits numStopBits{ NumStopBits::kOne };
        bool hardwareFlowControl{ false };
        unsigned long timeout_s{ 0 };
        unsigned long timeout_ms{ 0 };
    };
}




#endif // !SERIAL_PORT_H