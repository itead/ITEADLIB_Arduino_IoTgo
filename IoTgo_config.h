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


//#define DEBUG /* comment or uncomment this line to toggle debug message output */ 

#define DebugSerial	                    Serial /* Serial port for debug informations */

/* 
 * Select your way for access to internet according to your hardwares. 
 * ONLY one of three macros following must be defined. 
 * Define NET_USE_ESP8266 if you choose ESP8266 for accessing to internet. 
 * Define NET_USE_W5X00 for W5100/W5200/W5500. 
 * Define NET_USE_GSM for GSM/GPRS SIM900/SIM908/SIM800/SIM808. 
 */
#define NET_USE_ESP8266
//#define NET_USE_W5X00 
//#define NET_USE_GSM


/*
 * Configurations for NET_USE_ESP8266
 */
 
#define ESP8266_BAUD_RATE               (9600)  /* Guess what? */
#define ESP8266Serial                   Serial1 /* UART used between ESP8266 and MCU */


/*
 * Configurations for NET_USE_W5X00
 */

/* 
 * Select your ethernet card if NET_USE_W5X00 defined.
 * ONLY one of three macros following must be defined. 
 */
#define W5100_ETHERNET_SHIELD   /* Arduino Ethenret Shield and Compatibles */
//#define W5200_ETHERNET_SHIELD   /* WIZ820io, W5200 Ethernet Shield */
//#define W5500_ETHERNET_SHIELD   /* WIZ550io, ioShield series of WIZnet */

#define W5X00SPI                        SPI /* SPI used between W5X00 and MCU */


/*
 * Configurations for NET_USE_GSM
 */

#define GSMSerial                       Serial1 /* UART used between GSM and MCU */


#endif /* #ifndef __IOTGO_CONFIG_H__ */
