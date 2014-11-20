/**
 * @file IoTgo.h
 *
 * API of IoTgo (iotgo.iteadstudio.com)
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/11
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 
#ifndef __ITEADIOT_H__
#define __ITEADIOT_H__

#include <string.h>
#include <SoftwareSerial.h>

#include "Eth_ESP8266.h"
#include "IoTgo_error.h"

/**
 * @addtogroup IoTgo
 * @{
 */

/**
 * Constants of IoTgo platform. 
 *
 * Maybe these are useless for users. 
 */
enum IoTgoConstant
{
    IOT_BUFFER_SIZE     = 1024, /**< the length of http request */
    APIKEY_LEN          = 36,   /**< the length of apikey */
    DEVICE_ID_LEN       = 10,   /**< the length of device id */
};

/**
 * Indicates the type of device. 
 *
 * Used in method: @ref IoTgo::init. If the deviceid is created by developers on website
 * of IoTgo platform, the type is DEVICE_DIY. Else DEVICE_PRODUCT. 
 */
enum IoTgoDeviceType
{
    DEVICE_DIY      = 0,    /**< DIY deviceid */
    DEVICE_PRODUCT  = 1,    /**< Product deviceid */
};


/** 
 * IoTgo provides some simple API for device of IoT. 
 */
class IoTgo
{
public: /* public methods */
    IoTgo(void);
    void setServer(const char *server);
    bool connectWiFi(const char *ssid, const char *password);
    
    const char *init(const char *device_id, const char *apikey, 
        IoTgoDeviceType device_type = DEVICE_DIY);
    const char *query(const char *params[]);
    const char *update(const char *params[], const char *values[]);

  
private: /* private methods */  
    const char * request(const char *http_body, char *const buffer, int32_t len);
    

private: /* private datas */
    WIFI wifi;
    char buffer[IOT_BUFFER_SIZE];
    char apikey[APIKEY_LEN + 1];
    char device_id[DEVICE_ID_LEN + 1];
    char server[20];
};

/** @} */

#endif /* #ifndef __ITEADIOT_H__ */
