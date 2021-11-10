#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <memory>
#include <ostream>

#include "../src/interface.h"
#include "types.h"

namespace serial_port
{
    class SerialPort
    {
    public:
        SerialPort();
        SerialPort(const Settings& settings);
        SerialPort(const std::string& port_name, int baud_rate,
            Parity parity = Parity::kNone,
            NumStopBits stop_bits = serial_port::NumStopBits::kOne,
            bool hardware_flow_control = false,
            unsigned long int timeout_s = 0, unsigned long int timeout_ms = 0);
        ~SerialPort() = default;

    	// Allow moving a serial port
        SerialPort(SerialPort&&) = default;
        SerialPort& operator=(SerialPort&& other) noexcept { this->sp_ = std::move(other.sp_); return *this; }

        // Do not allow copying a serial port
        SerialPort(const SerialPort&) = delete;
        SerialPort& operator=(const SerialPort& other) = delete;
        

        void Open();
        void Close();
        bool IsOpen();
        const Settings& GetSettings() const;

        unsigned long NumBytesAvailable();
        void FlushBuffer() const;
        unsigned long ReadData(char* data, unsigned long num_bytes);
        std::string ReadString();
        unsigned long WriteData(const char* data, unsigned long num_bytes);
        unsigned long WriteString(const std::string& str);

        friend std::ostream& operator<<(std::ostream& os, const SerialPort& obj)
        {
            os << "Serial Port: " << std::endl;
        	os << obj.sp_->GetSettings();
            return os;
        }

    private:
        std::unique_ptr<Interface> sp_;
    };

}


#endif // !SERIAL_PORT_H