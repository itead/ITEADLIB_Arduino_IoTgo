/**
 * @file IoTgo_config.h
 *
 * Configure some options for your board. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/27
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 
#ifndef __IOTGO_CONFIG_H__
#define __IOTGO_CONFIG_H__

/* comment or uncomment the line below to toggle debug message output */ 
//#define DEBUG 

/* Select your way to access internet */
#define NET_USE_ESP8266
//#define NET_USE_W5X00
//#define NET_USE_GSM

#ifdef NET_USE_ESP8266
#define ESP8266_BAUD_RATE   (9600)
#define ESP8266SERIAL_TIMEOUT_DEFAULT   (3000)
#define ESP8266Serial	Serial1
#define DebugSerial	    Serial
#endif  

#ifdef NET_USE_W5X00
#define DebugSerial Serial
#endif

#ifdef NET_USE_GSM
#define DebugSerial Serial
#endif


#endif /* #ifndef __IOTGO_CONFIG_H__ */
