@mainpage

# IoTgo Arduino Library

## About IoTgo 

IoTgo is an open source IoT platform, like WordPress, ZenCart and all other open
source software, you can deploy your own IoTgo cloud platform.

We at ITEAD Studio are committed to provide a complete set of hardware for IoTgo 
Platform with open source hardware designs and open source firmware.

Learn more about IoTgo here <https://github.com/itead/IoTgo>

## Source 

Source can be download at <https://github.com/itead/ITEADLIB_Arduino_IoTgo>.

You can clone it by:

    git clone https://github.com/itead/ITEADLIB_Arduino_IoTgo.git


## Documentation

Online API documentation can be reached at <http://docs.iteadstudio.com/IoTgo-ino/>.

Offline API documentation can be found under directory 
[doc](https://github.com/itead/ITEADLIB_Arduino_IoTgo/tree/master/doc).

For Raspberry Pi users, Segnix is recommended at <https://github.com/itead/Segnix/>.
Segnix ships with an IoTgo library beased on Linux/Raspberry Pi. 

## How to get started

On the home page of API documentation, the tabs of Modules, Classes and Examples 
will be useful for Arduino developers. 

## Project Layers

IoTgo Arduino Library has layers following:

  - IoTgo Device Layer: Switch/Light/THSensor [source: Dev_*]
  - Http Proxy Layer: IoTgo [source: IoTgo*]
  - Network Layer: NetInterface [source: Net_*]
  - Ethernet Layer: ESP8266(WIFI) [source: Eth_*]
  - Hardware Layer: MEGA/WBoard/WBoard Pro

# Configuration and Connection

## Configure Your Board

When you use with MEGA board or WBoard or WBoard Pro, uncomment the follow line 
in `IoTgo_config.h`.

	#define MEGA

## Connection

### MEGA

When you use it with MEGA board, the connection should be like these:

	ESP8266_TX->RX1(D19)

	ESP8266_RX->TX1(D18)

	ESP8266_CH_PD->3.3V

	ESP8266_VCC->3.3V

	ESP8266_GND->GND

When you want to output the debug information, please use DebugSerial. For example,

	DebugSerial.println("hello");

### WBoard and WBoard Pro

No more connections.

# Attention

**Note1**:	The size of message from ESP8266 is too big for arduino sometimes, 
so the library can't receive the whole buffer because the size of the hardware 
serial buffer which is defined in HardwareSerial.h is too small.

Open the file from `\arduino\hardware\arduino\avr\cores\arduino\HardwareSerial.h`.
See the follow line in the HardwareSerial.h file.

	#define SERIAL_BUFFER_SIZE 64

The default size of the buffer is 64. Change it into a bigger number, like 256 
or more.

# Author

- Author: Wu Pengfei(pengfei.wu@itead.cc)
- Date: 11/20/2014 6:27:43 PM 

-------------------------------------------------------------------------------

# The End!

-------------------------------------------------------------------------------
