#ifndef TYPES_H
#define TYPES_H

#include <stdexcept>
#include <string>

namespace serial_port
{
    enum class Parity { kNone, kOdd, kEven };
    enum class NumStopBits { kOne, kTwo };
    struct Settings
    {
        std::string port_name;
        int baud_rate{ 9600 };
        Parity parity{ Parity::kNone };
        NumStopBits num_stop_bits{ NumStopBits::kOne };
        bool hardware_flow_control{ false };
        unsigned long timeout_s{ 0 };
        unsigned long timeout_ms{ 0 };
    };

    // Exceptions
    using IoException = std::runtime_error;
}
#endif // TYPES_H
