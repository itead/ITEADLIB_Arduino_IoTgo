/**
 * @example iotgo.ino
 *
 * Show how to use API of This Library. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/14
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <IoTgo.h>

#define IOTGO_ID            "5580000003"
#define IOTGO_CHECK_CODE    "86f32f24-70bc-44e0-82f2-e95da6f8cde5"
#define IOTGO_APIKEY        "d0555f12-a67c-4c54-9ee0-8f5b7f4268fa"
#define WIFI_SSID           "ITEAD"
#define WIFI_PASS           "12345678"
#define IOT_SERVER          "172.16.7.6"

IoTgo iotgo;

void printBody(const char *buffer)
{
    if (buffer != NULL)
    {
        buffer = strstr(buffer, "{");
        if (buffer == NULL)
        {
            Serial.println("no \"{\" found!");
            return;
        }
        Serial.print("[");
        Serial.print(buffer);
        Serial.println("]");
    }
}

void setup()
{
    const char *apikey;
    iotgo.setServer(IOT_SERVER);
    if (!iotgo.connectWiFi(WIFI_SSID, WIFI_PASS))
    {
        Serial.println("connectWiFI error and halt...");
        while(1);
    }

    Serial.println("Connecting device to server...");
    apikey = iotgo.init(IOTGO_ID, IOTGO_APIKEY);
    //apikey = iotgo.init(IOTGO_ID, IOTGO_APIKEY, DEVICE_PRODUCT);
    Serial.print("apikey = ");
    Serial.println(apikey);    
    
    Serial.println("setup done.");
}


void loop()
{
    static int counter = 0;
    Serial.print("\ncounter = ");
    Serial.println(++counter);
    
    const char *iotgo_state[] = 
    {
        "state",
        NULL
    };
    const char *iotgo_on[] = 
    {
        "on",
        NULL
    };
    const char *iotgo_off[] = 
    {
        "off",
        NULL
    };
    
    const char *ret;

    Serial.println("update to on");
    ret = iotgo.update(iotgo_state, iotgo_on);
    Serial.print("iotgo.update =  ");
    printBody(ret);

    ret = iotgo.query(iotgo_state);
    Serial.print("iotgo.query =   ");
    printBody(ret);

    Serial.println("update to off");
    ret = iotgo.update(iotgo_state, iotgo_off);
    Serial.print("iotgo.update =  ");
    printBody(ret);

    ret = iotgo.query(NULL);
    Serial.print("iotgo.query =   ");
    printBody(ret);

    delay(1000);
}



