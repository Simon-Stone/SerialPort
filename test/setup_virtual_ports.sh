#!/bin/bash
# This script starts two virtual serial ports for use in unit testing the SerialPort library.

touch virtual_port_1
touch virtual_port_2
socat pty,link=virtual_port_1,raw,echo=0 pty,link=virtual_port_2,raw,echo=0


