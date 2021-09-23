#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include "../src/interface.h"
#include "types.h"

namespace serial_port
{
    class SerialPort
    {
    public:
        SerialPort();
        ~SerialPort();

        void Open();
        void Close();
        bool IsOpen();
        const Settings& GetSettings() const;

        unsigned long NumBytesAvailable();
        void FlushBuffer() const;
        unsigned long ReadData(char* data, unsigned long num_bytes);
        unsigned long WriteData(const char* data, unsigned long num_bytes);

    private:
        Interface* sp_;
    };

}


#endif // !SERIAL_PORT_H