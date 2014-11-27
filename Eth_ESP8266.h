/**
 * @file Eth_ESP8266.h
 *
 * ESP8266 library 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/17
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#ifndef __ETH_ESP8266_H__
#define __ETH_ESP8266_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "IoTgo_config.h"
#include "Net_NetInterface.h"

/* The way of encrypstion */
#define    ESP8266_ENC_OPEN          0
#define    ESP8266_ENC_WEP           1
#define    ESP8266_ENC_WAP_PSK       2
#define    ESP8266_ENC_WAP2_PSK      3
#define    ESP8266_ENC_WAP_WAP2_PSK  4

/* Communication mode */
#define    TCP     1
#define    UDP     0

/* The type of initialized ESP8266 */
#define    STA     1
#define    AP      2
#define    AP_STA  3

/**
 * Provides TCP service for Application Layer of Network Protocol Stack. 
 * 
 * The implemetation is based on ESP8266 by sigle connection mode. 
 *
 * @ingroup NetInterface
 */
class ESP8266: public NetInterface
{
public: /* Implementation of NetInterface */
    virtual int32_t createTCPConnection(String host, uint32_t port);
    virtual int32_t send(String data);
    virtual int32_t recv(char *buffer, uint32_t length);
    virtual int32_t releaseTCPConnection(void);

public: /* public methods */
    bool connectWiFi(String ssid, String password);
    bool disconnectWiFi(void);
    
private:
    void begin(void);
	bool Initialize(byte mode, String ssid, String pwd, byte chl = 1, byte ecn = 2);
	boolean ipConfig(byte type, String addr, int32_t port, boolean a = 0, byte id = 0);
	boolean Send(String str);
	boolean Send(byte id, String str);
	int32_t ReceiveMessage(char *buf);
	
    void Reset(void);
	bool confMode(byte a);
	boolean confJAP(String ssid , String pwd);
	boolean confSAP(String ssid , String pwd , byte chl , byte ecn);
    String showMode(void);
    String showAP(void);
    String showJAP(void);
    boolean quitAP(void);
    String showSAP(void);

    String showStatus(void);
    String showMux(void);
    boolean confMux(boolean a);
    boolean newMux(byte type, String addr, int32_t port);
    boolean newMux(byte id, byte type, String addr, int32_t port);
    void closeMux(void);
    void closeMux(byte id);
    String showIP(void);
    boolean confServer(byte mode, int32_t port);
};

#endif /* #ifndef __ETH_ESP8266_H__ */
