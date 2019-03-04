Linux GUI -arduino board_LED control-

This example creates a widget in linux which is used to turn on/off the builtin led of arduino board (UNO/nano/ESP8266) and logs it. A log file will be created in the same source folder named "serData.txt".

Compiler: GCC
Platform: Linux/Ubuntu
Widget toolkit: GTK+
Interface: Serial


Install necessary dependencies of GTK+:
 sudo apt-get install build-essential
 sudo apt-get install libgtk2.0-doc devhelp
 sudo apt-get install libgtk2.0-dev

To compile the program with GCC use the format: 
gcc guiserialcntrl.c -o led `pkg-config --cflags --libs gtk+-2.0`


Arduino ino project configures 9600 serial port and uses BUILTIN_LED.
