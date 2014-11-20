/**
 * @example light_local.ino
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

#include <IoTgo_device.h>

#define LIGHT_ID            "0200000001"
#define LIGHT_CHECK_CODE    ""
#define LIGHT_APIKEY        "9bed6c2c-fbc6-4400-9ef8-087f9f0d996e"
#define WIFI_SSID           "ITEAD"
#define WIFI_PASS           "12345678"
#define IOT_SERVER          "172.16.7.6"

#define LIGHT_PIN           (13)

Light light(LIGHT_PIN);

void setup()
{
    const char *apikey;
    light.setServer(IOT_SERVER);
    if (!light.connectWiFi(WIFI_SSID, WIFI_PASS))
    {
        Serial.println("connectWiFI error and halt...");
        while(1);
    }

    Serial.println("Connecting device to server...");
    apikey = light.init(LIGHT_ID, LIGHT_APIKEY);
    //apikey = light.init(LIGHT_ID, LIGHT_APIKEY, DEVICE_PRODUCT);
    Serial.print("apikey = ");
    Serial.println(apikey);    
    
    Serial.println("setup done.");
}

void loop()
{
    static int32_t counter = 0;
    Serial.print("\ncounter = ");
    Serial.println(++counter);
    int32_t ret;

    ret = light.on();
    if(ret == 0)
    {
        Serial.println("on");
    }
    else
    {
        Serial.print("on() err! ret = ");
        Serial.println(ret);
    }
    
    delay(1000);
    
    ret = light.off();
    if(ret == 0)
    {
        Serial.println("off");
    }
    else
    {
        Serial.print("off() err! ret = ");
        Serial.println(ret);
    }
    
    delay(1000);
}
