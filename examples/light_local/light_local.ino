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

#include <Light.h>

#define LIGHT_ID            "5580000003"
#define LIGHT_CHECK_CODE    "86f32f24-70bc-44e0-82f2-e95da6f8cde5"
#define LIGHT_APIKEY        "d0555f12-a67c-4c54-9ee0-8f5b7f4268fa"
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
        Serial.println("on() err!");
    }
    
    delay(1000);
    
    ret = light.off();
    if(ret == 0)
    {
        Serial.println("off");
    }
    else
    {
        Serial.println("off() err!");
    }
    
    delay(1000);
}
