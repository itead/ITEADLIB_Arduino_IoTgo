@mainpage

# IoTgo Arduino Library

# About IoTgo 

IoTgo is an open source IoT platform, like WordPress, ZenCart and all other open
source software, you can deploy your own IoTgo cloud platform.

We at ITEAD Studio are committed to provide a complete set of hardware for IoTgo 
Platform with open source hardware designs and open source firmware.

Learn more about IoTgo here <https://github.com/itead/IoTgo>

# Source 

Source can be download at <https://github.com/itead/ITEADLIB_Arduino_IoTgo>.

You can clone it by:

    git clone https://github.com/itead/ITEADLIB_Arduino_IoTgo.git


# Documentation

Online API documentation can be reached at <http://docs.iteadstudio.com/IoTgo-ino/>.

Offline API documentation can be found under directory 
[doc](https://github.com/itead/ITEADLIB_Arduino_IoTgo/tree/master/doc).

For Raspberry Pi users, Segnix is recommended at <https://github.com/itead/Segnix/>.
Segnix ships with an IoTgo library beased on Linux/Raspberry Pi. 

# How to get started

On the home page of API documentation, the tabs of Modules, Classes and Examples 
will be useful for Arduino developers. 

# Project Layers

IoTgo Arduino Library has layers following:

  - IoTgo Device Layer: Switch, Light, THSensor [source: Dev_*]
  - Http Proxy Layer: IoTgo [source: IoTgo*]
  - Network Layer: NetInterface [source: Net_*]
  - Ethernet Layer: ESP8266(WIFI), W5100/W5200/W5500, GSM/GPRS(SIM908/900/808/800)[source: Eth_*]
  - Hardware Layer: MEGA/WBoard/WBoard Pro

# Mainboard Requires

  - RAM: not less than 8KBytes
  - UART: one hardware serial at least 

# Suppported Mainboards

  - WBoard Pro
  - MEGA and its derivatives

# Configuration

All work of configuration is to comment or uncomment macros related. File: `IoTgo_config.h`
lists all macros which users can modify. Comments around macros will be helpful
to users.

# Connection

Depending on your way to access to internet, your hardware connection may be different.
Besides, another connection will be needed if you use examples shipped with this library.
Just be careful and patient when connecting all hardwares together. 

-------------------------------------------------------------------------------

# The End!

-------------------------------------------------------------------------------
