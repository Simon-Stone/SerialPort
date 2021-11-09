#include <gtest/gtest.h>

#include <iostream>

#include "serial_port/serial_port.h"

// Test the various ways to instantiate a serial port
TEST(SerialPortTests, Instantiation)
{
	serial_port::SerialPort default_port;
	ASSERT_EQ(default_port.GetSettings(), serial_port::Settings());

	auto custom_settings = serial_port::Settings("COM2", 115200, serial_port::Parity::kNone,
	                                             serial_port::NumStopBits::kTwo, false, 0, 10);
	serial_port::SerialPort settings_port(custom_settings);
	ASSERT_EQ(settings_port.GetSettings(), custom_settings);
}

// Test opening a port
TEST(SerialPortTests, Open)
{
	serial_port::SerialPort port("COM4", 9600);
	port.Open();
	port.Close();
}