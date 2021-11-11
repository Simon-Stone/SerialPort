# Tests for SerialPort

Unit testing a serial port library is difficult without knowledge of the available ports. Before running these tests, you should therefore make sure that you set port names in ``test.cc`` that are actually available on your system.

If you are on Windows, you can use the supplied VSPE file for the [Virtual Serial Ports Emulator by Eterlogic](http://www.eterlogic.com/Products.VSPE.html). If anyone knows any FOSS tool to emualte serial ports, let me know.

On Linux, you can use `socat` to emulate ports. Use the provided bash script `setup_virtual_ports.sh` to set up two ports. This will create two symlinks in your current working directory. Check where these links are pointing to and use these names in `test.cc`.