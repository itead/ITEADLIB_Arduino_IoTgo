@mainpage

# IoTgo Arduino Libaray Based on ESP8266

## Source Code

Read the fucking source code of `IoTgo.h` and `IoTgo.cpp`

## API Documentation

You can find API documentation under directory `doc/`.

# uartWIFI Library (for ESP8266)

## Source Code

Read the fucking source code of uartWIFI.h and uartWIFI.cpp

## Select Your Board

When you use with UNO board, uncomment the follow line in `uartWIFI.h`.

	#define UNO

When you use with MEGA board, uncomment the follow line in `uartWIFI.h`.

	#define MEGA

## Connection

When you use it with UNO board, the connection should be like these:

	ESP8266_TX->D0

	ESP8266_RX->D1

	ESP8266_CHPD->3.3V

	ESP8266_VCC->3.3V

	ESP8266_GND->GND

	//The baud rate of software serial can't be higher that 19200, so we use software serial as a debug port
	FTDI_RX->D3			

	FTDI_TX->D2

When you use it with MEGA board, the connection should be like these:

	ESP8266_TX->RX1(D19)

	ESP8266_RX->TX1(D18)

	ESP8266_CH_PD->3.3V

	ESP8266_VCC->3.3V

	ESP8266_GND->GND

When you want to output the debug information, please use DebugSerial. For example,

	DebugSerial.println("hello");

## Attention

**Note1**:	The size of message from ESP8266 is too big for arduino sometimes, so the library can't receive the whole buffer because  
the size of the hardware serial buffer which is defined in HardwareSerial.h is too small.

Open the file from `\arduino\hardware\arduino\avr\cores\arduino\HardwareSerial.h` .
See the follow line in the HardwareSerial.h file.

	#define SERIAL_BUFFER_SIZE 64

The default size of the buffer is 64. Change it into a bigger number, like 256 or more.

The SRAM size of mega is bigger than UNO's, so it is better to use MEGA board to communicate with ESP8266.


**BUG**: When you use this library and receive the http package, it might miss some char_tacters because the library can't process so much data in the same time.

** Created by Stan Lee(Lizq@iteadstudio.com) **

2014/10/8

**Modified version**

V1.0	released the first version of ESP8266 library

-------------------------------------------------------------------------------

# The End!

-------------------------------------------------------------------------------
