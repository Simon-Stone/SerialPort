#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <memory>
#include <ostream>
#include <vector>
#include <string>

#include "../src/interface.h"
#include "types.h"

namespace serial_port
{
    /// @brief A SerialPort class
    /// @details This class represents a serial port on the machine and handles opening, writing, reading, and closing a port.
    class SerialPort
    {
    public:
        /// @brief Default constructor. Does not open any port.
        SerialPort();
        /// @brief Create a port with specific settings (but do not open it)
        /// @param settings A Settings object 
        explicit SerialPort(const Settings& settings);
        /// @brief Create a port with specific settings (but do not open it)
        /// @param port_name Name of the port (e.g. "COM1" on Windows or "/dev/ttyS0" on Linux)
        /// @param baud_rate A baud rate. Any value will be accepted here. It will be checked for validity when the port is actually opened.
        /// @param parity The parity setting for the port.
        /// @param stop_bits The number of stop bits being used.
        /// @param hardware_flow_control Whether or not to use hardware flow control.
        /// @param timeout_s Timeout value in seconds (currently not used!)
        /// @param timeout_ms Timeout value in milliseconds (currently not used!)
        SerialPort(const std::string& port_name, int baud_rate,
            Parity parity = Parity::kNone,
            NumStopBits stop_bits = serial_port::NumStopBits::kOne,
            bool hardware_flow_control = false,
            unsigned long int timeout_s = 0, unsigned long int timeout_ms = 0);
        /// @brief Default destructor. Port will be closed.
        ~SerialPort() = default;

        /// @brief SerialPort objects may be moved
        SerialPort(SerialPort&&) = default;
        SerialPort& operator=(SerialPort&& other) noexcept { this->sp_ = std::move(other.sp_); return *this; }

        /// @brief SerialPort objects may not be copied
        SerialPort(const SerialPort&) = delete;
        SerialPort& operator=(const SerialPort& other) = delete;
        
        /// @brief A static function to enumerate available ports
        /// @return A list of PortInfo objects describing the available ports
        static std::vector<PortInfo> EnumeratePorts();

        /// @brief Open the port with the current settings. If a port was opened through this object previously, it will be closed first.
        void Open() const;
        /// @brief Close the port.
        void Close() const;
        /// @brief Returns whether or not the port is currently open
        [[nodiscard]] bool IsOpen() const;
        /// @brief Get the currently defined settings
        [[nodiscard]] const Settings& GetSettings() const;
        /// @brief Return the number of bytes available in the RX buffer
        [[nodiscard]] unsigned long NumBytesAvailable() const;
        /// @brief Flush the RX and TX buffers
        void FlushBuffer() const;
        /// @brief Read data from the port.
        /// @param data A pointer to a char array. Must be at least num_bytes elements long!
        /// @param num_bytes The number of bytes to attempt to read from the port
        /// @return The actual number of bytes read
        unsigned long ReadData(char* data, unsigned long num_bytes) const;
        /// @brief Read a string from the port terminated with a '\\n' symbol.
        [[nodiscard]] std::string ReadString() const;
        /// @brief Write data to the port
        /// @param data An array of bytes to write
        /// @param num_bytes the number of bytes in the array
        /// @return The number of bytes actually written
        unsigned long WriteData(const char* data, unsigned long num_bytes) const;
        /// @brief Write a string to the port.
        /// @param str A string terminated by a '\\n' symbol
        /// @return The number of bytes actually written
        unsigned long WriteString(const std::string& str) const;  // NOLINT(modernize-use-nodiscard)
        /// @brief Overloaded stream output operator to print the port settings
        friend std::ostream& operator<<(std::ostream& os, const SerialPort& obj)
        {
            os << "Serial Port: " << std::endl;
        	os << obj.sp_->GetSettings();
            return os;
        }

    private:
        /// @brief Pointer to implementation (PIMPL idiom)
        std::unique_ptr<Interface> sp_;
    };

}


#endif // !SERIAL_PORT_H